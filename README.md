# Campus Navigation System

A menu-driven campus navigation application built in **C++** using graph data structures and Dijkstra's shortest-path algorithm. It helps users find the shortest walking route between campus locations and manage the campus map dynamically.

## Features

- **View campus locations** with their location IDs.
- **Find the shortest route** between two locations.
- **Display route distance** in meters.
- **Estimate walking time** using an assumed walking speed of 80 meters per minute.
- **Search locations** by name using case-insensitive partial matching.
- **View campus connections** and their distances.
- **Add locations and connections** while the program is running.
- **Validate user input**, including empty names, duplicate locations, invalid IDs, and duplicate connections.
- **Save and load the campus map** using a local `campus.txt` file, so changes persist between runs.

## Technologies Used

- C++
- Standard Template Library (STL)
- File handling (`<fstream>`)

## Data Structures and Algorithms

| Concept | How it is used |
|---|---|
| Graph | Represents campus locations and paths |
| Adjacency list | Stores each location's directly connected locations |
| Priority queue | Efficiently selects the next closest location in Dijkstra's algorithm |
| Dijkstra's algorithm | Calculates shortest routes for non-negative distances |
| Parent array | Reconstructs the route from destination back to source |
| Vectors | Store locations, graph edges, distances, and the final route |
| File handling | Saves and restores the campus map |

**Shortest-path complexity:** With an adjacency-list graph and a min-priority queue, Dijkstra's algorithm runs in approximately **O((V + E) log V)**, where **V** is the number of locations and **E** is the number of connections.

## Project Structure

```text
Campus-Navigation/
├── data/
│   └── sample-campus.txt
├── src/
│   └── srcCode.cc
├── .gitignore
└── README.md
```

## Default Campus Map

When no saved `campus.txt` file is found, the application initializes this map:

**Locations**

1. Main Gate
2. Library
3. Canteen
4. CSE Department
5. Auditorium

**Default connections**

| From | To | Distance |
|---|---|---:|
| Main Gate | Library | 200 m |
| Main Gate | Canteen | 300 m |
| Library | Canteen | 180 m |
| Library | CSE Department | 250 m |
| Canteen | CSE Department | 100 m |
| CSE Department | Auditorium | 150 m |

A copy of this starter map is included at `data/sample-campus.txt`. To use it as the initial saved map, copy it into the program's working directory and name it `campus.txt`.

**Windows (PowerShell):**

```powershell
Copy-Item data/sample-campus.txt campus.txt
```

**Linux / macOS:**

```bash
cp data/sample-campus.txt campus.txt
```

The application reads and writes `campus.txt` in its current working directory. If that file is absent, it creates the default map automatically.

## Getting Started

### Prerequisites

Install a C++ compiler such as **g++**.

### Clone the repository

```bash
git clone https://github.com/aadityavasishtag-arch/Campus-Navigation.git
cd Campus-Navigation
```

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

**Windows (PowerShell):**

```powershell
.\\campus.exe
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

Enter the location IDs shown by the program when selecting a source and destination. Distances are stored in meters.

## Possible Future Improvements

- Display multiple route alternatives.
- Support editing and deleting locations or connections.
- Build a graphical campus map with highlighted routes.
- Add landmarks, building categories, and accessibility-aware routes.
- Develop a web interface for interactive navigation.

## Author

**Aditya G**

---

*This project is developed as a C++ data structures and algorithms project and can be extended into an interactive campus navigation web application.*
