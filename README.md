# CampusNav — Campus Navigation System

CampusNav is a campus wayfinding application that combines a **C++ graph-based navigation backend** with a **web dashboard**. It helps users explore campus locations, manage connections between them, and find the shortest walking route using Dijkstra's algorithm.

The project began as a menu-driven C++ application and has been extended with a Crow REST API and an interactive HTML, CSS, and JavaScript frontend.

## Features

### Navigation
- Find the shortest route between two campus locations.
- Calculate route distance in metres.
- Estimate walking time using an assumed speed of **80 metres per minute**.
- Use Dijkstra's shortest-path algorithm with an adjacency-list graph and a priority queue.

### Campus management
- View the available campus locations.
- View connections and their distances.
- Add new campus locations through the web dashboard.
- Add a connection between two existing locations and specify its distance.
- Refresh the location lists and schematic map after updates.
- Validate connection input, including missing locations, identical endpoints, and non-positive distances.

### Web dashboard
- Route planner with dynamically populated source and destination selectors.
- Interactive schematic campus map showing locations and connections.
- Shortest-route visualization.
- Campus location and connection management forms.
- Responsive dashboard layout with side-by-side management cards on desktop and stacked cards on smaller screens.

### Original console application
The repository also retains the original menu-driven C++ console application, including location search, map display, and its file save/load functionality.

## Technologies Used

| Technology | Purpose |
|---|---|
| C++ (C++17 for the backend) | Graph model, navigation logic, and API server |
| Crow | HTTP REST API |
| Standalone Asio | Networking support for Crow |
| HTML5 | Dashboard structure |
| CSS3 | Dashboard styling and responsive layout |
| JavaScript | API requests, form handling, route display, and map updates |
| CMake | Backend build configuration |
| Python HTTP server | Local frontend development server |

## Data Structures and Algorithms

| Concept | How it is used |
|---|---|
| Graph | Represents campus locations and paths |
| Adjacency list | Stores the connections for each location |
| Priority queue | Selects the next closest location in Dijkstra's algorithm |
| Dijkstra's algorithm | Finds shortest routes for non-negative edge weights |
| Parent information | Reconstructs the route from destination to source |
| Vectors | Store locations, graph edges, distances, and paths |

**Dijkstra's time complexity:** With an adjacency-list graph and a min-priority queue, the algorithm runs in approximately **O((V + E) log V)**, where **V** is the number of locations and **E** is the number of connections.

## Project Structure

```text
Campus-Navigation/
├── backend/
│   ├── crow/                 # Crow header files
│   ├── crow.h
│   ├── main.cpp              # REST API routes and server
│   ├── navigation.cpp       # Graph and shortest-path implementation
│   └── navigation.h
├── frontend/
│   ├── index.html            # Dashboard
│   ├── style.css             # Dashboard styles
│   └── script.js             # API integration and map rendering
├── data/
│   └── sample-campus.txt
├── src/
│   └── srcCode.cc            # Original console application
├── CMakeLists.txt
├── .gitignore
└── README.md
```

## Default Campus Map

The backend's starter campus graph contains these locations:

1. Main Gate
2. Library
3. Canteen
4. CSE Department
5. Auditorium

Default connections:

| From | To | Distance |
|---|---|---:|
| Main Gate | Library | 200 m |
| Main Gate | Canteen | 300 m |
| Library | Canteen | 180 m |
| Library | CSE Department | 250 m |
| Canteen | CSE Department | 100 m |
| CSE Department | Auditorium | 150 m |

The web dashboard can add further locations and connections while the backend is running.

> **Current storage note:** The web backend currently keeps dynamically added locations and connections in memory. They are reset when the backend process is restarted. Permanent storage for the web backend is a planned improvement. The original console application has its own file-based save/load functionality.

## Getting Started

### Prerequisites

For the full-stack version on Windows, install:

- Git
- CMake
- GCC / MinGW-w64 (the MSYS2 UCRT64 toolchain is used for development)
- Standalone Asio
- Python 3

Crow's header files are included in the `backend/crow/` directory.

If using MSYS2 UCRT64, install Asio from the **MSYS2 UCRT64** terminal:

```bash
pacman -S mingw-w64-ucrt-x86_64-asio
```

### 1. Clone the repository

```bash
git clone https://github.com/aadityavasishtag-arch/Campus-Navigation.git
cd Campus-Navigation
```

### 2. Build the C++ backend

From the project root, configure and build the backend:

```powershell
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
```

If the `build` directory has already been configured with a different CMake generator, use that existing build configuration or remove and recreate the build directory before configuring with MinGW Makefiles.

### 3. Start the backend

In **PowerShell**, from the project root:

```powershell
.\build\campus_backend.exe
```

The Crow API server runs at:

```text
http://localhost:18080
```

Keep this terminal open while using the dashboard.

### 4. Start the frontend

Open a **second terminal**. From the project root, run:

```powershell
cd frontend
py -m http.server 5500
```

Open the dashboard in your browser:

```text
http://localhost:5500
```

Keep both terminals running: one for the C++ backend and one for the frontend server.

## REST API

The frontend communicates with the Crow backend at `http://localhost:18080`.

| Method | Endpoint | Purpose |
|---|---|---|
| GET | `/locations` | Retrieve campus locations |
| POST | `/locations` | Add a campus location |
| GET | `/connections` | Retrieve campus connections |
| POST | `/connections` | Add a connection and its distance |
| GET | `/route` | Calculate a shortest route |

The POST endpoints accept JSON request bodies. The route endpoint accepts the source and destination as query parameters.

## Original Console Application

The original C++ console program is available at `src/srcCode.cc`.

Compile with g++:

**Windows:**
```powershell
g++ -std=c++11 src/srcCode.cc -o campus.exe
.\campus.exe
```

**Linux / macOS:**
```bash
g++ -std=c++11 src/srcCode.cc -o campus
./campus
```

The console application provides menu operations for displaying locations, finding shortest routes, searching locations, viewing connections, adding locations and connections, and saving/loading the campus map.

## Future Improvements

- Add permanent file-based or database storage to the web backend.
- Support editing and deleting locations and connections.
- Add route alternatives and accessibility-aware navigation.
- Improve map positioning with real campus coordinates.
- Deploy the web application so it can be accessed online.

## Author

**Aditya G**

---

*CampusNav is a data structures and algorithms project extended into a full-stack campus navigation application.*
