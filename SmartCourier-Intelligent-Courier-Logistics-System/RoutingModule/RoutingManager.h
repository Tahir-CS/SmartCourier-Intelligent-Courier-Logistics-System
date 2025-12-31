#ifndef ROUTINGMANAGER_H
#define ROUTINGMANAGER_H

#include <string>
#include "GlobalConstants.h"

// --- Edge Structure ---
// Represents a road between two cities.
struct Edge {
    std::string destination;
    int distance;
    bool isBlocked; // Feature: Can mark roads as blocked
    Edge* next;     // Linked list of edges for Adjacency List
    Edge(std::string dest, int dist) : destination(dest), distance(dist), isBlocked(false), next(nullptr) {}
};

// --- Graph Node Structure ---
// Represents a City in the network.
struct GraphNode {
    std::string city;
    Edge* edges;     // Head of the list of connected roads (Edges)
    GraphNode* next; // Linked list of Cities (Nodes)
    
    GraphNode(std::string c) : city(c), edges(nullptr), next(nullptr) {}
    
    // Add a road connected to this city
    void addEdge(std::string dest, int dist) {
        Edge* newEdge = new Edge(dest, dist);
        if(!edges) edges = newEdge;
        else {
            // Append to end of edge list
            Edge* temp = edges;
            while(temp->next) temp->next;
            temp->next = newEdge;
        }
    }
};

// --- Routing Manager Class ---
// Manages the Graph and runs Dijkstra's Algorithm.
class RoutingManager {
private:
    GraphNode* head; // Head of the list of all cities
    GraphNode* findNode(std::string city); // Helper to find a city node by name

public:
    RoutingManager();
    ~RoutingManager();
    void addRoute(std::string from, std::string to, int distance);
    void findShortestPath(std::string start, std::string end); // Dijkstra's Logic
    void displayNetwork(); // Show map
};
#endif