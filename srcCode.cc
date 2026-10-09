
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
using namespace std;

const int N = 5;

string places[N] = {
    "Main Gate",
    "Library",
    "Canteen",
    "CSE Department",
    "Auditorium"
};

vector<pair<int, int>> graph[N];

void addEdge(int u, int v, int distance) {
    graph[u].push_back({v, distance});
    graph[v].push_back({u, distance});
}

void shortestPath(int source, int destination) {
    vector<int> dist(N, INT_MAX);
    vector<int> parent(N, -1);

    priority_queue<
        pair<int, int>,
        vector<pair<int, int>>,
        greater<pair<int, int>>
    > pq;

    dist[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d != dist[u])
            continue;

        for (auto edge : graph[u]) {
            int v = edge.first;
            int weight = edge.second;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    if (dist[destination] == INT_MAX) {
        cout << "No route available.\n";
        return;
    }

    vector<int> path;
    for (int v = destination; v != -1; v = parent[v])
        path.push_back(v);

    reverse(path.begin(), path.end());

    cout << "\nShortest route: ";
    for (int i = 0; i < path.size(); i++) {
        cout << places[path[i]];
        if (i != path.size() - 1)
            cout << " -> ";
    }

    cout << "\nTotal distance: "
         << dist[destination] << " metres\n";
}

int main() {
    // Sample campus paths (distances in metres)
    addEdge(0, 1, 200); // Main Gate - Library
    addEdge(0, 2, 300); // Main Gate - Canteen
    addEdge(1, 2, 180); // Library - Canteen
    addEdge(1, 3, 250); // Library - CSE Department
    addEdge(2, 3, 100); // Canteen - CSE Department
    addEdge(3, 4, 150); // CSE Department - Auditorium

    cout << "=== CAMPUS NAVIGATION SYSTEM ===\n";

    cout << "\nAvailable locations:\n";
    for (int i = 0; i < N; i++)
        cout << i << ". " << places[i] << '\n';

    int source, destination;

    cout << "\nEnter source location number: ";
    cin >> source;

    cout << "Enter destination location number: ";
    cin >> destination;

    if (source < 0 || source >= N ||
        destination < 0 || destination >= N) {
        cout << "Invalid location number.\n";
        return 0;
    }

    shortestPath(source, destination);

    return 0;
}
