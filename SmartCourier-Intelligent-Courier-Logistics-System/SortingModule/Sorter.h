#ifndef SORTER_H
#define SORTER_H

#include "../SharedData.h"
#include <vector>

class ParcelSorter {
private:
    // We use a vector to represent the Heap Tree (Standard DSA approach)
    vector<Parcel> heap;

    // Helper: Moves an item UP the tree to fix priority
    void heapifyUp(int index);

    // Helper: Moves an item DOWN the tree to fix priority
    void heapifyDown(int index);

public:
    void addParcel(Parcel p);
    Parcel getNextParcel(); // Removes and returns highest priority
    bool isEmpty();
    void displayQueue();
};

#endif
