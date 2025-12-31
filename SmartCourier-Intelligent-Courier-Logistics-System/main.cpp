// File: main.cpp
#include <iostream>
#include "include/UI_Utils.h"
#include "SharedData.h"
#include "SortingModule/Sorter.h"
#include "RoutingModule/Routing.h"
#include "TrackingModule/Tracker.h"
#include "OperationsModule/Operations.h"

using namespace std;

int main() {
    // Enable Colors for Windows CMD
#ifdef _WIN32
    system("color");
#endif

    // Instantiate Modules
    ParcelSorter sorter;
    RoutingEngine router;
    ParcelTracker tracker;
    Operations ops;

    int choice;
    while (true) {
        UI::clearScreen();
        UI::printLogo();
        UI::printLine();
        cout << "   STATUS: " << UI::green() << "SYSTEM ONLINE" << UI::reset()
             << "  |  PENDING: " << UI::yellow() << "0" << UI::reset() << endl;
        UI::printLine();
        cout << endl;

        UI::printOption(1, "Add New Parcel");
        UI::printOption(2, "View Priority Queue");
        UI::printOption(3, "Process Next Parcel");
        UI::printOption(4, "Calculate Route");
        UI::printOption(5, "Track Parcel");
        UI::printOption(6, "Exit Application");

        cout << endl;
        UI::printLine();
        cout << "   " << UI::cyan() << ">>> Select Option: " << UI::reset();

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        if (choice == 1) {
            UI::clearScreen();
            UI::printLine();
            cout << "   REGISTER NEW PARCEL\n";
            UI::printLine();

            int id, prio;
            string dest;
            cout << "\n   Enter Parcel ID: "; cin >> id;
            cout << "   Enter Destination: "; cin >> dest;
            cout << "   Enter Priority (3=High, 2=Med, 1=Low): "; cin >> prio;

            Parcel p(id, dest, prio);
            sorter.addParcel(p);
            tracker.addUpdate("Parcel Received at Hub", "09:00 AM");
            UI::showLoading("   Validating Data");
            cout << "\n   " << UI::green() << "Success! Parcel added to Sorting Module." << UI::reset() << endl;
            UI::pause();
        }
        else if (choice == 2) {
            UI::clearScreen();
            UI::printLine();
            cout << "   PRIORITY QUEUE\n";
            UI::printLine();
            sorter.displayQueue();
            UI::pause();
        }
        else if (choice == 3) {
            UI::clearScreen();
            UI::printLine();
            cout << "   PROCESSING QUEUE\n";
            UI::printLine();
            if (!sorter.isEmpty()) {
                Parcel p = sorter.getNextParcel();
                ops.addParcelToQueue(p);
                ops.processNextParcel();
                tracker.addUpdate("Parcel Processed", "10:30 AM");
            } else {
                cout << "No parcels in Sorting Queue!\n";
            }
            UI::pause();
        }
        else if (choice == 4) {
            UI::clearScreen();
            UI::printLine();
            cout << "   ROUTE OPTIMIZATION (DIJKSTRA)\n";
            UI::printLine();
            string start, end;
            cout << "\n   Enter Start City: "; cin >> start;
            cout << "   Enter End City: "; cin >> end;
            UI::showLoading("   Calculating Paths");
            router.findShortestPath(start, end);
            UI::pause();
        }
        else if (choice == 5) {
            UI::clearScreen();
            UI::printLine();
            cout << "   TRACK PARCEL\n";
            UI::printLine();
            int id;
            cout << "\n   Enter Parcel ID to track: "; cin >> id;
            tracker.showHistory(id);
            UI::pause();
        }
        else if (choice == 6) {
            cout << "\n   Shutting down Smart Courier..." << endl;
            break;
        }
        else {
            cout << "Invalid Choice!\n";
            UI::pause();
        }
    }
    return 0;
}
