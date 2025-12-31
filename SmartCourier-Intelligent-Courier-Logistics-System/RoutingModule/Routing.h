#ifndef ROUTING_H
#define ROUTING_H

#include "../SharedData.h"
#include <vector>
#include <limits>

const int INF = 99999; // Represents "Infinite" distance

// A connection between cities
struct Edge {
    int targetNode;
    int weight; // Distance
};

class RoutingEngine {
private:
    int numNodes;
    // Adjacency List: vector of vectors of Edges
    vector<vector<Edge>> adjList; 
    vector<string> cityNames;

    int getCityIndex(string name);

public:
    RoutingEngine(); // Constructor builds the map
    void addRoute(string src, string dest, int weight);
    void findShortestPath(string startCity, string endCity);
};

#endif
