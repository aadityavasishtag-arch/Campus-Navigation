
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <limits>
#include <algorithm>
#include <cctype>
#include <fstream>

using namespace std;

struct Edge
{
    int destination;
    int distance;
};

const int INF = numeric_limits<int>::max();

string toLowerCase(string text);

vector<string> locations = {
    "Main Gate",
    "Library",
    "Canteen",
    "CSE Department",
    "Auditorium"};

vector<vector<Edge>> graph(locations.size());

// Add an undirected connection between two locations
void addEdge(int source, int destination, int distance)
{
    graph[source].push_back({destination, distance});
    graph[destination].push_back({source, distance});
}

void saveCampusMap()
{
    ofstream file("campus.txt");

    if (!file)
    {
        cout << "Error saving campus map.\n";
        return;
    }

    // Save number of locations
    file << locations.size() << endl;

    // Save location names
    for (const string &name : locations)
    {
        file << name << endl;
    }

    // Count undirected edges
    int edgeCount = 0;

    for (int i = 0; i < graph.size(); i++)
    {
        for (const Edge &edge : graph[i])
        {
            if (i < edge.destination)
            {
                edgeCount++;
            }
        }
    }

    file << edgeCount << endl;

    // Save each undirected connection only once
    for (int i = 0; i < graph.size(); i++)
    {
        for (const Edge &edge : graph[i])
        {
            if (i < edge.destination)
            {
                file << i << " "
                     << edge.destination << " "
                     << edge.distance << endl;
            }
        }
    }

    file.close();

    cout << "Campus map saved successfully!\n";
}

bool loadCampusMap()
{
    ifstream file("campus.txt");

    if (!file)
    {
        cout << "No saved campus map found.\n";
        return false;
    }

    int locationCount;
    file >> locationCount;
    file.ignore(numeric_limits<streamsize>::max(), '\n');

    if (locationCount <= 0)
    {
        cout << "Invalid campus map file.\n";
        return false;
    }

    vector<string> loadedLocations;

    for (int i = 0; i < locationCount; i++)
    {
        string name;
        getline(file, name);

        if (!file)
        {
            cout << "Invalid campus map file.\n";
            return false;
        }

        if (name.find_first_not_of(" \t\r") == string::npos)
        {
            cout << "Invalid campus map file: empty location name.\n";
            return false;
        }

        for (const string &existingName : loadedLocations)
        {
            if (toLowerCase(existingName) == toLowerCase(name))
            {
                cout << "Invalid campus map file: duplicate location name.\n";
                return false;
            }
        }

        loadedLocations.push_back(name);
    }

    int edgeCount;
    file >> edgeCount;

    if (!file || edgeCount < 0)
    {
        cout << "Invalid campus map file.\n";
        return false;
    }

    vector<vector<Edge>> loadedGraph(locationCount);

    for (int i = 0; i < edgeCount; i++)
    {
        int source, destination, distance;

        file >> source >> destination >> distance;

        if (!file ||
            source < 0 || source >= locationCount ||
            destination < 0 || destination >= locationCount ||
            source == destination ||
            distance <= 0)
        {

            cout << "Invalid connection in file.\n";
            return false;
        }

        bool duplicateConnection = false;
        for (const Edge &existingEdge : loadedGraph[source])
        {
            if (existingEdge.destination == destination)
            {
                duplicateConnection = true;
                break;
            }
        }

        if (duplicateConnection)
        {
            cout << "Invalid campus map file: duplicate connection.\n";
            return false;
        }

        loadedGraph[source].push_back({destination, distance});
        loadedGraph[destination].push_back({source, distance});
    }

    locations = loadedLocations;
    graph = loadedGraph;

    file.close();

    cout << "Campus map loaded successfully!\n";
    return true;
}

// Create the campus map
void createCampusMap()
{
    addEdge(0, 1, 200); // Main Gate - Library
    addEdge(0, 2, 300); // Main Gate - Canteen
    addEdge(1, 2, 180); // Library - Canteen
    addEdge(1, 3, 250); // Library - CSE Department
    addEdge(2, 3, 100); // Canteen - CSE Department
    addEdge(3, 4, 150); // CSE Department - Auditorium
}

// Display all campus locations
void displayLocations()
{
    cout << "\n===== CAMPUS LOCATIONS =====\n";

    for (int i = 0; i < locations.size(); i++)
    {
        cout << i + 1 << ". " << locations[i] << endl;
    }
}

// Display connected locations
void displayCampusMap()
{
    cout << "\n===== CAMPUS CONNECTIONS =====\n";

    for (int i = 0; i < graph.size(); i++)
    {
        cout << "\n"
             << locations[i] << ":\n";

        for (const Edge &edge : graph[i])
        {
            cout << "  -> " << locations[edge.destination]
                 << " (" << edge.distance << " meters)\n";
        }
    }
}

// Convert string to lowercase
string toLowerCase(string text)
{
    transform(text.begin(), text.end(), text.begin(),
              [](unsigned char ch)
              {
                  return tolower(ch);
              });

    return text;
}

// Search for a campus location
void searchLocation()
{
    string searchName;

    cout << "\nEnter location name to search: ";
    getline(cin, searchName);

    if (searchName.find_first_not_of(" \t\r") == string::npos)
    {
        cout << "Please enter a location name to search.\n";
        return;
    }

    string searchLower = toLowerCase(searchName);
    bool found = false;

    for (int i = 0; i < locations.size(); i++)
    {
        if (toLowerCase(locations[i]).find(searchLower) != string::npos)
        {

            cout << "Found: " << locations[i]
                 << " (Location ID: " << i + 1 << ")\n";

            found = true;
        }
    }

    if (!found)
    {
        cout << "Location not found.\n";
    }
}

