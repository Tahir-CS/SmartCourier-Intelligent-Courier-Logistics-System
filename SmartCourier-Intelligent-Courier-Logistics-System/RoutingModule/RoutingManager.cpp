#include "RoutingManager.h"
#include <iostream>
#include <iomanip>

using namespace std;

// Constructor: Initializes the default map with realistic Pakistani cities/distances.
RoutingManager::RoutingManager() : head(nullptr) {
    addRoute("Lahore", "Islamabad", 380);
    addRoute("Lahore", "Multan", 340);
    addRoute("Islamabad", "Peshawar", 180);
    addRoute("Multan", "Karachi", 900);
    addRoute("Karachi", "Quetta", 690);
}

// Destructor: Cleans up all Nodes and Edges to prevent memory leaks.
RoutingManager::~RoutingManager() {
    while(head) {
        GraphNode* temp = head;
        head = head->next;
        // Delete all edges for this city
        while(temp->edges) {
            Edge* e = temp->edges;
            temp->edges = temp->edges->next;
            delete e;
        }
        delete temp; // Delete the city node
    }
}

// Find a City Node by its name string.
GraphNode* RoutingManager::findNode(string city) {
    GraphNode* temp = head;
    while(temp) {
        if(temp->city == city) return temp;
        temp = temp->next;
    }
    return nullptr;
}

// Add a bi-directional route (undirected graph)
void RoutingManager::addRoute(string from, string to, int distance) {
    if(!findNode(from)) {
        GraphNode* n = new GraphNode(from);
        n->next = head; head = n;
    }
    if(!findNode(to)) {
        GraphNode* n = new GraphNode(to);
        n->next = head; head = n;
    }
    findNode(from)->addEdge(to, distance);
    findNode(to)->addEdge(from, distance);
}

// --- DIJKSTRA'S ALGORITHM ---
// Finds the shortest path between start and end using a Greedy approach.
void RoutingManager::findShortestPath(string start, string end) {
    const int MAX = 50; // Max cities assumption for simple array use
    string cities[MAX];
    int dist[MAX];      // Stores shortest distance from Start
    bool visited[MAX];  // Keeps track of visited nodes
    string prev[MAX];   // To reconstruct the path
    int count = 0;

    // 1. Initialize arrays
    GraphNode* temp = head;
    while(temp && count < MAX) {
        cities[count] = temp->city;
        dist[count] = 999999; // Set distance to Infinity
        visited[count] = false;
        count++;
        temp = temp->next;
    }

    // 2. Set Start Node distance to 0
    int startIdx = -1, endIdx = -1;
    for(int i=0; i<count; i++) {
        if(cities[i] == start) { startIdx = i; dist[i] = 0; }
        if(cities[i] == end) endIdx = i;
    }

    if(startIdx == -1 || endIdx == -1) {
        cout << RED << "  Invalid City." << RESET << endl;
        return;
    }

    // 3. Main Loop
    for(int i=0; i<count-1; i++) {
        // Find the unvisited node with smallest distance
        int min = 999999, u = -1;
        for(int v=0; v<count; v++)
            if(!visited[v] && dist[v] <= min) { min = dist[v]; u = v; }
        
        if(u == -1) break; // No more reachable nodes
        visited[u] = true;

        // Relax neighbors
        GraphNode* curr = findNode(cities[u]);
        Edge* e = curr->edges;
        while(e) {
            int v = -1;
            // Find index of neighbor
            for(int k=0; k<count; k++) if(cities[k] == e->destination) v = k;
            
            // Update distance if a shorter path is found
            if(v != -1 && !visited[v] && dist[u] + e->distance < dist[v]) {
                dist[v] = dist[u] + e->distance;
                prev[v] = cities[u]; // Store predecessor for path reconstruction
            }
            e = e->next;
        }
    }

    // 4. Output Results
    if(dist[endIdx] == 999999) cout << RED << "  No path found." << RESET << endl;
    else {
        cout << GREEN << "  Distance: " << dist[endIdx] << " km" << RESET << endl;
        // Reconstruct path string backwards from End -> Start
        string path = end;
        string curr = end;
        while(curr != start) {
            for(int i=0; i<count; i++) {
                if(cities[i] == curr) {
                    curr = prev[i];
                    path = curr + " -> " + path;
                    break;
                }
            }
        }
        cout << "  Route: " << YELLOW << path << RESET << endl;
    }
}

// Print all cities and connections
void RoutingManager::displayNetwork() {
    cout << MAGENTA << "\n  === Network Map ===" << RESET << endl;
    GraphNode* temp = head;
    while(temp) {
        cout << "  " << BOLD << temp->city << RESET << ": ";
        Edge* e = temp->edges;
        while(e) {
            cout << e->destination << "(" << e->distance << ") ";
            e = e->next;
        }
        cout << endl;
        temp = temp->next;
    }
}