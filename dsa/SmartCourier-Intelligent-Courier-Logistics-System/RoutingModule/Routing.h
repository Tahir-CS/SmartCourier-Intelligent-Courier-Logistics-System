#ifndef ROUTING_H
#define ROUTING_H

#include <string>
#include <vector>
#include "../include/CustomStructures.h"

class RoutingManager {
private:
    Graph routingNetwork;

public:
    RoutingManager();
    void findShortestPath(std::string start, std::string end);
    void displayNetwork();
    void blockRoute(std::string from, std::string to);
    void unblockRoute(std::string from, std::string to);
};

#endif