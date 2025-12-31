#ifndef CUSTOM_STRUCTURES_H
#define CUSTOM_STRUCTURES_H

#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
#include <ctime>
#include "../include/UI_Utils.h"

using namespace std;

// Utility function for timestamp
string getCurrentTimestamp() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
    return string(buffer);
}

// Enums for parcel status and priority
enum ParcelStatus {
    IN_PICKUP_QUEUE,
    AT_WAREHOUSE,
    IN_TRANSIT,
    OUT_FOR_DELIVERY,
    DELIVERED,
    MISSING
};

enum Priority {
    NORMAL,
    TWO_DAY,
    OVERNIGHT
};

enum WeightCategory {
    LIGHT,
    MEDIUM,
    HEAVY
};

// ============================================================================
// PARCEL CLASS
// ============================================================================
class Parcel {
public:
    int id;
    string senderName, recipientName, origin, destination, currentLocation;
    Priority priority;
    WeightCategory weightCategory;
    double weight;
    ParcelStatus status;
    string registrationTime, deliveryTime;
    int deliveryAttempts;
    EventLog* eventHistory;

    Parcel(int i, string s, string r, string o, string d, Priority p, WeightCategory wc, double w)
        : id(i), senderName(s), recipientName(r), origin(o), destination(d), currentLocation(o),
          priority(p), weightCategory(wc), weight(w), status(IN_PICKUP_QUEUE),
          registrationTime(getCurrentTimestamp()), deliveryTime(""), deliveryAttempts(0), eventHistory(nullptr) {}

    void addEvent(string location, ParcelStatus newStatus, string description);
    void printDetails();
    void printTimeline();
};

// ============================================================================
// EVENT LOG (LINKED LIST)
// ============================================================================
struct EventLog {
    string timestamp, location, description;
    ParcelStatus status;
    EventLog* next;

    EventLog(string ts, string loc, ParcelStatus st, string desc)
        : timestamp(ts), location(loc), status(st), description(desc), next(nullptr) {}
};

// ============================================================================
// OPERATION LOG (STACK)
// ============================================================================
struct Operation {
    string operationType, timestamp, details;
    int parcelId;
    Operation* next;

    Operation(string type, int id, string det)
        : operationType(type), parcelId(id), timestamp(getCurrentTimestamp()), details(det), next(nullptr) {}
};

class OperationLog {
private:
    Operation* top;

public:
    OperationLog() : top(nullptr) {}
    ~OperationLog() { while (top) { Operation* temp = top; top = top->next; delete temp; } }

    void push(string type, int id, string details) {
        Operation* newOp = new Operation(type, id, details);
        newOp->next = top;
        top = newOp;
    }

    Operation* pop() {
        if (!top) return nullptr;
        Operation* temp = top;
        top = top->next;
        return temp;
    }

    void displayLog() {
        if (!top) { printWarning("No operations logged yet"); return; }
        cout << BOLD << MAGENTA << "\nƒôï OPERATION LOG (Most Recent First):" << RESET << endl;
        printLine(80, '=');
        Operation* temp = top;
        int num = 1;
        while (temp) {
            cout << CYAN << num++ << ". " << RESET;
            cout << YELLOW << temp->operationType << RESET;
            cout << " (PKG" << setfill('0') << setw(6) << temp->parcelId << ") - ";
            cout << temp->details << " [" << temp->timestamp << "]" << endl;
            temp = temp->next;
        }
        cout << GREEN << "Total operations logged: " << (num - 1) << RESET << endl;
    }
};

// ============================================================================
// QUEUE (LINKED LIST)
// ============================================================================
template<typename T>
struct QueueNode {
    T* data;
    QueueNode* next;
    QueueNode(T* d) : data(d), next(nullptr) {}
};

template<typename T>
class Queue {
private:
    QueueNode<T>* front;
    QueueNode<T>* rear;
    int size;

public:
    Queue() : front(nullptr), rear(nullptr), size(0) {}
    ~Queue() { while (front) { QueueNode<T>* temp = front; front = front->next; delete temp; } }

