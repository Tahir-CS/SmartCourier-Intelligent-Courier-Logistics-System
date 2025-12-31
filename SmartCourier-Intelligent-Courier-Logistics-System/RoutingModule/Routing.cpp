#include "Routing.h"
#include <queue>
#include <algorithm>

RoutingEngine::RoutingEngine() {
    numNodes = 5;
    adjList.resize(numNodes);
    cityNames = {"Lahore", "Islamabad", "Karachi", "Multan", "Peshawar"};

    // Hardcoded Map for Demo
    // 0:LHR, 1:ISL, 2:KHI, 3:MUL, 4:PEW
    addRoute("Lahore", "Islamabad", 380);
    addRoute("Lahore", "Multan", 340);
    addRoute("Islamabad", "Peshawar", 180);
    addRoute("Multan", "Karachi", 900);
    addRoute("Lahore", "Karachi", 1200); // Long direct route
}

int RoutingEngine::getCityIndex(string name) {
    for (int i = 0; i < cityNames.size(); i++) {
        if (cityNames[i] == name) return i;
    }
    return -1;
}

void RoutingEngine::addRoute(string src, string dest, int weight) {
    int u = getCityIndex(src);
    int v = getCityIndex(dest);
    if (u != -1 && v != -1) {
        adjList[u].push_back({v, weight});
        adjList[v].push_back({u, weight}); // Undirected graph (road goes both ways)
    }
}

void RoutingEngine::findShortestPath(string startCity, string endCity) {
    int start = getCityIndex(startCity);
    int end = getCityIndex(endCity);

    if (start == -1 || end == -1) {
        cout << "[Error] Invalid City Names!\n";
        return;
    }

    // Dijkstra's Algorithm
    vector<int> dist(numNodes, INF);
    vector<int> parent(numNodes, -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if (d > dist[u]) continue;

        for (auto& edge : adjList[u]) {
            int v = edge.targetNode;
            int weight = edge.weight;
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    // Reconstruct Path
    if (dist[end] == INF) {
        cout << "No route exists between " << startCity << " and " << endCity << endl;
        return;
    }

    cout << "\n[Routing] Optimal Route Calculated: ";
    vector<int> path;
    for (int v = end; v != -1; v = parent[v])
        path.push_back(v);
    
    reverse(path.begin(), path.end());

    for (int i = 0; i < path.size(); i++) {
        cout << cityNames[path[i]];
        if (i < path.size() - 1) cout << " -> ";
    }
    cout << "\nTotal Distance: " << dist[end] << " km\n";
}
