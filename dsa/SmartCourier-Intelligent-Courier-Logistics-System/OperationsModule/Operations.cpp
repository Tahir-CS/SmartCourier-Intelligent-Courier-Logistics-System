#include "Operations.h"
#include "../include/UI_Utils.h"
#include <iostream>
#include <cstdlib>

using namespace std;

void CourierLogisticsSystem::initializeNetwork() {
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
    routingNetwork.addRoute("Quetta", "Multan", 650);
    routingNetwork.addRoute("Rawalpindi", "Lahore", 370);
}

CourierLogisticsSystem::CourierLogisticsSystem() : nextParcelId(1001) {
    initializeNetwork();
}

void CourierLogisticsSystem::registerParcel() {
    UI::printHeader("REGISTER NEW PARCEL");
    string sender, recipient, origin, destination;
    int priChoice=3, wtChoice=1;
    double weight=1.0;
    cout << UI::cyan() << "Sender Name: " << UI::reset();
    cin.ignore();
    getline(cin, sender);
    cout << UI::cyan() << "Recipient Name: " << UI::reset(); getline(cin, recipient);
    cout << UI::cyan() << "Origin City: " << UI::reset(); getline(cin, origin);
    cout << UI::cyan() << "Destination City: " << UI::reset(); getline(cin, destination);
    cout << UI::cyan() << "\nPriority Options:" << UI::reset() << endl;
    UI::printOption(1, "Overnight");
    UI::printOption(2, "2-Day");
    UI::printOption(3, "Normal");
    cout << UI::cyan() << "Select Priority: " << UI::reset(); cin >> priChoice;
    cout << UI::cyan() << "\nWeight Category:" << UI::reset() << endl;
    UI::printOption(1, "Light (<5kg)");
    UI::printOption(2, "Medium (5-20kg)");
    UI::printOption(3, "Heavy (>20kg)");
    cout << UI::cyan() << "Select Category: " << UI::reset(); cin >> wtChoice;
    cout << UI::cyan() << "Exact Weight (kg): " << UI::reset(); cin >> weight;
    Priority priority = static_cast<Priority>(priChoice);
    WeightCategory weightCat = static_cast<WeightCategory>(wtChoice);
    Parcel* newParcel = new Parcel(nextParcelId, sender, recipient, origin, destination, priority, weightCat, weight);
    parcelDatabase.insert(nextParcelId, newParcel);
    priorityQueue.insert(newParcel);
    pickupQueue.enqueue(newParcel);
    destinationTree.insert(newParcel);  // Insert into BST by destination
    newParcel->addEvent(origin, IN_PICKUP_QUEUE, "Added to pickup queue");
    operationLog.push("REGISTER", nextParcelId, "Parcel registered: " + origin + " → " + destination);
    cout << endl; UI::printSuccess("Parcel registered successfully!");
    cout << "Tracking ID: " << UI::yellow() << "PKG" << setfill('0') << setw(6) << nextParcelId << UI::reset() << endl;
    nextParcelId++;
    UI::waitForEnter();
}

void CourierLogisticsSystem::viewParcelDetails() {
    UI::printHeader("TRACK PARCEL");
    int id; cout << UI::cyan() << "Enter Parcel ID: PKG" << UI::reset(); cin >> id;
    Parcel* parcel = parcelDatabase.search(id);
    if(!parcel) UI::printError("Parcel not found!");
    else { cout << endl; parcel->printDetails(); parcel->printTimeline(); }
    UI::waitForEnter();
}

void CourierLogisticsSystem::processPickupQueue() {
    UI::printHeader("PROCESS PICKUP QUEUE");
    if(pickupQueue.isEmpty()){ UI::printWarning("No parcels in pickup queue"); UI::waitForEnter(); return; }
    pickupQueue.display("PICKUP QUEUE");
    cout << UI::cyan() << "\nProcess how many parcels? (0 to cancel): " << UI::reset(); int count; cin >> count;
    for(int i=0;i<count && !pickupQueue.isEmpty(); i++){
        Parcel* parcel = pickupQueue.dequeue();
        parcel->addEvent(parcel->currentLocation, PICKED_UP, "Picked up by courier");
        warehouseQueue.enqueue(parcel);
        parcel->addEvent("Warehouse Hub", AT_WAREHOUSE, "Arrived at warehouse for sorting");
        operationLog.push("PICKUP", parcel->id, "Picked up from " + parcel->origin);
        UI::printSuccess("PKG" + to_string(parcel->id) + " picked up and sent to warehouse");
    }
    UI::waitForEnter();
}

