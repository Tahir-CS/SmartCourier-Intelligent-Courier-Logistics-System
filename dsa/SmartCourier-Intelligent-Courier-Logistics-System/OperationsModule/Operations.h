#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "../include/CustomStructures.h"
#include "../SortingModule/Sorter.h"
#include "../RoutingModule/Routing.h"
#include "../TrackingModule/Tracker.h"
#include "../include/UI_Utils.h"

class CourierLogisticsSystem {
private:
    HashTable<Parcel> parcelDatabase;
    MinHeap priorityQueue;
    Queue<Parcel> pickupQueue;
    Queue<Parcel> warehouseQueue;
    Queue<Parcel> transitQueue;
    Graph routingNetwork;
    OperationLog operationLog;
    DestinationBST destinationTree;  // BST for destination-based organization
    int nextParcelId;
    void initializeNetwork();
public:
    CourierLogisticsSystem();
    void registerParcel();
    void viewParcelDetails();
    void processPickupQueue();
    void processWarehouseQueue();
    void processTransitQueue();
    void viewPriorityQueue();
    void viewAllQueues();
    void findRoute();
    void manageRoutes();
    void viewOperationLog();
    void markParcelMissing();
    void generateReport();
    void displayMainMenu();
    void undoLastOperation();  // Undo method
    void viewParcelsByDestination();  // BST view method
};

#endif // OPERATIONS_H
