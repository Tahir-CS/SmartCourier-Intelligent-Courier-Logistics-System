#ifndef OPERATIONSMANAGER_H
#define OPERATIONSMANAGER_H

#include "Parcel.h"
#include <string>

// --- Queue Node ---
// Standard linked list node for Queues
struct QueueNode {
    Parcel* data;
    QueueNode* next;
    QueueNode(Parcel* d) : data(d), next(nullptr) {}
};

// --- Custom Queue Class ---
// Implements FIFO (First In First Out) for Processing Stages.
class ParcelQueue {
private:
    QueueNode* front;
    QueueNode* rear;
    int size;
public:
    ParcelQueue();
    ~ParcelQueue();
    void enqueue(Parcel* p); // Add to rear
    Parcel* dequeue();       // Remove from front
    bool remove(int id);     // Special: Remove middle element (for Undo)
    bool isEmpty();
    int getSize();
};

// --- Stack Node ---
// For Undo History
struct StackNode {
    std::string type; // Type of operation (e.g., "DISPATCH")
    int parcelId;
    StackNode* next;
    StackNode(std::string t, int id) : type(t), parcelId(id), next(nullptr) {}
};

// --- Operation Stack Class ---
// Implements LIFO (Last In First Out) for Undo Functionality.
class OperationStack {
private:
    StackNode* top;
public:
    OperationStack();
    ~OperationStack();
    void push(std::string type, int id);
    StackNode* pop();
    bool isEmpty();
};

// --- BST Node ---
// For sorting by Destination string
struct BSTNode {
    Parcel* data;
    BSTNode* left;
    BSTNode* right;
    BSTNode(Parcel* d) : data(d), left(nullptr), right(nullptr) {}
};

// --- Destination Tree Class ---
// Binary Search Tree to organize parcels alphabetically by City.
class DestinationTree {
private:
    BSTNode* root;
    void insertRec(BSTNode*& node, Parcel* p); // Recursive Insert
    void inOrderRec(BSTNode* node);            // Recursive Traversal
public:
    DestinationTree();
    void insert(Parcel* p);
    void display(); // Prints in alphabetical order
};

#endif