
#include "crow.h"
#include "navigation.h"
#include <fstream>
// CORS middleware
struct CorsMiddleware
{

    struct context
    {
    };

    void before_handle(
        crow::request &req,
        crow::response &res,
        context &ctx)
    {
        // No action needed before the request
    }

    void after_handle(
        crow::request &req,
        crow::response &res,
        context &ctx)
    {
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");
    }
};

int main()
{

    crow::App<CorsMiddleware> app;

    NavigationGraph campus;

    // Load saved campus data or create the default network
ifstream savedData("data/campus_data.txt");

if (savedData.good()) {

    savedData.close();

    if (!campus.loadFromFile("data/campus_data.txt")) {
        cerr << "Error: Could not load campus data!" << endl;
        return 1;
    }

    cout << "Campus data loaded successfully!" << endl;

}
else {

    // First-time setup: Add default locations
    campus.addLocation("Main Gate");
    campus.addLocation("Library");
    campus.addLocation("Canteen");
    campus.addLocation("CSE Department");
    campus.addLocation("Auditorium");

    // Add default connections
    campus.addConnection("Main Gate", "Library", 200);
    campus.addConnection("Main Gate", "Canteen", 300);
    campus.addConnection("Library", "Canteen", 180);
    campus.addConnection("Library", "CSE Department", 250);
    campus.addConnection("Canteen", "CSE Department", 100);
    campus.addConnection("CSE Department", "Auditorium", 150);

    if (!campus.saveToFile("data/campus_data.txt")) {
        cerr << "Error: Could not save campus data!" << endl;
        return 1;
    }

    cout << "Default campus data created!" << endl;
}

    // API 1: Get all campus locations
    CROW_ROUTE(app, "/locations")
    ([&campus]()
     {

        crow::json::wvalue result;

        crow::json::wvalue::list locationList;

for (string location : campus.getLocations()) {
    locationList.emplace_back(location);
}

result["locations"] = std::move(locationList);

        return result; });
    // API: Add a new campus location
    CROW_ROUTE(app, "/locations")
        .methods(crow::HTTPMethod::POST)([&campus](const crow::request &req)
                                {

    auto body = crow::json::load(req.body);

    // Validate JSON input
    if (!body || !body.has("name") ||
    body["name"].t() != crow::json::type::String) {

        return crow::response(
            400,
            "Invalid request. Provide a location name."
        );
    }

    string name = body["name"].s();

    // Add location to campus graph
    if (!campus.addLocation(name)) {

        return crow::response(
            400,
            "Location already exists or name is invalid."
        );
    }
    if (!campus.saveToFile("data/campus_data.txt")) {
    return crow::response(
        500,
        "Location added, but saving to file failed."
    );
}
    crow::json::wvalue result;

    result["success"] = true;
    result["message"] = "Location added successfully.";
    result["name"] = name;

    crow::response response(result);
    response.code = 201;

    return response; });
   // API: Add a connection between two campus locations
CROW_ROUTE(app, "/connections")
.methods(crow::HTTPMethod::POST)
([&campus](const crow::request& req) {

    auto body = crow::json::load(req.body);

    if (!body ||
        !body.has("from") ||
        !body.has("to") ||
        !body.has("distance") ||
        body["from"].t() != crow::json::type::String ||
        body["to"].t() != crow::json::type::String ||
        body["distance"].t() != crow::json::type::Number) {

        return crow::response(
            400,
            "Invalid connection details."
        );
    }

    string source = body["from"].s();
    string destination = body["to"].s();
    int distance = body["distance"].i();

    if (!campus.addConnection(source, destination, distance)) {

        return crow::response(
            400,
            "Invalid or duplicate connection."
        );
    }
    if (!campus.saveToFile("data/campus_data.txt")) {
    return crow::response(
        500,
        "Connection added, but saving to file failed."
    );
}
    crow::json::wvalue result;

    result["success"] = true;
    result["message"] = "Connection added successfully.";

    return crow::response(201, result);
});

    // Handle browser CORS preflight requests
    CROW_ROUTE(app, "/locations")
        .methods("OPTIONS"_method)([]()
                                   { return crow::response(204); });

    // API 2: Get all campus connections
    CROW_ROUTE(app, "/connections")
    ([&campus]()
     {

    crow::json::wvalue result;
    crow::json::wvalue::list connectionList;

    for (const Connection& connection : campus.getConnections()) {

        crow::json::wvalue item;

        item["from"] = connection.from;
        item["to"] = connection.to;
        item["distance"] = connection.distance;

        connectionList.emplace_back(std::move(item));
    }

    result["connections"] = std::move(connectionList);

    return result; });
    // API 2: Find the shortest route
    CROW_ROUTE(app, "/route")
    ([&campus](const crow::request &req)
     {

        const char* source = req.url_params.get("source");
        const char* destination = req.url_params.get("destination");

        if (source == nullptr || destination == nullptr) {
            crow::response response(400, "Source and destination are required");
            return response;
        }

        RouteResult route = campus.findShortestPath(source, destination);

        crow::json::wvalue result;

        result["found"] = route.found;

        if (route.found) {

            result["totalDistance"] = route.totalDistance;

            result["walkingTime"] = route.totalDistance / 80.0;

           crow::json::wvalue::list pathList;

for (string location : route.path) {
    pathList.emplace_back(location);
}

result["path"] = std::move(pathList);
        }

        return crow::response(result); });

    cout << "Campus Navigation API is running!\n";
    cout << "Open http://localhost:18080/locations\n";

    app.port(18080).multithreaded().run();

    return 0;
}
