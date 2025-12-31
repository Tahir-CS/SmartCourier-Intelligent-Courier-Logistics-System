#include "Sorter.h"

void ParcelSorter::heapifyUp(int index) {
    if (index == 0) return;
    int parentIndex = (index - 1) / 2;

    // If current is higher priority than parent, swap
    if (heap[index].priority > heap[parentIndex].priority) {
        swap(heap[index], heap[parentIndex]);
        heapifyUp(parentIndex);
    }
}

void ParcelSorter::heapifyDown(int index) {
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;
    int largest = index;

    if (leftChild < heap.size() && heap[leftChild].priority > heap[largest].priority)
        largest = leftChild;

    if (rightChild < heap.size() && heap[rightChild].priority > heap[largest].priority)
        largest = rightChild;

    if (largest != index) {
        swap(heap[index], heap[largest]);
        heapifyDown(largest);
    }
}

void ParcelSorter::addParcel(Parcel p) {
    heap.push_back(p);
    heapifyUp(heap.size() - 1);
    cout << "[Sorter] Parcel " << p.parcelID << " added to priority queue.\n";
}

Parcel ParcelSorter::getNextParcel() {
    if (heap.empty()) return Parcel(); // Return empty if nothing there

    Parcel top = heap[0];
    heap[0] = heap.back();
    heap.pop_back();
    heapifyDown(0);
    
    return top;
}

bool ParcelSorter::isEmpty() {
    return heap.empty();
}

void ParcelSorter::displayQueue() {
    cout << "\n--- Priority Sorting Queue ---\n";
    for (const auto& p : heap) {
        string pName = (p.priority == 3) ? "Overnight" : (p.priority == 2 ? "Standard" : "Low");
        cout << "ID: " << p.parcelID << " | Dest: " << p.destination << " | Priority: " << pName << endl;
    }
}
