#ifndef SORTINGMANAGER_H
#define SORTINGMANAGER_H

#include "Parcel.h"

// --- Custom Min-Heap Class ---
// A Min-Heap is the perfect data structure for a Priority Queue.
// It allows us to retrieve the highest priority (smallest number) item in O(1) time.
class SortingManager {
private:
    Parcel** heap; // Dynamic array of pointers to Parcel objects
    int capacity;  // Max size of heap
    int size;      // Current number of elements

    // Heap Helper Functions (Navigation)
    int parent(int i);
    int leftChild(int i);
    int rightChild(int i);
    
    // Core Logic Helpers
    void swap(Parcel** a, Parcel** b);
    void heapifyUp(int i);    // Moves an element UP the tree (Insertion)
    void heapifyDown(int i);  // Moves an element DOWN the tree (Extraction)
    bool compareParcels(Parcel* a, Parcel* b); // Determines which parcel has higher priority

public:
    SortingManager(int cap = 1000);
    ~SortingManager();

    void addParcel(Parcel* parcel); // Insert new parcel (O(log n))
    Parcel* extractHighPriority();  // Remove & return most urgent parcel (O(log n))
    Parcel* peek();                 // View most urgent without removing (O(1))
    bool isEmpty();
    void displayQueue();            // Visualizes the queue for the UI
};

#endif