void CourierLogisticsSystem::processWarehouseQueue() {
    UI::printHeader("PROCESS WAREHOUSE QUEUE");
    if(warehouseQueue.isEmpty()){ UI::printWarning("No parcels in warehouse"); UI::waitForEnter(); return; }
    warehouseQueue.display("WAREHOUSE QUEUE");
    cout << UI::cyan() << "\nProcess how many parcels? (0 to cancel): " << UI::reset(); int count; cin >> count;
    for(int i=0;i<count && !warehouseQueue.isEmpty(); i++){
        Parcel* parcel = warehouseQueue.dequeue();
        parcel->addEvent("Warehouse Hub", IN_TRANSIT, "Dispatched from warehouse");
        transitQueue.enqueue(parcel);
        operationLog.push("WAREHOUSE", parcel->id, "Processed at warehouse");
        UI::printSuccess("PKG" + to_string(parcel->id) + " dispatched for transit");
    }
    UI::waitForEnter();
}

void CourierLogisticsSystem::processTransitQueue() {
    UI::printHeader("PROCESS TRANSIT QUEUE");
    if(transitQueue.isEmpty()){ UI::printWarning("No parcels in transit"); UI::waitForEnter(); return; }
    transitQueue.display("TRANSIT QUEUE");
    cout << UI::cyan() << "\nDeliver how many parcels? (0 to cancel): " << UI::reset(); int count; cin >> count;
    for(int i=0;i<count && !transitQueue.isEmpty(); i++){
        Parcel* parcel = transitQueue.dequeue();
        parcel->addEvent(parcel->destination, OUT_FOR_DELIVERY, "Out for final delivery");
        parcel->deliveryAttempts++;
        if(rand() % 10 < 9){
            parcel->addEvent(parcel->destination, DELIVERED, "Successfully delivered to recipient");
            parcel->deliveryTime = getCurrentTimestamp();
            operationLog.push("DELIVER", parcel->id, "Successfully delivered");
            UI::printSuccess("PKG" + to_string(parcel->id) + " delivered successfully!");
        } else {
            parcel->addEvent(parcel->destination, OUT_FOR_DELIVERY, "Delivery failed - recipient unavailable");
            transitQueue.enqueue(parcel);
            operationLog.push("ATTEMPT", parcel->id, "Delivery attempt #" + to_string(parcel->deliveryAttempts));
            UI::printWarning("PKG" + to_string(parcel->id) + " delivery failed - will retry");
        }
    }
    UI::waitForEnter();
}

void CourierLogisticsSystem::viewPriorityQueue(){ UI::printHeader("PRIORITY SORTED PARCELS"); priorityQueue.display(); UI::waitForEnter(); }

void CourierLogisticsSystem::viewAllQueues(){ UI::printHeader("ALL QUEUES STATUS"); UI::printOption(1, "PICKUP QUEUE:"); if(!pickupQueue.isEmpty()) pickupQueue.display("Pickup Queue"); else UI::printInfo("Empty"); cout<<endl; UI::printOption(2, "WAREHOUSE QUEUE:"); if(!warehouseQueue.isEmpty()) warehouseQueue.display("Warehouse Queue"); else UI::printInfo("Empty"); cout<<endl; UI::printOption(3, "TRANSIT QUEUE:"); if(!transitQueue.isEmpty()) transitQueue.display("Transit Queue"); else UI::printInfo("Empty"); UI::waitForEnter(); }

void CourierLogisticsSystem::findRoute(){ UI::printHeader("ROUTE FINDER"); string start,end; cout<<UI::cyan()<<"Starting City: "<<UI::reset(); cin.ignore(); getline(cin,start); cout<<UI::cyan()<<"Destination City: "<<UI::reset(); getline(cin,end); routingNetwork.findShortestPath(start,end); UI::waitForEnter(); }

void CourierLogisticsSystem::manageRoutes(){ UI::printHeader("ROUTE MANAGEMENT"); UI::printOption(1, "View Network"); UI::printOption(2, "Block Route"); UI::printOption(3, "Unblock Route"); cout<<UI::cyan()<<"Choice: "<<UI::reset(); int choice; cin>>choice; if(choice==1) routingNetwork.displayNetwork(); else if(choice==2||choice==3){ string from,to; cout<<UI::cyan()<<"From City: "<<UI::reset(); cin.ignore(); getline(cin,from); cout<<UI::cyan()<<"To City: "<<UI::reset(); getline(cin,to); if(choice==2){ routingNetwork.blockRoute(from,to); operationLog.push("BLOCK_ROUTE",0,from+" → "+to);} else { routingNetwork.unblockRoute(from,to); operationLog.push("UNBLOCK_ROUTE",0,from+" → "+to);} } UI::waitForEnter(); }

