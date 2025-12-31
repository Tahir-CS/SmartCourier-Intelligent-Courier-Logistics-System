#include "Routing.h"
#include <iostream>
#include <limits>

using namespace std;

RoutingManager::RoutingManager() {
    // Initialize default routes
    routingNetwork.addRoute("Lahore", "Islamabad", 380);
    routingNetwork.addRoute("Lahore", "Multan", 340);
    routingNetwork.addRoute("Lahore", "Faisalabad", 130);
    routingNetwork.addRoute("Islamabad", "Rawalpindi", 15);
    routingNetwork.addRoute("Islamabad", "Peshawar", 180);
    routingNetwork.addRoute("Islamabad", "Lahore", 380);
    routingNetwork.addRoute("Multan", "Bahawalpur", 100);
    routingNetwork.addRoute("Multan", "Lahore", 340);
    routingNetwork.addRoute("Faisalabad", "Sargodha", 90);
    routingNetwork.addRoute("Faisalabad", "Lahore", 130);
    routingNetwork.addRoute("Karachi", "Hyderabad", 165);
    routingNetwork.addRoute("Karachi", "Multan", 900);
    routingNetwork.addRoute("Peshawar", "Islamabad", 180);
}

void RoutingManager::findShortestPath(string start, string end) {
    routingNetwork.findShortestPath(start, end);
}

void RoutingManager::displayNetwork() {
    routingNetwork.displayNetwork();
}

void RoutingManager::blockRoute(string from, string to) {
    routingNetwork.blockRoute(from, to);
}

void RoutingManager::unblockRoute(string from, string to) {
    routingNetwork.unblockRoute(from, to);
}