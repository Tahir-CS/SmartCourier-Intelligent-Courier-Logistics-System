#ifndef TRACKINGMANAGER_H
#define TRACKINGMANAGER_H
#include "Parcel.h"

// --- Hash Node Structure ---
// Used for Chaining collision resolution in the Hash Table.
template<typename T>
struct HashNode {
    int key;       // Tracking ID
    T* value;      // Pointer to Parcel
    HashNode* next;// Next node in case of collision
    HashNode(int k, T* v) : key(k), value(v), next(nullptr) {}
};

// --- Hash Table Class ---
// Provides O(1) average time complexity for searching parcels.
class TrackingManager {
private:
    static const int TABLE_SIZE = 1000;
    HashNode<Parcel>* table[TABLE_SIZE]; // Array of pointers (buckets)
    
    // Hash Function: Maps an ID to an index
    int hashFunction(int key) { return key % TABLE_SIZE; }

public:
    TrackingManager();
    ~TrackingManager();
    void registerParcel(int id, Parcel* parcel); // Insert
    Parcel* getParcel(int id); // Search
};
#endif