void CourierLogisticsSystem::viewOperationLog(){ UI::printHeader("OPERATION LOG"); operationLog.displayLog(); UI::waitForEnter(); }

void CourierLogisticsSystem::markParcelMissing(){ UI::printHeader("MARK PARCEL AS MISSING"); int id; cout<<UI::cyan()<<"Enter Parcel ID: PKG"<<UI::reset(); cin>>id; Parcel* parcel = parcelDatabase.search(id); if(!parcel) UI::printError("Parcel not found!"); else { parcel->addEvent(parcel->currentLocation, MISSING, "Parcel marked as missing - investigation initiated"); operationLog.push("MISSING", id, "Marked as missing"); UI::printWarning("Parcel PKG" + to_string(id) + " marked as MISSING"); } UI::waitForEnter(); }

void CourierLogisticsSystem::generateReport(){ UI::printHeader("SYSTEM ANALYTICS REPORT"); int inPickup=pickupQueue.getSize(); int atWarehouse=warehouseQueue.getSize(); int inTransit=transitQueue.getSize(); cout << "Total Parcels Registered : " << (nextParcelId-1001) << endl; cout << "In Pickup Queue          : " << inPickup << endl; cout << "At Warehouse             : " << atWarehouse << endl; cout << "In Transit               : " << inTransit << endl; UI::waitForEnter(); }

void CourierLogisticsSystem::undoLastOperation() {
    UI::printHeader("UNDO LAST OPERATION");
    Operation* lastOp = operationLog.pop();
    if (!lastOp) {
        UI::printWarning("No operations to undo.");
        UI::waitForEnter();
        return;
    }
    
    Parcel* parcel = parcelDatabase.search(lastOp->parcelId);
    if (!parcel) {
        UI::printError("Parcel not found for undo.");
        delete lastOp;
        UI::waitForEnter();
        return;
    }
    
    // Reverse based on operation type
    if (lastOp->operationType == "REGISTER") {
        parcelDatabase.remove(lastOp->parcelId);
        UI::printSuccess("Registration undone for PKG" + to_string(lastOp->parcelId));
        delete parcel;
    } else if (lastOp->operationType == "PICKUP") {
        warehouseQueue.enqueue(parcel);
        pickupQueue.enqueue(parcel);
        parcel->status = IN_PICKUP_QUEUE;
        parcel->currentLocation = parcel->origin;
        UI::printSuccess("Pickup undone for PKG" + to_string(lastOp->parcelId));
    } else if (lastOp->operationType == "WAREHOUSE") {
        transitQueue.enqueue(parcel);
        warehouseQueue.enqueue(parcel);
        parcel->status = AT_WAREHOUSE;
        parcel->currentLocation = "Warehouse Hub";
        UI::printSuccess("Warehouse processing undone for PKG" + to_string(lastOp->parcelId));
    } else if (lastOp->operationType == "DELIVER" || lastOp->operationType == "ATTEMPT") {
        parcel->status = IN_TRANSIT;
        parcel->currentLocation = "In Transit";
        parcel->deliveryTime = "";
        if (lastOp->operationType == "ATTEMPT") parcel->deliveryAttempts--;
        UI::printSuccess("Delivery undone for PKG" + to_string(lastOp->parcelId));
    } else if (lastOp->operationType == "MISSING") {
        parcel->status = OUT_FOR_DELIVERY;
        UI::printSuccess("Missing mark undone for PKG" + to_string(lastOp->parcelId));
    } else {
        UI::printWarning("Undo not supported for this operation type.");
    }
    
    delete lastOp;
    UI::waitForEnter();
}

void CourierLogisticsSystem::viewParcelsByDestination() {
    UI::printHeader("PARCELS SORTED BY DESTINATION");
    destinationTree.displaySorted();
    UI::waitForEnter();
}

void CourierLogisticsSystem::displayMainMenu(){ UI::printLogo(); UI::printHeader("SwiftEx Intelligent Courier Logistics System"); UI::printOption(1, "Register New Parcel"); UI::printOption(2, "Track Parcel (View Details & Timeline)"); UI::printOption(3, "View Priority Queue (Sorted Parcels)"); UI::printOption(4, "Process Pickup Queue"); UI::printOption(5, "Process Warehouse Queue"); UI::printOption(6, "Process Transit Queue"); UI::printOption(7, "View All Queues"); UI::printOption(8, "Route Finder"); UI::printOption(9, "Route Management"); UI::printOption(10, "View Operation Log"); UI::printOption(11, "Mark Parcel Missing"); UI::printOption(12, "System Analytics Report"); UI::printOption(13, "Undo Last Operation"); UI::printOption(14, "View Parcels by Destination"); UI::printOption(0, "Exit"); }