    void enqueue(T* item) {
        QueueNode<T>* newNode = new QueueNode<T>(item);
        if (!rear) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        size++;
    }

    T* dequeue() {
        if (!front) return nullptr;
        QueueNode<T>* temp = front;
        T* item = temp->data;
        front = front->next;
        if (!front) rear = nullptr;
        delete temp;
        size--;
        return item;
    }

    bool isEmpty() { return front == nullptr; }
    int getSize() { return size; }

    void display(string queueName) {
        if (isEmpty()) { printWarning(queueName + " is empty"); return; }
        cout << BOLD << MAGENTA << "\n" << queueName << ":" << RESET << endl;
        printLine(80, '-');
        QueueNode<T>* temp = front;
        int count = 1;
        while (temp) {
            cout << CYAN << count++ << ". " << RESET;
            cout << "PKG" << setfill('0') << setw(6) << temp->data->id << " - ";
            cout << temp->data->senderName << " -> " << temp->data->recipientName << " (";
            cout << temp->data->origin << " -> " << temp->data->destination << ")" << endl;
            temp = temp->next;
        }
        cout << GREEN << "Total in queue: " << size << RESET << endl;
        printLine(80, '-');
    }
};

// ============================================================================
// MIN HEAP (PRIORITY QUEUE)
// ============================================================================
class MinHeap {
private:
    Parcel** heap;
    int capacity;
    int size;

    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    MinHeap(int cap = 1000);
    ~MinHeap();

    void insert(Parcel* parcel);
    Parcel* extractMin();
    bool isEmpty() { return size == 0; }
    int getSize() { return size; }

    void display();
};

// ============================================================================
// GRAPH (ADJACENCY LIST FOR ROUTING)
// ============================================================================
struct Edge {
    string destination;
    int distance;
    bool isBlocked;
    Edge* next;

    Edge(string dest, int dist) : destination(dest), distance(dist), isBlocked(false), next(nullptr) {}
};

struct GraphNode {
    string city;
    Edge* edges;
    GraphNode* next;

    GraphNode(string c) : city(c), edges(nullptr), next(nullptr) {}

    void addEdge(string dest, int dist) {
        Edge* newEdge = new Edge(dest, dist);
        if (!edges) {
            edges = newEdge;
        } else {
            Edge* temp = edges;
            while (temp->next) temp = temp->next;
            temp->next = newEdge;
        }
    }
};

class Graph {
private:
    GraphNode* head;

    GraphNode* findNode(string city);

public:
    Graph() : head(nullptr) {}
    ~Graph();

    void addCity(string city);
    void addRoute(string from, string to, int distance);
    void blockRoute(string from, string to);
    void unblockRoute(string from, string to);

    void findShortestPath(string start, string end);
    void displayNetwork();
};

// ============================================================================
// HASH TABLE (FOR TRACKING)
// ============================================================================
template<typename T>
struct HashNode {
    int key;
    T* value;
    HashNode* next;
    HashNode(int k, T* v) : key(k), value(v), next(nullptr) {}
};

template<typename T>
class HashTable {
private:
    static const int TABLE_SIZE = 1000;
    HashNode<T>* table[TABLE_SIZE];

    int hashFunction(int key) { return key % TABLE_SIZE; }

public:
    HashTable() { for (int i = 0; i < TABLE_SIZE; i++) table[i] = nullptr; }
    ~HashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            HashNode<T>* temp = table[i];
            while (temp) {
                HashNode<T>* toDel = temp;
                temp = temp->next;
                delete toDel->value;
                delete toDel;
            }
        }
    }

    void insert(int key, T* value) {
        int idx = hashFunction(key);
        HashNode<T>* newNode = new HashNode<T>(key, value);
        if (!table[idx]) {
            table[idx] = newNode;
        } else {
            HashNode<T>* temp = table[idx];
            while (temp->next) temp = temp->next;
            temp->next = newNode;
        }
    }

    T* search(int key) {
        int idx = hashFunction(key);
        HashNode<T>* temp = table[idx];
        while (temp) {
            if (temp->key == key) return temp->value;
            temp = temp->next;
        }
        return nullptr;
    }

    bool remove(int key) {
        int idx = hashFunction(key);
        HashNode<T>* temp = table[idx];
        HashNode<T>* prev = nullptr;
        while (temp) {
            if (temp->key == key) {
                if (prev) prev->next = temp->next;
                else table[idx] = temp->next;
                delete temp;
                return true;
            }
            prev = temp;
            temp = temp->next;
        }
        return false;
    }
};

