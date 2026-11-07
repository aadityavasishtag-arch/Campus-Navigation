
const API_URL = "http://localhost:18080";

// Get HTML elements
const sourceSelect = document.getElementById("source");
const destinationSelect = document.getElementById("destination");
const findRouteButton = document.getElementById("findRoute");
const resultDiv = document.getElementById("result");
//const locationsList = document.getElementById("locationsList");
const locationsList = document.getElementById("locationsList");
const locationCount = document.getElementById("locationCount");
const addLocationForm = document.getElementById("addLocationForm");
const newLocationName = document.getElementById("newLocationName");
const locationMessage = document.getElementById("locationMessage");
const addConnectionForm = document.getElementById("addConnectionForm");
const connectionFrom = document.getElementById("connectionFrom");
const connectionTo = document.getElementById("connectionTo");
const connectionDistance = document.getElementById("connectionDistance");
const connectionMessage = document.getElementById("connectionMessage");
// Load campus locations from C++ backend
async function loadLocations() {

    try {

        const response = await fetch(`${API_URL}/locations`);

        if (!response.ok) {
            throw new Error("Unable to load locations");
        }

        const data = await response.json();
        // Generate map positions dynamically


        // Fetch campus connections from the C++ backend
        const connectionsResponse = await fetch(`${API_URL}/connections`);

        if (!connectionsResponse.ok) {
            throw new Error("Unable to load campus connections");
        }

        const connectionsData = await connectionsResponse.json();

        mapConnections = connectionsData.connections;
        // Generate map coordinates for every location
        mapLocations = {};
        const centerX = 400;
        const centerY = 210;
        const radiusX = 300;
        const radiusY = 140;


        data.locations.forEach((location, index) => {

            const angle =
                (2 * Math.PI * index / data.locations.length)
                - Math.PI / 2;

            const x = centerX + radiusX * Math.cos(angle);
            const y = centerY + radiusY * Math.sin(angle);

            mapLocations[location] = { x, y };
        });

        locationCount.textContent = data.locations.length;

        // Clear existing options
        sourceSelect.innerHTML = '<option value="">Select starting location</option>';
        destinationSelect.innerHTML = '<option value="">Select destination</option>';

        connectionFrom.innerHTML = '<option value="">Select starting location</option>';
        connectionTo.innerHTML = '<option value="">Select destination</option>';

        locationsList.innerHTML = "";




        // Populate dropdowns and campus list
        data.locations.forEach(location => {
            // Draw the map using backend connection data
            //renderCampusMap();

            const sourceOption = document.createElement("option");
            sourceOption.value = location;
            sourceOption.textContent = location;
            sourceSelect.appendChild(sourceOption);

            const destinationOption = document.createElement("option");
            destinationOption.value = location;
            destinationOption.textContent = location;
            destinationSelect.appendChild(destinationOption);
            // Add location to connection dropdowns
            const fromOption = document.createElement("option");
            fromOption.value = location;
            fromOption.textContent = location;
            connectionFrom.appendChild(fromOption);

            const toOption = document.createElement("option");
            toOption.value = location;
            toOption.textContent = location;
            connectionTo.appendChild(toOption);

            const listItem = document.createElement("li");
            listItem.textContent = location;
            locationsList.appendChild(listItem);

        });
        renderCampusMap();

    }
    catch (error) {

        console.error("Location loading error:", error);

        resultDiv.textContent =
            "Unable to connect to the campus server. Please check whether the backend is running.";

        locationsList.innerHTML =
            "<li>Could not load campus locations.</li>";

    }
}

// Campus map node positions
let mapLocations = {};

// Campus connections and distances
// Connections will be loaded from the C++ backend
let mapConnections = [];


// Draw campus map and highlight shortest route
function renderCampusMap(route = []) {

    const svg = document.getElementById("campusMap");

    if (!svg) return;

    const routeSet = new Set();

    // Identify connections belonging to shortest route
    for (let i = 0; i < route.length - 1; i++) {
        routeSet.add(`${route[i]}|${route[i + 1]}`);
        routeSet.add(`${route[i + 1]}|${route[i]}`);
    }

    let mapHTML = "";

    // Draw campus connections
    mapConnections.forEach(connection => {

        const start = mapLocations[connection.from];
        const end = mapLocations[connection.to];
        if (!start || !end) {
            console.warn(
                "Missing map coordinates:",
                JSON.stringify(connection),
                "Available locations:",
                Object.keys(mapLocations)
            );
            return;
        }

        const isHighlighted = routeSet.has(
            `${connection.from}|${connection.to}`
        );

        mapHTML += `
            <line
                x1="${start.x}"
                y1="${start.y}"
                x2="${end.x}"
                y2="${end.y}"
                class="map-edge ${isHighlighted ? "highlighted" : ""}"
            />

            <text
                x="${(start.x + end.x) / 2}"
                y="${(start.y + end.y) / 2 - 10}"
                class="map-distance"
            >
                ${connection.distance}m
            </text>
        `;
    });


    // Draw campus locations
    Object.entries(mapLocations).forEach(([name, position]) => {

        let nodeClass = "";

        const selectedSource =
            document.getElementById("source").value;

        const selectedDestination =
            document.getElementById("destination").value;

        if (name === selectedSource) {
            nodeClass = "start-node";
        }
        else if (name === selectedDestination) {
            nodeClass = "end-node";
        }
        else if (route.includes(name)) {
            nodeClass = "route-node";
        }

        mapHTML += `
           <circle
    cx="${position.x}"
    cy="${position.y}"
    r="16"
    class="map-node ${nodeClass}"
    data-location="${name}"
    style="cursor: pointer;"
/>

            <text
                x="${position.x}"
                y="${position.y + 38}"
                class="map-label"
            >
                ${name}
            </text>
        `;
    });

    svg.innerHTML = mapHTML;
    // Add click events to campus location markers
    svg.querySelectorAll(".map-node").forEach(node => {

        node.addEventListener("click", () => {

            const location = node.dataset.location;

            const source = document.getElementById("source");
            const destination = document.getElementById("destination");

            if (source.value === "") {
                source.value = location;
            }
            else if (destination.value === "") {
                destination.value = location;
            }
            else {
                destination.value = location;
            }

            // Refresh marker colors immediately
            renderCampusMap();

        });

    });
}


