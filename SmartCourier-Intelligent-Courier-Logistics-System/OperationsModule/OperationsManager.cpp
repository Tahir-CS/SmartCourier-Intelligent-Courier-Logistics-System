#include "OperationsManager.h"
#include <iostream>
#include <iomanip>
using namespace std;

// ========================= QUEUE IMPLEMENTATION =========================
ParcelQueue::ParcelQueue() : front(nullptr), rear(nullptr), size(0) {}
ParcelQueue::~ParcelQueue() { while(front) { QueueNode* t = front; front = front->next; delete t; } }

void ParcelQueue::enqueue(Parcel* p) {
    QueueNode* n = new QueueNode(p);
    if(!rear) front = rear = n;
    else { rear->next = n; rear = n; }
    size++;
}

Parcel* ParcelQueue::dequeue() {
    if(!front) return nullptr;
    QueueNode* t = front;
    Parcel* d = front->data;
    front = front->next;
    if(!front) rear = nullptr;
    delete t;
    size--;
    return d;
}

// Special Function: Remove specific ID from anywhere in Queue (Needed for Undo)
bool ParcelQueue::remove(int id) {
    if(!front) return false;
    
    // Case 1: Head matches
    if(front->data->id == id) { dequeue(); return true; }
    
    // Case 2: Search rest of list
    QueueNode* curr = front;
    while(curr->next) {
        if(curr->next->data->id == id) {
            QueueNode* temp = curr->next;
            curr->next = temp->next;
            if(!curr->next) rear = curr; // Update rear if last item removed
            delete temp;
            size--;
            return true;
        }
        curr = curr->next;
    }
    return false;
}

bool ParcelQueue::isEmpty() { return size == 0; }
int ParcelQueue::getSize() { return size; }

// ========================= STACK IMPLEMENTATION =========================
OperationStack::OperationStack() : top(nullptr) {}
OperationStack::~OperationStack() { while(top) { StackNode* t = top; top = top->next; delete t; } }

void OperationStack::push(string type, int id) {
    StackNode* n = new StackNode(type, id);
    n->next = top; top = n;
}

StackNode* OperationStack::pop() {
    if(!top) return nullptr;
    StackNode* t = top;
    top = top->next;
    return t;
}

bool OperationStack::isEmpty() { return top == nullptr; }

// ========================= BST IMPLEMENTATION =========================
DestinationTree::DestinationTree() : root(nullptr) {}

void DestinationTree::insert(Parcel* p) { insertRec(root, p); }

void DestinationTree::insertRec(BSTNode*& node, Parcel* p) {
    if(!node) { node = new BSTNode(p); return; }
    if(p->destination < node->data->destination) insertRec(node->left, p);
    else insertRec(node->right, p);
}

void DestinationTree::display() {
    if(!root) { cout << YELLOW << "  [Tree Empty]" << RESET << endl; return; }
    inOrderRec(root);
}

// In-Order Traversal: Visits Left -> Root -> Right (Results in alphabetical order)
void DestinationTree::inOrderRec(BSTNode* node) {
    if(!node) return;
    inOrderRec(node->left);
    cout << "  " << CYAN << setw(12) << left << node->data->destination << RESET 
         << " : PKG" << node->data->id << " (" << statusToString(node->data->status) << ")" << endl;
    inOrderRec(node->right);
}