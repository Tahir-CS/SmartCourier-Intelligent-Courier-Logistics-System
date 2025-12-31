#include <iostream>
#include <limits>
#include <ctime>
#include "GlobalConstants.h"
#include "Parcel.h"
#include "SortingManager.h"
#include "RoutingManager.h"
#include "TrackingManager.h"
#include "OperationsManager.h"

using namespace std;

// --- Helper for Pause ---
void waitForEnter() {
    cout << "\n" << CYAN << "Press Enter to continue..." << RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// --- Main System Class (Controller) ---
// This class integrates all the Data Structures and provides the UI logic.
class CourierSystem {
private:
    // Core Modules
    TrackingManager tracker;   // Module 3: Storage (Hash)
    SortingManager sorter;     // Module 1: Priority (Heap)
    RoutingManager router;     // Module 2: Network (Graph)
    DestinationTree destTree;  // Module 4: Organization (BST)
    
    // Operation Queues
    ParcelQueue pickupQ;
    ParcelQueue warehouseQ;
    ParcelQueue transitQ;
    OperationStack undoStack;  // For Undo Feature

    int nextId; // Auto-incrementing ID

public:
    CourierSystem() : nextId(1001) {}

    // Option 1: Register
    void registerParcel() {
        clearScreen();
        cout << BOLD << BLUE << "=== REGISTER NEW PARCEL ===" << RESET << endl;
        string sender, dest;
        int pOpt, wOpt;

        cout << "Sender Name: "; cin.ignore(); getline(cin, sender);
        cout << "Destination: "; getline(cin, dest);
        cout << "Priority (1:Overnight, 2:2-Day, 3:Normal): "; cin >> pOpt;
        cout << "Weight (1:Light, 2:Med, 3:Heavy): "; cin >> wOpt;

        // Create Object
        Parcel* p = new Parcel(nextId, sender, "TBD", "Lahore", dest, 
                               (Priority)pOpt, (WeightCategory)wOpt, 5.0, "2023-12-30");
        
        // Add to ALL Data Structures
        tracker.registerParcel(nextId, p);
        sorter.addParcel(p);
        pickupQ.enqueue(p);
        destTree.insert(p); 
        undoStack.push("REGISTER", nextId);

        cout << GREEN << "\nSuccess! Tracking ID: PKG" << nextId << RESET << endl;
        nextId++;
    }

    // Option 2: Move from Pickup -> Warehouse
    void processPickup() {
        if(pickupQ.isEmpty()) { cout << RED << "  No parcels to pickup." << RESET << endl; return; }
        
        Parcel* p = pickupQ.dequeue();
        p->addEvent("Now", "Warehouse", AT_WAREHOUSE, "Arrived at Hub");
        warehouseQ.enqueue(p);
        undoStack.push("PICKUP", p->id);
        
        cout << GREEN << "  PKG" << p->id << " moved to Warehouse." << RESET << endl;
    }

    // Option 3: Move from Warehouse -> Transit
    void processWarehouse() {
        if(warehouseQ.isEmpty()) { cout << RED << "  Warehouse empty." << RESET << endl; return; }
        
        Parcel* p = warehouseQ.dequeue();
        p->addEvent("Now", "Transit", IN_TRANSIT, "Dispatched from Hub");
        transitQ.enqueue(p);
        undoStack.push("DISPATCH", p->id);
        
        cout << GREEN << "  PKG" << p->id << " dispatched." << RESET << endl;
    }

    // Option 9: Undo Logic
    void undoLast() {
        if(undoStack.isEmpty()) { cout << RED << "  Nothing to undo." << RESET << endl; return; }
        
        StackNode* op = undoStack.pop();
        Parcel* p = tracker.getParcel(op->parcelId);
        
        cout << YELLOW << "  Undoing " << op->type << " on PKG" << op->parcelId << "..." << RESET << endl;
        
        // Reverse Logic
        if(op->type == "DISPATCH") {
            // Reverse Dispatch: Remove from Transit, add back to Warehouse
            if(transitQ.remove(p->id)) {
                warehouseQ.enqueue(p);
                p->status = AT_WAREHOUSE;
                p->addEvent("Now", "Warehouse", AT_WAREHOUSE, "UNDO: Returned to Warehouse");
                cout << GREEN << "  Reverted to Warehouse." << RESET << endl;
            }
        } else if(op->type == "PICKUP") {
            // Reverse Pickup: Remove from Warehouse, add back to Pickup
            if(warehouseQ.remove(p->id)) {
                pickupQ.enqueue(p);
                p->status = IN_PICKUP_QUEUE;
                p->addEvent("Now", "Pickup", IN_PICKUP_QUEUE, "UNDO: Returned to Pickup Queue");
                cout << GREEN << "  Reverted to Pickup Queue." << RESET << endl;
            }
        }
        delete op;
    }

    // Option 4: Tracking Logic
    void track() {
        int id; 
        cout << "Enter ID (Number only, e.g. 1001): "; 
        cin >> id;
        
        Parcel* p = tracker.getParcel(id); // O(1) Search
        if(!p) { cout << RED << "  Not Found." << RESET << endl; return; }
        
        p->printTicket(); 
        
        cout << MAGENTA << "\n  === Tracking Timeline ===" << RESET << endl;
        EventLog* e = p->eventHistory;
        int step = 1;
        while(e) {
            cout << "  " << step++ << ". [" << e->timestamp << "] " << CYAN << e->location << RESET << endl;
            cout << "     " << e->description << " (" << statusToString(e->status) << ")" << endl;
            if(e->next) cout << "      |" << endl << "      v" << endl;
            e = e->next;
        }
    }

    void viewBST() { destTree.display(); }
    void viewHeap() { sorter.displayQueue(); }
    void findRoute() {
        string to; cout << "Enter City: "; cin.ignore(); getline(cin, to);
        router.findShortestPath("Lahore", to);
    }
    void viewMap() { router.displayNetwork(); }
};

int main() {
    CourierSystem sys;
    int choice;
    while(true) {
        clearScreen();
        // Clean ASCII UI
        cout << BOLD << BG_BLUE << WHITE << "     SWIFTEX LOGISTICS ENGINE     " << RESET << endl;
        cout << CYAN << " 1. Register Parcel" << endl;
        cout << " 2. Process Pickup (Queue -> Warehouse)" << endl;
        cout << " 3. Process Dispatch (Warehouse -> Transit)" << endl;
        cout << " 4. Track Parcel (Hash Table)" << endl;
        cout << " 5. View Priority Queue (Min-Heap)" << endl;
        cout << " 6. View Parcels by Destination (BST)" << endl;
        cout << " 7. Find Route (Dijkstra)" << endl;
        cout << " 8. View Network Map" << endl;
        cout << " 9. Undo Last Action (Stack)" << endl;
        cout << " 0. Exit" << RESET << endl;
        cout << "\n Select >> ";
        cin >> choice;

        if(choice == 0) break;
        switch(choice) {
            case 1: sys.registerParcel(); break;
            case 2: sys.processPickup(); break;
            case 3: sys.processWarehouse(); break;
            case 4: sys.track(); break;
            case 5: sys.viewHeap(); break;
            case 6: sys.viewBST(); break;
            case 7: sys.findRoute(); break;
            case 8: sys.viewMap(); break;
            case 9: sys.undoLast(); break;
        }
        waitForEnter();
    }
    return 0;
}
