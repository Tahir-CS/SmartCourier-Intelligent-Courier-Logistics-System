#include "Operations.h"

void Operations::addParcelToQueue(Parcel p) {
    processingQueue.push(p);
    cout << "[Ops] Parcel " << p.parcelID << " added to processing queue.\n";
}

void Operations::processNextParcel() {
    if (processingQueue.empty()) {
        cout << "[Ops] No parcels to process.\n";
        return;
    }
    Parcel p = processingQueue.front();
    processingQueue.pop();
    cout << "[Ops] Processing Parcel " << p.parcelID << " for destination: " << p.destination << endl;
}

void Operations::showQueueStatus() {
    cout << "[Ops] Parcels in Processing Queue: " << processingQueue.size() << endl;
}