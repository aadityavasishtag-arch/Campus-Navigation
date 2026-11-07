#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

struct Edge {
    int destination;
    int distance;
};
struct Connection {
    string from;
    string to;
    int distance;
};
struct RouteResult {
    bool found;
    int totalDistance;
    vector<string> path;
};

class NavigationGraph {
private:
    vector<string> locations;
    vector<vector<Edge>> adjList;

public:
    bool addLocation(string name);
    bool addConnection(
        string source,
        string destination,
        int distance
    );

    vector<string> getLocations();
    vector<Connection> getConnections() const;

    bool saveToFile(string filename);
    bool loadFromFile(string filename);
    RouteResult findShortestPath(
        string source,
        string destination
    );
};

#endif