// ============================================================================
// BINARY SEARCH TREE FOR DESTINATION-BASED ORGANIZATION
// ============================================================================
struct BSTNode {
    Parcel* parcel;
    BSTNode* left;
    BSTNode* right;

    BSTNode(Parcel* p) : parcel(p), left(nullptr), right(nullptr) {}
};

class DestinationBST {
private:
    BSTNode* root;

    void insertHelper(BSTNode*& node, Parcel* parcel) {
        if (!node) {
            node = new BSTNode(parcel);
            return;
        }
        if (parcel->destination < node->parcel->destination) {
            insertHelper(node->left, parcel);
        } else {
            insertHelper(node->right, parcel);
        }
    }

    void inOrderTraversal(BSTNode* node) {
        if (!node) return;
        inOrderTraversal(node->left);
        cout << "PKG" << setfill('0') << setw(6) << node->parcel->id
             << " - " << node->parcel->destination << " (" << node->parcel->senderName << " -> " << node->parcel->recipientName << ")" << endl;
        inOrderTraversal(node->right);
    }

    void destroyTree(BSTNode* node) {
        if (!node) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

public:
    DestinationBST() : root(nullptr) {}
    ~DestinationBST() { destroyTree(root); }

    void insert(Parcel* parcel) {
        insertHelper(root, parcel);
    }

    void displaySorted() {
        if (!root) {
            cout << "No parcels in destination tree." << endl;
            return;
        }
        cout << "Parcels sorted by destination:" << endl;
        inOrderTraversal(root);
    }
};

// ============================================================================
// IMPLEMENTATIONS
// ============================================================================

// Parcel methods
void Parcel::addEvent(string location, ParcelStatus newStatus, string description) {
    EventLog* newEvent = new EventLog(getCurrentTimestamp(), location, newStatus, description);
    if (!eventHistory) {
        eventHistory = newEvent;
    } else {
        EventLog* temp = eventHistory;
        while (temp->next) temp = temp->next;
        temp->next = newEvent;
    }
    status = newStatus;
    currentLocation = location;
}

void Parcel::printDetails() {
    cout << BOLD << CYAN << "\nPARCEL DETAILS:" << RESET << endl;
    printLine(80, '=');
    cout << "ID: PKG" << setfill('0') << setw(6) << id << endl;
    cout << "Sender: " << senderName << endl;
    cout << "Recipient: " << recipientName << endl;
    cout << "Origin: " << origin << endl;
    cout << "Destination: " << destination << endl;
    cout << "Current Location: " << currentLocation << endl;
    cout << "Priority: ";
    if (priority == OVERNIGHT) cout << RED << "Overnight" << RESET;
    else if (priority == TWO_DAY) cout << YELLOW << "2-Day" << RESET;
    else cout << GREEN << "Normal" << RESET;
    cout << endl;
    cout << "Weight: " << weight << " kg (" << (weightCategory == LIGHT ? "Light" : weightCategory == MEDIUM ? "Medium" : "Heavy") << ")" << endl;
    cout << "Status: ";
    switch (status) {
        case IN_PICKUP_QUEUE: cout << BLUE << "In Pickup Queue" << RESET; break;
        case AT_WAREHOUSE: cout << YELLOW << "At Warehouse" << RESET; break;
        case IN_TRANSIT: cout << CYAN << "In Transit" << RESET; break;
        case OUT_FOR_DELIVERY: cout << MAGENTA << "Out for Delivery" << RESET; break;
        case DELIVERED: cout << GREEN << "Delivered" << RESET; break;
        case MISSING: cout << RED << "Missing" << RESET; break;
    }
    cout << endl;
    cout << "Registration Time: " << registrationTime << endl;
    if (!deliveryTime.empty()) cout << "Delivery Time: " << deliveryTime << endl;
    cout << "Delivery Attempts: " << deliveryAttempts << endl;
}

void Parcel::printTimeline() {
    cout << endl << BOLD << MAGENTA << "TRACKING TIMELINE:" << RESET << endl;
    printLine(80, '-');
    if (!eventHistory) {
        cout << "No events recorded." << endl;
        return;
    }
    EventLog* temp = eventHistory;
    int count = 1;
    while (temp) {
        cout << CYAN << "  " << count++ << ". " << RESET;
        cout << YELLOW << temp->location << RESET << " - ";
        switch (temp->status) {
            case IN_PICKUP_QUEUE: cout << BLUE << "Added to Pickup Queue" << RESET; break;
            case AT_WAREHOUSE: cout << YELLOW << "Arrived at Warehouse" << RESET; break;
            case IN_TRANSIT: cout << CYAN << "Dispatched for Transit" << RESET; break;
            case OUT_FOR_DELIVERY: cout << MAGENTA << "Out for Delivery" << RESET; break;
            case DELIVERED: cout << GREEN << "Delivered Successfully" << RESET; break;
            case MISSING: cout << RED << "Marked as Missing" << RESET; break;
        }
        cout << " [" << temp->timestamp << "]" << endl;
        cout << "     " << temp->description << endl;
        temp = temp->next;
    }
}

// MinHeap implementations
MinHeap::MinHeap(int cap) : capacity(cap), size(0) {
    heap = new Parcel*[capacity];
}

MinHeap::~MinHeap() {
    delete[] heap;
}

void MinHeap::heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap[index]->priority >= heap[parent]->priority) break;
        swap(heap[index], heap[parent]);
        index = parent;
    }
}

