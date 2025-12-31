#include "Tracker.h"
#include <iostream>

using namespace std;

Tracker::Tracker() {
    // Constructor
}

void Tracker::updateParcelStatus(Parcel* parcel, ParcelStatus newStatus, string location, string description) {
    if (!parcel) {
        UI::printError("Parcel not found!");
        return;
    }
    parcel->status = newStatus;
    parcel->currentLocation = location;
    parcel->addEvent(location, newStatus, description);
    UI::printSuccess("Parcel status updated to " + to_string(newStatus) + " at " + location);
}

ParcelStatus Tracker::getParcelStatus(int parcelId, HashTable<Parcel>& parcelDatabase) {
    Parcel* parcel = parcelDatabase.search(parcelId);
    if (!parcel) {
        UI::printError("Parcel not found!");
        return MISSING; // or some default
    }
    return parcel->status;
}

void Tracker::printParcelHistory(int parcelId, HashTable<Parcel>& parcelDatabase) {
    Parcel* parcel = parcelDatabase.search(parcelId);
    if (!parcel) {
        UI::printError("Parcel not found!");
        return;
    }
    UI::printHeader("PARCEL TIMELINE - PKG" + to_string(parcelId));
    parcel->printTimeline();
}

void Tracker::simulateParcelMovement(Parcel* parcel) {
    // Simulate a basic movement sequence
    if (!parcel) return;

    // Example: from pickup to warehouse
    if (parcel->status == IN_PICKUP_QUEUE) {
        updateParcelStatus(parcel, AT_WAREHOUSE, parcel->origin, "Picked up and moved to warehouse");
    } else if (parcel->status == AT_WAREHOUSE) {
        updateParcelStatus(parcel, IN_TRANSIT, "Transit Hub", "Loaded for transit");
    } else if (parcel->status == IN_TRANSIT) {
        updateParcelStatus(parcel, OUT_FOR_DELIVERY, parcel->destination, "Arrived at destination for delivery");
    } else if (parcel->status == OUT_FOR_DELIVERY) {
        updateParcelStatus(parcel, DELIVERED, parcel->destination, "Successfully delivered");
        parcel->deliveryTime = getCurrentTimestamp();
    }
}

void Tracker::viewAllParcelStatuses(HashTable<Parcel>& parcelDatabase) {
    UI::printHeader("ALL PARCEL STATUSES");
    // Assuming HashTable has a method to iterate, but since it's custom, perhaps print all
    // For simplicity, since we don't have iteration, maybe note that it's not implemented fully
    // But in a real system, HashTable would need an iterator or print method.
    // For now, print a message
    cout << "Feature to view all statuses requires HashTable iteration implementation." << endl;
    UI::waitForEnter();
}