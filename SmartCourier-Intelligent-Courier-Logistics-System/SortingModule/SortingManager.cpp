#include "SortingManager.h"
#include <iostream>
#include <iomanip>

using namespace std;

// Constructor: Allocate memory for the heap array
SortingManager::SortingManager(int cap) : capacity(cap), size(0) {
    heap = new Parcel*[capacity];
}

// Destructor: Clean up the array (but not the parcels, as they are managed by the Hash Table)
SortingManager::~SortingManager() {
    delete[] heap;
}

// --- Navigation Helpers ---
int SortingManager::parent(int i) { return (i - 1) / 2; }
int SortingManager::leftChild(int i) { return 2 * i + 1; }
int SortingManager::rightChild(int i) { return 2 * i + 2; }

void SortingManager::swap(Parcel** a, Parcel** b) {
    Parcel* temp = *a;
    *a = *b;
    *b = temp;
}

// --- Comparison Logic ---
// Returns TRUE if Parcel A is "more urgent" than Parcel B.
bool SortingManager::compareParcels(Parcel* a, Parcel* b) {
    // Rule 1: Lower Priority number = Higher Urgency (1 > 2 > 3)
    if(a->priority != b->priority) {
        return a->priority < b->priority;
    }
    // Rule 2: Tie-Breaker - Lighter parcels go first (Optimization)
    return a->weightKg < b->weightKg;
}

// --- Heapify Up (For Insertion) ---
// Checks if current node is smaller than parent. If so, swap and repeat.
void SortingManager::heapifyUp(int i) {
    while(i > 0 && compareParcels(heap[i], heap[parent(i)])) {
        swap(&heap[i], &heap[parent(i)]);
        i = parent(i);
    }
}

// --- Heapify Down (For Extraction) ---
// Checks if current node is larger than children. If so, swap with smallest child.
void SortingManager::heapifyDown(int i) {
    int minIndex = i;
    int left = leftChild(i);
    int right = rightChild(i);

    // Check if Left Child is smaller (higher priority)
    if(left < size && compareParcels(heap[left], heap[minIndex])) minIndex = left;
    // Check if Right Child is smaller
    if(right < size && compareParcels(heap[right], heap[minIndex])) minIndex = right;

    // If a child is smaller, swap and recurse
    if(i != minIndex) {
        swap(&heap[i], &heap[minIndex]);
        heapifyDown(minIndex);
    }
}

// --- Add Parcel ---
// Adds to bottom of tree, then bubbles up.
void SortingManager::addParcel(Parcel* parcel) {
    if(size == capacity) return; // Heap Full
    heap[size] = parcel;
    heapifyUp(size);
    size++;
}

// --- Extract Min ---
// Removes the root (highest priority), replaces with last element, then bubbles down.
Parcel* SortingManager::extractHighPriority() {
    if(size == 0) return nullptr;
    Parcel* min = heap[0];
    heap[0] = heap[size - 1]; // Move last element to root
    size--;
    heapifyDown(0); // Fix heap property
    return min;
}

bool SortingManager::isEmpty() { return size == 0; }

// --- Display UI ---
void SortingManager::displayQueue() {
    if(isEmpty()) {
        cout << YELLOW << "  [Queue Empty]" << RESET << endl;
        return;
    }
    cout << CYAN << "  Total Parcels: " << size << RESET << endl;
    cout << "  ---------------------------------------------------------" << endl;
    cout << BOLD << "  ID      | Priority    | Weight | Route" << RESET << endl;
    cout << "  ---------------------------------------------------------" << endl;
    for(int i = 0; i < size; i++) {
        cout << "  PKG" << setfill('0') << setw(4) << heap[i]->id << setfill(' ') << " | ";
        string col = (heap[i]->priority == OVERNIGHT) ? RED : GREEN;
        cout << col << setw(11) << left << priorityToString(heap[i]->priority) << RESET << " | ";
        cout << setw(6) << left << heap[i]->weightKg << " | ";
        cout << heap[i]->origin << " -> " << heap[i]->destination << endl;
    }
    cout << "  ---------------------------------------------------------" << endl;
}