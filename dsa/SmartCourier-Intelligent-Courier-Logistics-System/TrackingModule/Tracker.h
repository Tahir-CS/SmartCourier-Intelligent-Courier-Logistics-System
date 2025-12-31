#ifndef TRACKER_H
#define TRACKER_H

#include "../include/CustomStructures.h"
#include "../include/UI_Utils.h"
#include <string>

using namespace std;

class Tracker {
private:
    // Perhaps a reference to the parcel database, but since it's in Operations, maybe pass parcels.

public:
    Tracker();

    // Update parcel status and add event
    void updateParcelStatus(Parcel* parcel, ParcelStatus newStatus, string location, string description);

    // Get current status of a parcel
    ParcelStatus getParcelStatus(int parcelId, HashTable<Parcel>& parcelDatabase);

    // Print parcel history/timeline
    void printParcelHistory(int parcelId, HashTable<Parcel>& parcelDatabase);

    // Simulate parcel movement (e.g., dispatch, transit, delivery)
    void simulateParcelMovement(Parcel* parcel);

    // View all parcels with their statuses
    void viewAllParcelStatuses(HashTable<Parcel>& parcelDatabase);
};

#endif // TRACKER_H