void MinHeap::heapifyDown(int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int smallest = index;

    if (left < size && heap[left]->priority < heap[smallest]->priority) smallest = left;
    if (right < size && heap[right]->priority < heap[smallest]->priority) smallest = right;

    if (smallest != index) {
        swap(heap[index], heap[smallest]);
        heapifyDown(smallest);
    }
}

void MinHeap::insert(Parcel* parcel) {
    if (size == capacity) {
        cout << RED << "Heap is full!" << RESET << endl;
        return;
    }
    heap[size] = parcel;
    heapifyUp(size);
    size++;
}

Parcel* MinHeap::extractMin() {
    if (size == 0) return nullptr;
    Parcel* min = heap[0];
    heap[0] = heap[size - 1];
    size--;
    heapifyDown(0);
    return min;
}

void MinHeap::display() {
    if (isEmpty()) { printWarning("Queue is empty"); return; }
    cout << BOLD << CYAN << "\nPRIORITY QUEUE (Sorted by Priority):" << RESET << endl;
    printLine(80, '=');
    for (int i = 0; i < size; i++) {
        cout << (heap[i]->priority == OVERNIGHT ? RED : heap[i]->priority == TWO_DAY ? YELLOW : GREEN);
        cout << "PKG" << setfill('0') << setw(6) << heap[i]->id << " - ";
        cout << heap[i]->senderName << " -> " << heap[i]->recipientName << " (";
        cout << heap[i]->origin << " -> " << heap[i]->destination << ")" << RESET << endl;
    }
    cout << GREEN << "Total parcels in queue: " << size << RESET << endl;
}

// Graph implementations
GraphNode* Graph::findNode(string city) {
    GraphNode* temp = head;
    while (temp) {
        if (temp->city == city) return temp;
        temp = temp->next;
    }
    return nullptr;
}

Graph::~Graph() {
    while (head) {
        GraphNode* temp = head;
        head = head->next;
        while (temp->edges) {
            Edge* e = temp->edges;
            temp->edges = temp->edges->next;
            delete e;
        }
        delete temp;
    }
}

