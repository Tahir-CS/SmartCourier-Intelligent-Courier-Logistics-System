// File: main.cpp
#include <iostream>
#include "SharedData.h"
#include "SortingModule/Sorter.h"
#include "RoutingModule/Routing.h"
#include "TrackingModule/Tracker.h"
#include "OperationsModule/Operations.h"

using namespace std;

int main() {
    // Instantiate Modules
    ParcelSorter sorter;
    RoutingEngine router;
    ParcelTracker tracker;
    Operations ops;

    int choice;
    while (true) {
        cout << "\n=== SwiftEx Logistics Engine ===\n";
        cout << "1. Add New Parcel (Sorting Module)\n";
        cout << "2. View Priority Queue\n";
        cout << "3. Process Next Parcel (Operations)\n";
        cout << "4. Calculate Route (Routing Module)\n";
        cout << "5. Track Parcel (Tracking Module)\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            int id, prio;
            string dest;
            cout << "Enter Parcel ID: "; cin >> id;
            cout << "Enter Destination (Lahore, Karachi, etc.): "; cin >> dest;
            cout << "Enter Priority (3=High, 2=Med, 1=Low): "; cin >> prio;
            
            Parcel p(id, dest, prio);
            sorter.addParcel(p);
            tracker.addUpdate("Parcel Received at Hub", "09:00 AM");
        }
        else if (choice == 2) {
            sorter.displayQueue();
        }
        else if (choice == 3) {
            if (!sorter.isEmpty()) {
                Parcel p = sorter.getNextParcel();
                ops.addParcelToQueue(p);
                ops.processNextParcel();
                tracker.addUpdate("Parcel Processed", "10:30 AM");
            } else {
                cout << "No parcels in Sorting Queue!\n";
            }
        }
        else if (choice == 4) {
            string start, end;
            cout << "Enter Start City: "; cin >> start;
            cout << "Enter End City: "; cin >> end;
            router.findShortestPath(start, end);
        }
        else if (choice == 5) {
            int id;
            cout << "Enter Parcel ID to track: "; cin >> id;
            tracker.showHistory(id);
        }
        else if (choice == 6) {
            break;
        }
        else {
            cout << "Invalid Choice!\n";
        }
    }
    return 0;
}