// Find shortest route using Dijkstra's algorithm
void shortestPath(int source, int destination)
{
    int n = locations.size();

    vector<int> distance(n, INF);
    vector<int> parent(n, -1);

    // Min-priority queue: {distance, node}
    priority_queue<
        pair<int, int>,
        vector<pair<int, int>>,
        greater<pair<int, int>>>
        pq;

    distance[source] = 0;
    pq.push({0, source});

    while (!pq.empty())
    {
        int currentDistance = pq.top().first;
        int currentNode = pq.top().second;
        pq.pop();

        if (currentDistance > distance[currentNode])
        {
            continue;
        }

        for (const Edge &edge : graph[currentNode])
        {
            int nextNode = edge.destination;
            int newDistance = currentDistance + edge.distance;

            if (newDistance < distance[nextNode])
            {
                distance[nextNode] = newDistance;
                parent[nextNode] = currentNode;

                pq.push({newDistance, nextNode});
            }
        }
    }

    if (distance[destination] == INF)
    {
        cout << "\nNo route available between these locations.\n";
        return;
    }

    // Reconstruct shortest path
    vector<int> path;
    int current = destination;

    while (current != -1)
    {
        path.push_back(current);
        current = parent[current];
    }

    reverse(path.begin(), path.end());

    cout << "\n===== SHORTEST ROUTE =====\n";

    for (int i = 0; i < path.size(); i++)
    {
        cout << locations[path[i]];

        if (i != path.size() - 1)
        {
            cout << " -> ";
        }
    }

    cout << "\n\nTotal Distance: "
         << distance[destination] << " meters\n";

    // Estimate walking time at 80 meters per minute
    int walkingTime = (distance[destination] + 79) / 80;

    cout << "Estimated Walking Time: "
         << walkingTime << " minutes\n";
}

// Read and validate integer input
int getIntegerInput()
{
    int value;

    while (!(cin >> value))
    {
        cout << "Invalid input. Enter a number: ";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    return value;
}

void addLocation()
{
    string name;

    cout << "\nEnter new location name: ";
    getline(cin, name);

    if (name.find_first_not_of(" \t\r") == string::npos)
    {
        cout << "Location name cannot be empty.\n";
        return;
    }

    for (const string &existingName : locations)
    {
        if (toLowerCase(existingName) == toLowerCase(name))
        {
            cout << "A location with this name already exists.\n";
            return;
        }
    }

    locations.push_back(name);
    graph.emplace_back();

    cout << "Location added successfully!\n";
    cout << "New Location ID: " << locations.size() << endl;
    saveCampusMap();
}

void addConnection()
{
    displayLocations();

    cout << "\nEnter first location ID: ";
    int source = getIntegerInput();

    cout << "Enter second location ID: ";
    int destination = getIntegerInput();

    cout << "Enter distance in meters: ";
    int distance = getIntegerInput();

    int n = locations.size();

    if (source < 1 || source > n ||
        destination < 1 || destination > n)
    {
        cout << "Invalid location ID.\n";
        return;
    }

    if (source == destination)
    {
        cout << "A location cannot connect to itself.\n";
        return;
    }

    if (distance <= 0)
    {
        cout << "Distance must be positive.\n";
        return;
    }

    for (const Edge &edge : graph[source - 1])
    {
        if (edge.destination == destination - 1)
        {
            cout << "A connection between these locations already exists.\n";
            return;
        }
    }

    addEdge(source - 1, destination - 1, distance);

    cout << "Connection added successfully!\n";
    saveCampusMap();
}

// Main menu
int main()
{
    if (!loadCampusMap())
    {
        createCampusMap();
        saveCampusMap();
    }

    int choice;

    do
    {
        cout << "\n=================================\n";
        cout << "   CAMPUS NAVIGATION SYSTEM\n";
        cout << "=================================\n";

        cout << "1. Display Campus Locations\n";
        cout << "2. Find Shortest Route\n";
        cout << "3. Search Campus Location\n";
        cout << "4. Display Campus Connections\n";
        cout << "5. Add New Location\n";
        cout << "6. Add Connection\n";
        cout << "7. Save Campus Map\n";
        cout << "8. Load Campus Map\n";
        cout << "0. Exit\n";

        cout << "\nEnter your choice: ";
        choice = getIntegerInput();

        switch (choice)
        {
        case 1:
            displayLocations();
            break;

        case 2:
        {
            displayLocations();

            cout << "\nEnter Source Location ID: ";
            int source = getIntegerInput();

            cout << "Enter Destination Location ID: ";
            int destination = getIntegerInput();

            if (source < 1 || source > locations.size() ||
                destination < 1 || destination > locations.size())
            {
                cout << "Invalid location ID.\n";
            }
            else if (source == destination)
            {
                cout << "\nYou are already at "
                     << locations[source - 1] << ".\n";
                cout << "Distance: 0 meters\n";
            }
            else
            {
                shortestPath(source - 1, destination - 1);
            }

            break;
        }

        case 3:
            searchLocation();
            break;

        case 4:
            displayCampusMap();
            break;
        case 5:
            addLocation();
            break;

        case 6:
            addConnection();
            break;

        case 7:
            saveCampusMap();
            break;

        case 8:
            loadCampusMap();
            break;
        case 0:
            cout << "\nThank you for using Campus Navigation System!\n";
            break;

        default:
            cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 0);

    return 0;
}
