#include "OperationsModule/Operations.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    srand((unsigned)time(nullptr));
    CourierLogisticsSystem sys;
    int choice = -1;
    while(true) {
        sys.displayMainMenu();
        cout << "\nEnter choice: ";
        if(!(cin >> choice)) { cin.clear(); cin.ignore(10000,'\n'); continue; }
        switch(choice) {
            case 1: sys.registerParcel(); break;
            case 2: sys.viewParcelDetails(); break;
            case 3: sys.viewPriorityQueue(); break;
            case 4: sys.processPickupQueue(); break;
            case 5: sys.processWarehouseQueue(); break;
            case 6: sys.processTransitQueue(); break;
            case 7: sys.viewAllQueues(); break;
            case 8: sys.findRoute(); break;
            case 9: sys.manageRoutes(); break;
            case 10: sys.viewOperationLog(); break;
            case 11: sys.markParcelMissing(); break;
            case 12: sys.generateReport(); break;
            case 13: sys.undoLastOperation(); break;
            case 14: sys.viewParcelsByDestination(); break;
            case 0: cout << "Exiting...\n"; return 0;
            default: cout << "Invalid choice\n"; break;
        }
    }
    return 0;
}
