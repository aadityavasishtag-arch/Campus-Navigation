# Campus Navigation System

A menu-driven campus navigation application built in **C++** using graph data structures and Dijkstra's shortest-path algorithm. It helps users find the shortest walking route between campus locations and manage the campus map dynamically.

## Features

- **View campus locations** with their location IDs.
- **Find the shortest route** between two locations.
- **Display route distance** in meters.
- **Estimate walking time** using a fixed assumed walking speed of 80 meters per minute.
- **Search locations** by name (case-insensitive partial matching).
- **View campus connections** and their distances.
- **Add locations** while the program is running.
- **Add connections** between locations while the program is running.
- **Save and load the campus map** using a local `campus.txt` file, so changes persist between runs.

## Technologies Used

- C++
- Standard Template Library (STL)
- File handling (`<fstream>`)

## Data Structures and Algorithms

| Concept | How it is used |
|---|---|
| Graph | Represents the campus and the paths between locations |
| Adjacency list | Stores each location's directly connected locations |
| Priority queue | Efficiently selects the next closest location in Dijkstra's algorithm |
| Dijkstra's algorithm | Calculates the shortest route for non-negative distances |
| Parent array | Reconstructs the route from destination back to source |
| Vectors | Store locations, graph edges, distances, and the final route |
| File handling | Saves and restores the campus map |

**Shortest-path complexity:** With an adjacency-list graph and a min-priority queue, Dijkstra's algorithm runs in approximately (O((V+E)\log V)), where (V) is the number of locations and (E) is the number of connections.

## Default Campus Map

The initial map contains these locations:

1. Main Gate
2. Library
3. Canteen
4. CSE Department
5. Auditorium

Default connections (distances are in meters):

| From | To | Distance |
|---|---|---:|
| Main Gate | Library | 200 |
| Main Gate | Canteen | 300 |
| Library | Canteen | 180 |
| Library | CSE Department | 250 |
| Canteen | CSE Department | 100 |
| CSE Department | Auditorium | 150 |

The map can be extended from the application menu; these are only the initial sample locations and paths.

## Getting Started

### Prerequisites

Install a C++ compiler such as **g++**.

### Compile

**Windows (PowerShell / Command Prompt):**

```bash
g++ -std=c++11 src/srcCode.cc -o campus.exe
```

**Linux / macOS:**

```bash
g++ -std=c++11 src/srcCode.cc -o campus
```

### Run

**Windows:**

```bash
campus.exe
```

**Linux / macOS:**

```bash
./campus
```

## Using the Application

The menu provides these operations:

1. Display campus locations
2. Find the shortest route
3. Search for a campus location
4. Display campus connections
5. Add a new location
6. Add a connection
7. Save the campus map
8. Load the campus map
0. Exit

Enter the location IDs shown by the program when selecting a source and destination. The application stores distances in meters.

### Map persistence

The program reads the saved map from `campus.txt` when it starts. If no saved map is available, it initializes the default campus map. Adding a location or connection saves the updated map; the Save and Load menu options are also available manually.

Keep `campus.txt` in the program's **working directory** when running the application. If the file is missing, the default map is initialized.

## Possible Future Improvements

- Display multiple route alternatives.
- Support editing and deleting locations or connections.
- Validate location names and prevent duplicate locations or connections.
- Validate saved map data before loading it.
- Add a graphical campus map with highlighted routes.
- Build a web interface for interactive navigation.
- Add landmarks, building categories, and accessibility-aware routes.

## Author

**Aditya G**

---

*This project is developed as a C++ data structures and algorithms project and can be extended into an interactive campus navigation web application.*
