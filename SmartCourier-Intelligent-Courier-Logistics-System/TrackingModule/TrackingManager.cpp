#include "TrackingManager.h"

// Constructor: Initialize buckets to null
TrackingManager::TrackingManager() {
    for(int i=0; i<TABLE_SIZE; i++) table[i] = nullptr;
}

// Destructor: Delete all nodes AND the Parcel objects themselves.
// This is the "Owner" of the Parcel memory.
TrackingManager::~TrackingManager() {
    for(int i=0; i<TABLE_SIZE; i++) {
        HashNode<Parcel>* temp = table[i];
        while(temp) {
            HashNode<Parcel>* del = temp;
            temp = temp->next;
            delete del->value; // Delete Parcel object
            delete del;        // Delete Node
        }
    }
}

// Insert a parcel using Chaining
void TrackingManager::registerParcel(int id, Parcel* parcel) {
    int idx = hashFunction(id);
    HashNode<Parcel>* newNode = new HashNode<Parcel>(id, parcel);
    
    // If bucket is empty, insert directly
    if(!table[idx]) table[idx] = newNode;
    else {
        // Collision: Insert at head (O(1))
        newNode->next = table[idx];
        table[idx] = newNode;
    }
}

// Search for a parcel
Parcel* TrackingManager::getParcel(int id) {
    int idx = hashFunction(id);
    HashNode<Parcel>* temp = table[idx];
    
    // Traverse the linked list at this bucket
    while(temp) {
        if(temp->key == id) return temp->value;
        temp = temp->next;
    }
    return nullptr; // Not found
}