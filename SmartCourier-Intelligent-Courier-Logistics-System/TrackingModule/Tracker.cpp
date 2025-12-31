#include "Tracker.h"

ParcelTracker::ParcelTracker() {
    head = nullptr;
}

void ParcelTracker::addUpdate(string location, string time) {
    HistoryNode* newNode = new HistoryNode;
    newNode->location = location;
    newNode->timestamp = time;
    newNode->next = head; // Insert at front (Newest first)
    head = newNode;
}

void ParcelTracker::showHistory(int parcelID) {
    cout << "\n--- Tracking History for Parcel " << parcelID << " ---\n";
    HistoryNode* current = head;
    while (current != nullptr) {
        cout << "[" << current->timestamp << "] " << current->location << endl;
        current = current->next;
    }
    if (head == nullptr) cout << "No history available.\n";
}