void Graph::addCity(string city) {
    if (findNode(city)) return;
    GraphNode* newNode = new GraphNode(city);
    if (!head) head = newNode;
    else {
        GraphNode* temp = head;
        while (temp->next) temp = temp->next;
        temp->next = newNode;
    }
}

void Graph::addRoute(string from, string to, int distance) {
    if (!findNode(from)) addCity(from);
    if (!findNode(to)) addCity(to);
    findNode(from)->addEdge(to, distance);
}

void Graph::blockRoute(string from, string to) {
    GraphNode* node = findNode(from);
    if (!node) return;
    Edge* e = node->edges;
    while (e) {
        if (e->destination == to) {
            e->isBlocked = true;
            cout << YELLOW << "Route blocked: " << from << " -> " << to << RESET << endl;
            return;
        }
        e = e->next;
    }
}

void Graph::unblockRoute(string from, string to) {
    GraphNode* node = findNode(from);
    if (!node) return;
    Edge* e = node->edges;
    while (e) {
        if (e->destination == to) {
            e->isBlocked = false;
            cout << GREEN << "Route unblocked: " << from << " -> " << to << RESET << endl;
            return;
        }
        e = e->next;
    }
}

void Graph::findShortestPath(string start, string end) {
    // Dijkstra's algorithm implementation
    const int MAX = 100;
    string cities[MAX];
    int dist[MAX];
    bool visited[MAX];
    string prev[MAX];
    int count = 0;

    GraphNode* temp = head;
    while (temp && count < MAX) {
        cities[count] = temp->city;
        dist[count] = 999999;
        visited[count] = false;
        prev[count] = "";
        count++;
        temp = temp->next;
    }

    int u = -1;
    for (int i = 0; i < count; i++) {
        if (cities[i] == start) {
            u = i;
            dist[i] = 0;
        }
    }

    if (u == -1) { cout << RED << "Start city not found." << RESET << endl; return; }

    for (int i = 0; i < count - 1; i++) {
        int min = 999999, minIdx = -1;
        for (int v = 0; v < count; v++) {
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                minIdx = v;
            }
        }

        if (minIdx == -1) break;
        visited[minIdx] = true;

        GraphNode* curr = findNode(cities[minIdx]);
        Edge* e = curr->edges;
        while (e) {
            if (!e->isBlocked) {
                int neighborIdx = -1;
                for (int k = 0; k < count; k++) if (cities[k] == e->destination) neighborIdx = k;

                if (neighborIdx != -1 && !visited[neighborIdx] && dist[minIdx] != 999999
                    && dist[minIdx] + e->distance < dist[neighborIdx]) {
                    dist[neighborIdx] = dist[minIdx] + e->distance;
                    prev[neighborIdx] = cities[minIdx];
                }
            }
            e = e->next;
        }
    }

    int target = -1;
    for (int i = 0; i < count; i++) if (cities[i] == end) target = i;

    if (target == -1 || dist[target] == 999999) {
        cout << RED << "No path found." << RESET << endl;
    } else {
        cout << GREEN << "Shortest path: " << dist[target] << " km" << RESET << endl;
        string path = end;
        string curr = end;
        while (curr != start) {
            for (int i = 0; i < count; i++) {
                if (cities[i] == curr) {
                    curr = prev[i];
                    path = curr + " -> " + path;
                    break;
                }
            }
        }
        cout << "Route: " << YELLOW << path << RESET << endl;
    }
}

void Graph::displayNetwork() {
    cout << MAGENTA << "Network Map:" << RESET << endl;
    GraphNode* temp = head;
    while (temp) {
        Edge* e = temp->edges;
        while (e) {
            cout << temp->city << " -> " << e->destination << " (" << e->distance << "km)";
            if (e->isBlocked) cout << RED << " [BLOCKED]" << RESET;
            cout << endl;
            e = e->next;
        }
        temp = temp->next;
    }
}

#endif