// Find shortest route
async function findShortestRoute() {

    const source = sourceSelect.value;
    const destination = destinationSelect.value;

    // Validate selections
    if (!source || !destination) {

        resultDiv.textContent =
            "Please select both starting location and destination.";

        return;
    }

    if (source === destination) {

        resultDiv.textContent =
            "Starting location and destination must be different.";

        return;
    }

    resultDiv.textContent = "Calculating shortest route...";

    try {

        const url =
            `${API_URL}/route?source=${encodeURIComponent(source)}` +
            `&destination=${encodeURIComponent(destination)}`;

        const response = await fetch(url);

        if (!response.ok) {
            throw new Error("Route request failed");
        }

        const data = await response.json();

        if (!data.found) {

            resultDiv.textContent = "No route found between these locations.";

            renderCampusMap();

            return;
        }
        // Highlight the shortest route on the campus map
        renderCampusMap(data.path);

        // Display route details
        const routeHTML = data.path
            .map(location => `<li>${location}</li>`)
            .join("");

        resultDiv.innerHTML = `
            <h3>Shortest Route Found!</h3>

            <p><strong>Route:</strong></p>

            <ol>${routeHTML}</ol>

            <p>
                <strong>Total Distance:</strong>
                ${data.totalDistance} metres
            </p>

            <p>
                <strong>Walking Time:</strong>
                ${data.walkingTime.toFixed(2)} minutes
            </p>
        `;

    }
    catch (error) {

        console.error("Route error:", error);

        resultDiv.textContent =
            "Unable to calculate route. Please check the backend connection.";

    }
}


// Button event
findRouteButton.addEventListener("click", findShortestRoute);


// Update map whenever source or destination changes
sourceSelect.addEventListener("change", () => {
    renderCampusMap();
});

destinationSelect.addEventListener("change", () => {
    renderCampusMap();
});

addLocationForm.addEventListener("submit", async (event) => {
    event.preventDefault();

    const name = newLocationName.value.trim();

    if (!name) {
        locationMessage.textContent = "Please enter a location name.";
        return;
    }

    try {
        const response = await fetch(`${API_URL}/locations`, {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({ name: name })
        });

        const message = await response.text();

        if (!response.ok) {
            throw new Error(message);
        }

        locationMessage.textContent = "Location added successfully!";
        newLocationName.value = "";

        await loadLocations();

    } catch (error) {
        console.error("Add location error:", error);
        locationMessage.textContent =
            error.message || "Unable to add location.";
    }
});
// Add New Connection
addConnectionForm.addEventListener("submit", async (event) => {

    event.preventDefault();

    const from = connectionFrom.value;
    const to = connectionTo.value;
    const distance = Number(connectionDistance.value);

    // Validate inputs
    if (!from || !to) {
        connectionMessage.textContent = "Please select both locations.";
        return;
    }

    if (from === to) {
        connectionMessage.textContent =
            "Starting and destination locations cannot be the same.";
        return;
    }

    if (!Number.isInteger(distance) || distance <= 0) {
        connectionMessage.textContent =
            "Please enter a valid positive distance in metres.";
        return;
    }

    try {

        const response = await fetch(`${API_URL}/connections`, {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({
                from: from,
                to: to,
                distance: distance
            })
        });

        const message = await response.text();

        if (!response.ok) {
            throw new Error(message);
        }

        connectionMessage.textContent =
            "Connection added successfully!";

        connectionDistance.value = "";

        await loadLocations();

    }
    catch (error) {

        console.error("Add connection error:", error);

        connectionMessage.textContent =
            error.message || "Unable to add connection.";

    }

});
// Draw the initial campus map
renderCampusMap();

// Load locations when webpage opens
loadLocations();