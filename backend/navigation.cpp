#include "navigation.h"
#include <queue>
#include <algorithm>
#include <limits>
#include <cctype>
#include <fstream>
using namespace std;

bool NavigationGraph::addLocation(string name) {
    if (name.empty()) {
        return false;
    }

    for (string location : locations) {
        if (location == name) {
            return false;
        }
    }

    locations.push_back(name);
    adjList.push_back({});

    return true;
}

bool NavigationGraph::addConnection(
    string source,
    string destination,
    int distance
) {
    if (distance <= 0 || source == destination) {
        return false;
    }

    int u = -1, v = -1;

    for (int i = 0; i < locations.size(); i++) {
        if (locations[i] == source) u = i;
        if (locations[i] == destination) v = i;
    }

    if (u == -1 || v == -1) {
        return false;
    }

    for (Edge e : adjList[u]) {
        if (e.destination == v) {
            return false;
        }
    }

    adjList[u].push_back({v, distance});
    adjList[v].push_back({u, distance});

    return true;
}

vector<string> NavigationGraph::getLocations() {
    return locations;
}
vector<Connection> NavigationGraph::getConnections() const {

    vector<Connection> connections;

    for (int i = 0; i < locations.size(); i++) {

        for (const Edge& edge : adjList[i]) {

            // Avoid adding the same undirected connection twice
            if (i < edge.destination) {

                connections.push_back({
                    locations[i],
                    locations[edge.destination],
                    edge.distance
                });

            }
        }
    }

    return connections;
}
bool NavigationGraph::saveToFile(string filename) {

    ofstream file(filename);

    if (!file.is_open()) {
        return false;
    }

    // Save all locations
    file << locations.size() << endl;

    for (const string& location : locations) {
        file << location << endl;
    }

    // Save all connections
    vector<Connection> connections = getConnections();

    file << connections.size() << endl;

    for (const Connection& connection : connections) {
        file << connection.from << endl;
        file << connection.to << endl;
        file << connection.distance << endl;
    }

    file.close();

    return true;
}


bool NavigationGraph::loadFromFile(string filename) {

    ifstream file(filename);

    if (!file.is_open()) {
        return false;
    }

    int locationCount;

    if (!(file >> locationCount) || locationCount < 0) {
        return false;
    }

    file.ignore(numeric_limits<streamsize>::max(), '\n');

    // Load locations
    for (int i = 0; i < locationCount; i++) {

        string name;

        if (!getline(file, name)) {
            return false;
        }

        if (!addLocation(name)) {
            return false;
        }
    }

    // Load connections
    int connectionCount;

    if (!(file >> connectionCount) || connectionCount < 0) {
        return false;
    }

    file.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int i = 0; i < connectionCount; i++) {

        string source;
        string destination;
        int distance;

        if (!getline(file, source)) {
            return false;
        }

        if (!getline(file, destination)) {
            return false;
        }

        if (!(file >> distance)) {
            return false;
        }

        file.ignore(numeric_limits<streamsize>::max(), '\n');

        if (!addConnection(source, destination, distance)) {
            return false;
        }
    }

    file.close();

    return true;
}
RouteResult NavigationGraph::findShortestPath(
    string source,
    string destination
) {
    int n = locations.size();

    int src = -1, dest = -1;

    for (int i = 0; i < n; i++) {
        if (locations[i] == source) src = i;
        if (locations[i] == destination) dest = i;
    }

    RouteResult result;
    result.found = false;
    result.totalDistance = 0;

    if (src == -1 || dest == -1) {
        return result;
    }

    vector<int> dist(n, numeric_limits<int>::max());
    vector<int> parent(n, -1);

    priority_queue<
        pair<int, int>,
        vector<pair<int, int>>,
        greater<pair<int, int>>
    > pq;

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        for (Edge e : adjList[u]) {
            int v = e.destination;
            int weight = e.distance;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;

                pq.push({dist[v], v});
            }
        }
    }

    if (dist[dest] == numeric_limits<int>::max()) {
        return result;
    }

    vector<string> path;

    for (int v = dest; v != -1; v = parent[v]) {
        path.push_back(locations[v]);
    }

    reverse(path.begin(), path.end());

    result.found = true;
    result.totalDistance = dist[dest];
    result.path = path;

    return result;
}