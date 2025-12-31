#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <limits>
#include <cmath>

using namespace std;

// ============================================================================
// ANSI COLOR CODES (If these look weird too, you can remove them)
// ============================================================================
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

// ============================================================================
// UTILITY FUNCTIONS FOR UI
// ============================================================================
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void printLine(int length = 80, char c = '=') {
    cout << CYAN;
    for(int i = 0; i < length; i++) cout << c;
    cout << RESET << endl;
}

void printHeader(const string& title) {
    clearScreen();
    printLine(80, '=');
    cout << BOLD << CYAN << "| " << RESET;
    int padding = (76 - title.length()) / 2;
    for(int i = 0; i < padding; i++) cout << " ";
    cout << BOLD << YELLOW << title << RESET;
    for(int i = 0; i < 76 - padding - title.length(); i++) cout << " ";
    cout << BOLD << CYAN << " |" << RESET << endl;
    printLine(80, '=');
    cout << endl;
}

void printSuccess(const string& msg) {
    cout << GREEN << "[OK] " << msg << RESET << endl;
}

void printError(const string& msg) {
    cout << RED << "[ERR] " << msg << RESET << endl;
}

void printInfo(const string& msg) {
    cout << BLUE << "[i] " << msg << RESET << endl;
}

void printWarning(const string& msg) {
    cout << YELLOW << "[!] " << msg << RESET << endl;
}

void waitForEnter() {
    cout << CYAN << "\nPress Enter to continue..." << RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// ============================================================================
// TIMESTAMP UTILITY
// ============================================================================
string getCurrentTimestamp() {
    time_t now = time(0);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return string(buf);
}

// ============================================================================
// ENUMERATIONS
// ============================================================================
enum Priority { OVERNIGHT = 1, TWO_DAY = 2, NORMAL = 3 };
enum WeightCategory { LIGHT = 1, MEDIUM = 2, HEAVY = 3 };
enum ParcelStatus { 
    REGISTERED, 
    IN_PICKUP_QUEUE, 
    PICKED_UP, 
    AT_WAREHOUSE, 
    IN_TRANSIT, 
    OUT_FOR_DELIVERY, 
    DELIVERED, 
    RETURNED,
    MISSING 
};

string priorityToString(Priority p) {
    switch(p) {
        case OVERNIGHT: return "Overnight";
        case TWO_DAY: return "2-Day";
        case NORMAL: return "Normal";
        default: return "Unknown";
    }
}

string weightToString(WeightCategory w) {
    switch(w) {
        case LIGHT: return "Light (<5kg)";
        case MEDIUM: return "Medium (5-20kg)";
        case HEAVY: return "Heavy (>20kg)";
        default: return "Unknown";
    }
}

string statusToString(ParcelStatus s) {
    switch(s) {
        case REGISTERED: return "Registered";
        case IN_PICKUP_QUEUE: return "In Pickup Queue";
        case PICKED_UP: return "Picked Up";
        case AT_WAREHOUSE: return "At Warehouse";
        case IN_TRANSIT: return "In Transit";
        case OUT_FOR_DELIVERY: return "Out for Delivery";
        case DELIVERED: return "Delivered";
        case RETURNED: return "Returned to Sender";
        case MISSING: return "Missing";
        default: return "Unknown";
    }
}

// ============================================================================
// EVENT LOG STRUCTURE
// ============================================================================
struct EventLog {
    string timestamp;
    string location;
    ParcelStatus status;
    string description;
    EventLog* next;
    
    EventLog(string ts, string loc, ParcelStatus st, string desc) 
        : timestamp(ts), location(loc), status(st), description(desc), next(nullptr) {}
};

// ============================================================================
// PARCEL STRUCTURE
// ============================================================================
struct Parcel {
    int id;
    string senderName;
    string recipientName;
    string origin;
    string destination;
    Priority priority;
    WeightCategory weight;
    double weightKg;
    ParcelStatus status;
    string currentLocation;
    string registrationTime;
    string deliveryTime;
    int deliveryAttempts;
    EventLog* eventHistory;
    
    Parcel(int _id, string sender, string recipient, string orig, string dest, 
           Priority pri, WeightCategory wt, double wkg)
        : id(_id), senderName(sender), recipientName(recipient), 
          origin(orig), destination(dest), priority(pri), weight(wt), 
          weightKg(wkg), status(REGISTERED), currentLocation(orig),
          registrationTime(getCurrentTimestamp()), deliveryTime(""), 
          deliveryAttempts(0), eventHistory(nullptr) {
        addEvent(orig, REGISTERED, "Parcel registered in system");
    }
    
    void addEvent(string location, ParcelStatus newStatus, string description) {
        EventLog* newEvent = new EventLog(getCurrentTimestamp(), location, newStatus, description);
        if(!eventHistory) {
            eventHistory = newEvent;
        } else {
            EventLog* temp = eventHistory;
            while(temp->next) temp = temp->next;
            temp->next = newEvent;
        }
        status = newStatus;
        currentLocation = location;
    }
    
    void printDetails() {
        cout << BOLD << CYAN << "+==================================================================+" << RESET << endl;
        cout << BOLD << CYAN << "| " << YELLOW << "                        PARCEL DETAILS                         " << CYAN << "|" << RESET << endl;
        cout << CYAN << "+==================================================================+" << RESET << endl;
        
        cout << CYAN << "| " << RESET << "Tracking ID      : " << BOLD << WHITE << "PKG" << setfill('0') << setw(6) << id << RESET;
        for(int i = 0; i < 35; i++) cout << " ";
        cout << CYAN << "|" << RESET << endl;
        
        cout << CYAN << "| " << RESET << "Sender           : " << senderName;
        for(int i = 0; i < 44 - senderName.length(); i++) cout << " ";
        cout << CYAN << "|" << RESET << endl;
        
        cout << CYAN << "| " << RESET << "Recipient        : " << recipientName;
        for(int i = 0; i < 44 - recipientName.length(); i++) cout << " ";
        cout << CYAN << "|" << RESET << endl;
        
        cout << CYAN << "| " << RESET << "Origin           : " << origin;
        for(int i = 0; i < 44 - origin.length(); i++) cout << " ";
        cout << CYAN << "|" << RESET << endl;
        
        cout << CYAN << "| " << RESET << "Destination      : " << destination;
        for(int i = 0; i < 44 - destination.length(); i++) cout << " ";
        cout << CYAN << "|" << RESET << endl;
        
        string priStr = priorityToString(priority);
        cout << CYAN << "| " << RESET << "Priority         : " << (priority == OVERNIGHT ? RED : priority == TWO_DAY ? YELLOW : GREEN) << priStr << RESET;
        for(int i = 0; i < 44 - priStr.length(); i++) cout << " ";
        cout << CYAN << "|" << RESET << endl;
        
        string wStr = weightToString(weight);
        cout << CYAN << "| " << RESET << "Weight Category  : " << wStr;
        for(int i = 0; i < 44 - wStr.length(); i++) cout << " ";
        cout << CYAN << "|" << RESET << endl;
        
        cout << CYAN << "| " << RESET << "Weight           : " << weightKg << " kg";
        ostringstream oss;
        oss << weightKg;
        for(int i = 0; i < 41 - oss.str().length(); i++) cout << " ";
        cout << CYAN << "|" << RESET << endl;
        
        string statStr = statusToString(status);
        cout << CYAN << "| " << RESET << "Status           : " << BOLD << (status == DELIVERED ? GREEN : status == MISSING ? RED : YELLOW) << statStr << RESET;
        for(int i = 0; i < 44 - statStr.length(); i++) cout << " ";
        cout << CYAN << "|" << RESET << endl;
        
        cout << CYAN << "| " << RESET << "Current Location : " << currentLocation;
        for(int i = 0; i < 44 - currentLocation.length(); i++) cout << " ";
        cout << CYAN << "|" << RESET << endl;
        
        cout << CYAN << "| " << RESET << "Registered       : " << registrationTime;
        for(int i = 0; i < 44 - registrationTime.length(); i++) cout << " ";
        cout << CYAN << "|" << RESET << endl;
        
        if(deliveryTime != "") {
            cout << CYAN << "| " << RESET << "Delivered        : " << deliveryTime;
            for(int i = 0; i < 44 - deliveryTime.length(); i++) cout << " ";
            cout << CYAN << "|" << RESET << endl;
        }
        
        cout << CYAN << "| " << RESET << "Delivery Attempts: " << deliveryAttempts;
        ostringstream oss2;
        oss2 << deliveryAttempts;
        for(int i = 0; i < 44 - oss2.str().length(); i++) cout << " ";
        cout << CYAN << "|" << RESET << endl;
        
        cout << BOLD << CYAN << "+==================================================================+" << RESET << endl;
    }
    
    void printTimeline() {
        cout << endl << BOLD << MAGENTA << "TRACKING TIMELINE:" << RESET << endl;
        printLine(80, '-');
        
        EventLog* current = eventHistory;
        int count = 1;
        while(current) {
            cout << CYAN << "  " << count++ << ". " << RESET;
            cout << BOLD << "[" << current->timestamp << "]" << RESET << " ";
            cout << YELLOW << current->location << RESET << " - ";
            cout << GREEN << statusToString(current->status) << RESET << endl;
            cout << "     " << current->description << endl;
            if(current->next) cout << CYAN << "     v" << RESET << endl;
            current = current->next;
        }
        printLine(80, '-');
    }
};

// ============================================================================
// CUSTOM HASH TABLE
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
    
    int hashFunction(int key) {
        return key % TABLE_SIZE;
    }
    
public:
    HashTable() {
        for(int i = 0; i < TABLE_SIZE; i++) {
            table[i] = nullptr;
        }
    }
    
    void insert(int key, T* value) {
        int index = hashFunction(key);
        HashNode<T>* newNode = new HashNode<T>(key, value);
        
        if(!table[index]) {
            table[index] = newNode;
        } else {
            HashNode<T>* temp = table[index];
            while(temp->next) temp = temp->next;
            temp->next = newNode;
        }
    }
    
    T* search(int key) {
        int index = hashFunction(key);
        HashNode<T>* temp = table[index];
        
        while(temp) {
            if(temp->key == key) return temp->value;
            temp = temp->next;
        }
        return nullptr;
    }
    
    bool remove(int key) {
        int index = hashFunction(key);
        HashNode<T>* temp = table[index];
        HashNode<T>* prev = nullptr;
        
        while(temp) {
            if(temp->key == key) {
                if(prev) {
                    prev->next = temp->next;
                } else {
                    table[index] = temp->next;
                }
                delete temp;
                return true;
            }
            prev = temp;
            temp = temp->next;
        }
        return false;
    }
    
    ~HashTable() {
        for(int i = 0; i < TABLE_SIZE; i++) {
            HashNode<T>* temp = table[i];
            while(temp) {
                HashNode<T>* toDelete = temp;
                temp = temp->next;
                delete toDelete;
            }
        }
    }
};

// ============================================================================
// MIN HEAP FOR PRIORITY QUEUE
// ============================================================================
class MinHeap {
private:
    Parcel** heap;
    int capacity;
    int size;
    
    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }
    
    void swap(Parcel** a, Parcel** b) {
        Parcel* temp = *a;
        *a = *b;
        *b = temp;
    }
    
    void heapifyUp(int i) {
        while(i > 0 && compareParcels(heap[i], heap[parent(i)])) {
            swap(&heap[i], &heap[parent(i)]);
            i = parent(i);
        }
    }
    
    void heapifyDown(int i) {
        int minIndex = i;
        int left = leftChild(i);
        int right = rightChild(i);
        
        if(left < size && compareParcels(heap[left], heap[minIndex])) {
            minIndex = left;
        }
        if(right < size && compareParcels(heap[right], heap[minIndex])) {
            minIndex = right;
        }
        
        if(i != minIndex) {
            swap(&heap[i], &heap[minIndex]);
            heapifyDown(minIndex);
        }
    }
    
    bool compareParcels(Parcel* a, Parcel* b) {
        // Lower priority value = higher priority (OVERNIGHT=1 is highest)
        if(a->priority != b->priority) {
            return a->priority < b->priority;
        }
        // If same priority, lighter parcels first
        return a->weightKg < b->weightKg;
    }
    
public:
    MinHeap(int cap = 1000) : capacity(cap), size(0) {
        heap = new Parcel*[capacity];
    }
    
    void insert(Parcel* parcel) {
        if(size == capacity) {
            cout << RED << "Heap is full!" << RESET << endl;
            return;
        }
        heap[size] = parcel;
        heapifyUp(size);
        size++;
    }
    
    Parcel* extractMin() {
        if(size == 0) return nullptr;
        
        Parcel* min = heap[0];
        heap[0] = heap[size - 1];
        size--;
        heapifyDown(0);
        
        return min;
    }
    
    Parcel* peek() {
        return size > 0 ? heap[0] : nullptr;
    }
    
    bool isEmpty() {
        return size == 0;
    }
    
    int getSize() {
        return size;
    }
    
    void display() {
        if(isEmpty()) {
            printWarning("Queue is empty");
            return;
        }
        
        cout << BOLD << CYAN << "\n+==============================================================================+" << RESET << endl;
        cout << CYAN << "|  ID     | Priority   | Weight | From -> To                                   |" << RESET << endl;
        cout << CYAN << "+==============================================================================+" << RESET << endl;
        
        for(int i = 0; i < size; i++) {
            cout << CYAN << "| " << RESET;
            cout << "PKG" << setfill('0') << setw(6) << heap[i]->id << " | ";
            
            string pri = priorityToString(heap[i]->priority);
            cout << (heap[i]->priority == OVERNIGHT ? RED : heap[i]->priority == TWO_DAY ? YELLOW : GREEN);
            cout << setfill(' ') << setw(10) << left << pri << RESET << "| ";
            
            cout << setw(6) << right << heap[i]->weightKg << " | ";
            
            string route = heap[i]->origin + " -> " + heap[i]->destination;
            cout << setw(42) << left << route;
            cout << CYAN << " |" << RESET << endl;
        }
        
        cout << BOLD << CYAN << "+==============================================================================+" << RESET << endl;
        cout << GREEN << "Total parcels in queue: " << size << RESET << endl;
    }
    
    ~MinHeap() {
        delete[] heap;
    }
};

// ============================================================================
// QUEUE
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
    
    void enqueue(T* data) {
        QueueNode<T>* newNode = new QueueNode<T>(data);
        if(!rear) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        size++;
    }
    
    T* dequeue() {
        if(!front) return nullptr;
        
        QueueNode<T>* temp = front;
        T* data = front->data;
        front = front->next;
        
        if(!front) rear = nullptr;
        
        delete temp;
        size--;
        return data;
    }
    
    T* peek() {
        return front ? front->data : nullptr;
    }
    
    bool isEmpty() {
        return front == nullptr;
    }
    
    int getSize() {
        return size;
    }
    
    void display(string queueName) {
        if(isEmpty()) {
            printWarning(queueName + " is empty");
            return;
        }
        
        cout << BOLD << MAGENTA << "\n" << queueName << ":" << RESET << endl;
        printLine(80, '-');
        
        QueueNode<T>* temp = front;
        int count = 1;
        while(temp) {
            cout << CYAN << count++ << ". " << RESET;
            cout << "PKG" << setfill('0') << setw(6) << temp->data->id << " - ";
            cout << temp->data->origin << " -> " << temp->data->destination;
            cout << " [" << priorityToString(temp->data->priority) << "]" << endl;
            temp = temp->next;
        }
        printLine(80, '-');
        cout << GREEN << "Total in queue: " << size << RESET << endl;
    }
    
    ~Queue() {
        while(front) {
            QueueNode<T>* temp = front;
            front = front->next;
            delete temp;
        }
    }
};

// ============================================================================
// GRAPH (Network)
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
        if(!edges) {
            edges = newEdge;
        } else {
            Edge* temp = edges;
            while(temp->next) temp = temp->next;
            temp->next = newEdge;
        }
    }
};

class Graph {
private:
    GraphNode* head;
    
    GraphNode* findNode(string city) {
        GraphNode* temp = head;
        while(temp) {
            if(temp->city == city) return temp;
            temp = temp->next;
        }
        return nullptr;
    }
    
public:
    Graph() : head(nullptr) {}
    
    void addCity(string city) {
        if(findNode(city)) return;
        
        GraphNode* newNode = new GraphNode(city);
        if(!head) {
            head = newNode;
        } else {
            GraphNode* temp = head;
            while(temp->next) temp = temp->next;
            temp->next = newNode;
        }
    }
    
    void addRoute(string from, string to, int distance) {
        GraphNode* fromNode = findNode(from);
        GraphNode* toNode = findNode(to);
        
        if(!fromNode) {
            addCity(from);
            fromNode = findNode(from);
        }
        if(!toNode) {
            addCity(to);
        }
        
        fromNode->addEdge(to, distance);
    }
    
    void blockRoute(string from, string to) {
        GraphNode* fromNode = findNode(from);
        if(!fromNode) return;
        
        Edge* edge = fromNode->edges;
        while(edge) {
            if(edge->destination == to) {
                edge->isBlocked = true;
                printWarning("Route blocked: " + from + " -> " + to);
                return;
            }
            edge = edge->next;
        }
    }
    
    void unblockRoute(string from, string to) {
        GraphNode* fromNode = findNode(from);
        if(!fromNode) return;
        
        Edge* edge = fromNode->edges;
        while(edge) {
            if(edge->destination == to) {
                edge->isBlocked = false;
                printSuccess("Route unblocked: " + from + " -> " + to);
                return;
            }
            edge = edge->next;
        }
    }
    
    // Dijkstra's Algorithm
    void findShortestPath(string start, string end) {
        const int MAX_CITIES = 100;
        string cities[MAX_CITIES];
        int distances[MAX_CITIES];
        bool visited[MAX_CITIES];
        string previous[MAX_CITIES];
        int cityCount = 0;
        
        // Initialize
        GraphNode* temp = head;
        while(temp && cityCount < MAX_CITIES) {
            cities[cityCount] = temp->city;
            distances[cityCount] = 999999;
            visited[cityCount] = false;
            previous[cityCount] = "";
            cityCount++;
            temp = temp->next;
        }
        
        // Find start index
        int startIdx = -1;
        for(int i = 0; i < cityCount; i++) {
            if(cities[i] == start) {
                startIdx = i;
                distances[i] = 0;
                break;
            }
        }
        
        if(startIdx == -1) {
            printError("Start city not found in network");
            return;
        }
        
        // Dijkstra's main loop
        for(int count = 0; count < cityCount - 1; count++) {
            int minDist = 999999;
            int minIdx = -1;
            
            for(int i = 0; i < cityCount; i++) {
                if(!visited[i] && distances[i] < minDist) {
                    minDist = distances[i];
                    minIdx = i;
                }
            }
            
            if(minIdx == -1) break;
            
            visited[minIdx] = true;
            
            // Update distances
            GraphNode* currentNode = findNode(cities[minIdx]);
            if(currentNode) {
                Edge* edge = currentNode->edges;
                while(edge) {
                    if(!edge->isBlocked) {
                        int destIdx = -1;
                        for(int i = 0; i < cityCount; i++) {
                            if(cities[i] == edge->destination) {
                                destIdx = i;
                                break;
                            }
                        }
                        
                        if(destIdx != -1 && !visited[destIdx]) {
                            int newDist = distances[minIdx] + edge->distance;
                            if(newDist < distances[destIdx]) {
                                distances[destIdx] = newDist;
                                previous[destIdx] = cities[minIdx];
                            }
                        }
                    }
                    edge = edge->next;
                }
            }
        }
        
        // Find end index and print path
        int endIdx = -1;
        for(int i = 0; i < cityCount; i++) {
            if(cities[i] == end) {
                endIdx = i;
                break;
            }
        }
        
        if(endIdx == -1 || distances[endIdx] == 999999) {
            printError("No route available from " + start + " to " + end);
            return;
        }
        
        // Reconstruct path
        string path[MAX_CITIES];
        int pathLen = 0;
        string current = end;
        
        while(current != "") {
            path[pathLen++] = current;
            for(int i = 0; i < cityCount; i++) {
                if(cities[i] == current) {
                    current = previous[i];
                    break;
                }
            }
        }
        
        // Print result
        cout << BOLD << GREEN << "\n[TRUCK] SHORTEST ROUTE FOUND:" << RESET << endl;
        printLine(80, '=');
        cout << CYAN << "Total Distance: " << BOLD << distances[endIdx] << " km" << RESET << endl;
        cout << CYAN << "Route: " << RESET;
        
        for(int i = pathLen - 1; i >= 0; i--) {
            cout << YELLOW << path[i] << RESET;
            if(i > 0) cout << CYAN << " -> " << RESET;
        }
        cout << endl;
        printLine(80, '=');
    }
    
    void displayNetwork() {
        cout << BOLD << MAGENTA << "\n[MAP] DELIVERY NETWORK:" << RESET << endl;
        printLine(80, '=');
        
        GraphNode* temp = head;
        while(temp) {
            cout << BOLD << CYAN << temp->city << RESET << " connects to:" << endl;
            Edge* edge = temp->edges;
            while(edge) {
                cout << "  -> " << YELLOW << edge->destination << RESET;
                cout << " (" << edge->distance << " km)";
                if(edge->isBlocked) cout << RED << " [BLOCKED]" << RESET;
                cout << endl;
                edge = edge->next;
            }
            cout << endl;
            temp = temp->next;
        }
        printLine(80, '=');
    }
    
    ~Graph() {
        while(head) {
            GraphNode* temp = head;
            head = head->next;
            
            while(temp->edges) {
                Edge* edgeTemp = temp->edges;
                temp->edges = temp->edges->next;
                delete edgeTemp;
            }
            delete temp;
        }
    }
};

// ============================================================================
// OPERATION LOG
// ============================================================================
struct Operation {
    string operationType;
    int parcelId;
    string timestamp;
    string details;
    Operation* next;
    
    Operation(string type, int id, string det) 
        : operationType(type), parcelId(id), timestamp(getCurrentTimestamp()), 
          details(det), next(nullptr) {}
};

class OperationLog {
private:
    Operation* top;
    int count;
    
public:
    OperationLog() : top(nullptr), count(0) {}
    
    void push(string type, int parcelId, string details) {
        Operation* newOp = new Operation(type, parcelId, details);
        newOp->next = top;
        top = newOp;
        count++;
    }
    
    Operation* pop() {
        if(!top) return nullptr;
        
        Operation* temp = top;
        top = top->next;
        count--;
        return temp;
    }
    
    void displayLog() {
        if(!top) {
            printWarning("No operations logged yet");
            return;
        }
        
        cout << BOLD << MAGENTA << "\n[LOG] OPERATION LOG (Most Recent First):" << RESET << endl;
        printLine(80, '=');
        
        Operation* temp = top;
        int num = 1;
        while(temp && num <= 20) {  // Show last 20 operations
            cout << CYAN << num++ << ". " << RESET;
            cout << "[" << temp->timestamp << "] ";
            cout << YELLOW << temp->operationType << RESET;
            cout << " - PKG" << setfill('0') << setw(6) << temp->parcelId;
            cout << " - " << temp->details << endl;
            temp = temp->next;
        }
        printLine(80, '=');
        cout << GREEN << "Total operations logged: " << count << RESET << endl;
    }
    
    ~OperationLog() {
        while(top) {
            Operation* temp = top;
            top = top->next;
            delete temp;
        }
    }
};

// ============================================================================
// COURIER LOGISTICS SYSTEM
// ============================================================================
class CourierLogisticsSystem {
private:
    HashTable<Parcel> parcelDatabase;  // Fast O(1) lookup
    MinHeap priorityQueue;             // Priority-based sorting
    Queue<Parcel> pickupQueue;         // Pickup operations
    Queue<Parcel> warehouseQueue;      // Warehouse processing
    Queue<Parcel> transitQueue;        // In-transit parcels
    Graph routingNetwork;              // Delivery network
    OperationLog operationLog;         // Operation history
    int nextParcelId;
    
    void initializeNetwork() {
        // Create a realistic delivery network
        routingNetwork.addRoute("Lahore", "Islamabad", 380);
        routingNetwork.addRoute("Lahore", "Multan", 340);
        routingNetwork.addRoute("Lahore", "Faisalabad", 130);
        routingNetwork.addRoute("Islamabad", "Rawalpindi", 15);
        routingNetwork.addRoute("Islamabad", "Peshawar", 180);
        routingNetwork.addRoute("Islamabad", "Lahore", 380);
        routingNetwork.addRoute("Multan", "Bahawalpur", 100);
        routingNetwork.addRoute("Multan", "Lahore", 340);
        routingNetwork.addRoute("Faisalabad", "Sargodha", 90);
        routingNetwork.addRoute("Faisalabad", "Lahore", 130);
        routingNetwork.addRoute("Karachi", "Hyderabad", 165);
        routingNetwork.addRoute("Karachi", "Multan", 900);
        routingNetwork.addRoute("Peshawar", "Islamabad", 180);
        routingNetwork.addRoute("Quetta", "Multan", 650);
        routingNetwork.addRoute("Rawalpindi", "Lahore", 370);
    }
    
public:
    CourierLogisticsSystem() : nextParcelId(1001) {
        initializeNetwork();
    }
    
    void registerParcel() {
        printHeader("REGISTER NEW PARCEL");
        
        string sender, recipient, origin, destination;
        int priChoice, wtChoice;
        double weight;
        
        cout << CYAN << "Sender Name: " << RESET;
        cin.ignore();
        getline(cin, sender);
        
        cout << CYAN << "Recipient Name: " << RESET;
        getline(cin, recipient);
        
        cout << CYAN << "Origin City: " << RESET;
        getline(cin, origin);
        
        cout << CYAN << "Destination City: " << RESET;
        getline(cin, destination);
        
        cout << CYAN << "\nPriority Options:" << RESET << endl;
        cout << "  1. " << RED << "Overnight" << RESET << endl;
        cout << "  2. " << YELLOW << "2-Day" << RESET << endl;
        cout << "  3. " << GREEN << "Normal" << RESET << endl;
        cout << CYAN << "Select Priority: " << RESET;
        cin >> priChoice;
        
        cout << CYAN << "\nWeight Category:" << RESET << endl;
        cout << "  1. Light (<5kg)" << endl;
        cout << "  2. Medium (5-20kg)" << endl;
        cout << "  3. Heavy (>20kg)" << endl;
        cout << CYAN << "Select Category: " << RESET;
        cin >> wtChoice;
        
        cout << CYAN << "Exact Weight (kg): " << RESET;
        cin >> weight;
        
        Priority priority = static_cast<Priority>(priChoice);
        WeightCategory weightCat = static_cast<WeightCategory>(wtChoice);
        
        Parcel* newParcel = new Parcel(nextParcelId, sender, recipient, origin, 
                                       destination, priority, weightCat, weight);
        
        parcelDatabase.insert(nextParcelId, newParcel);
        priorityQueue.insert(newParcel);
        pickupQueue.enqueue(newParcel);
        newParcel->addEvent(origin, IN_PICKUP_QUEUE, "Added to pickup queue");
        
        operationLog.push("REGISTER", nextParcelId, "Parcel registered: " + origin + " -> " + destination);
        
        cout << endl;
        printSuccess("Parcel registered successfully!");
        cout << BOLD << "Tracking ID: " << YELLOW << "PKG" << setfill('0') << setw(6) << nextParcelId << RESET << endl;
        
        nextParcelId++;
        waitForEnter();
    }
    
    void viewParcelDetails() {
        printHeader("TRACK PARCEL");
        
        int id;
        cout << CYAN << "Enter Parcel ID: PKG" << RESET;
        cin >> id;
        
        Parcel* parcel = parcelDatabase.search(id);
        if(!parcel) {
            printError("Parcel not found!");
        } else {
            cout << endl;
            parcel->printDetails();
            parcel->printTimeline();
        }
        
        waitForEnter();
    }
    
    void processPickupQueue() {
        printHeader("PROCESS PICKUP QUEUE");
        
        if(pickupQueue.isEmpty()) {
            printWarning("No parcels in pickup queue");
            waitForEnter();
            return;
        }
        
        pickupQueue.display("PICKUP QUEUE");
        
        cout << CYAN << "\nProcess how many parcels? (0 to cancel): " << RESET;
        int count;
        cin >> count;
        
        for(int i = 0; i < count && !pickupQueue.isEmpty(); i++) {
            Parcel* parcel = pickupQueue.dequeue();
            parcel->addEvent(parcel->currentLocation, PICKED_UP, "Picked up by courier");
            warehouseQueue.enqueue(parcel);
            parcel->addEvent("Warehouse Hub", AT_WAREHOUSE, "Arrived at warehouse for sorting");
            
            operationLog.push("PICKUP", parcel->id, "Picked up from " + parcel->origin);
            printSuccess("PKG" + to_string(parcel->id) + " picked up and sent to warehouse");
        }
        
        waitForEnter();
    }
    
    void processWarehouseQueue() {
        printHeader("PROCESS WAREHOUSE QUEUE");
        
        if(warehouseQueue.isEmpty()) {
            printWarning("No parcels in warehouse");
            waitForEnter();
            return;
        }
        
        warehouseQueue.display("WAREHOUSE QUEUE");
        
        cout << CYAN << "\nProcess how many parcels? (0 to cancel): " << RESET;
        int count;
        cin >> count;
        
        for(int i = 0; i < count && !warehouseQueue.isEmpty(); i++) {
            Parcel* parcel = warehouseQueue.dequeue();
            parcel->addEvent("Warehouse Hub", IN_TRANSIT, "Dispatched from warehouse");
            transitQueue.enqueue(parcel);
            
            operationLog.push("WAREHOUSE", parcel->id, "Processed at warehouse");
            printSuccess("PKG" + to_string(parcel->id) + " dispatched for transit");
        }
        
        waitForEnter();
    }
    
    void processTransitQueue() {
        printHeader("PROCESS TRANSIT QUEUE");
        
        if(transitQueue.isEmpty()) {
            printWarning("No parcels in transit");
            waitForEnter();
            return;
        }
        
        transitQueue.display("TRANSIT QUEUE");
        
        cout << CYAN << "\nDeliver how many parcels? (0 to cancel): " << RESET;
        int count;
        cin >> count;
        
        for(int i = 0; i < count && !transitQueue.isEmpty(); i++) {
            Parcel* parcel = transitQueue.dequeue();
            parcel->addEvent(parcel->destination, OUT_FOR_DELIVERY, "Out for final delivery");
            parcel->deliveryAttempts++;
            
            // 90% success rate simulation
            if(rand() % 10 < 9) {
                parcel->addEvent(parcel->destination, DELIVERED, "Successfully delivered to recipient");
                parcel->deliveryTime = getCurrentTimestamp();
                operationLog.push("DELIVER", parcel->id, "Successfully delivered");
                printSuccess("PKG" + to_string(parcel->id) + " delivered successfully!");
            } else {
                parcel->addEvent(parcel->destination, OUT_FOR_DELIVERY, "Delivery failed - recipient unavailable");
                transitQueue.enqueue(parcel);  // Re-attempt
                operationLog.push("ATTEMPT", parcel->id, "Delivery attempt #" + to_string(parcel->deliveryAttempts));
                printWarning("PKG" + to_string(parcel->id) + " delivery failed - will retry");
            }
        }
        
        waitForEnter();
    }
    
    void viewPriorityQueue() {
        printHeader("PRIORITY SORTED PARCELS");
        priorityQueue.display();
        waitForEnter();
    }
    
    void viewAllQueues() {
        printHeader("ALL QUEUES STATUS");
        
        cout << BOLD << YELLOW << "1. PICKUP QUEUE:" << RESET << endl;
        if(!pickupQueue.isEmpty()) {
            pickupQueue.display("Pickup Queue");
        } else {
            printInfo("Empty");
        }
        
        cout << endl << BOLD << YELLOW << "2. WAREHOUSE QUEUE:" << RESET << endl;
        if(!warehouseQueue.isEmpty()) {
            warehouseQueue.display("Warehouse Queue");
        } else {
            printInfo("Empty");
        }
        
        cout << endl << BOLD << YELLOW << "3. TRANSIT QUEUE:" << RESET << endl;
        if(!transitQueue.isEmpty()) {
            transitQueue.display("Transit Queue");
        } else {
            printInfo("Empty");
        }
        
        waitForEnter();
    }
    
    void findRoute() {
        printHeader("ROUTE FINDER");
        
        string start, end;
        cout << CYAN << "Starting City: " << RESET;
        cin.ignore();
        getline(cin, start);
        
        cout << CYAN << "Destination City: " << RESET;
        getline(cin, end);
        
        routingNetwork.findShortestPath(start, end);
        
        waitForEnter();
    }
    
    void manageRoutes() {
        printHeader("ROUTE MANAGEMENT");
        
        cout << "1. View Network" << endl;
        cout << "2. Block Route" << endl;
        cout << "3. Unblock Route" << endl;
        cout << CYAN << "Choice: " << RESET;
        
        int choice;
        cin >> choice;
        
        if(choice == 1) {
            routingNetwork.displayNetwork();
        } else if(choice == 2 || choice == 3) {
            string from, to;
            cout << CYAN << "From City: " << RESET;
            cin.ignore();
            getline(cin, from);
            cout << CYAN << "To City: " << RESET;
            getline(cin, to);
            
            if(choice == 2) {
                routingNetwork.blockRoute(from, to);
                operationLog.push("BLOCK_ROUTE", 0, from + " -> " + to);
            } else {
                routingNetwork.unblockRoute(from, to);
                operationLog.push("UNBLOCK_ROUTE", 0, from + " -> " + to);
            }
        }
        
        waitForEnter();
    }
    
    void viewOperationLog() {
        printHeader("OPERATION LOG");
        operationLog.displayLog();
        waitForEnter();
    }
    
    void markParcelMissing() {
        printHeader("MARK PARCEL AS MISSING");
        
        int id;
        cout << CYAN << "Enter Parcel ID: PKG" << RESET;
        cin >> id;
        
        Parcel* parcel = parcelDatabase.search(id);
        if(!parcel) {
            printError("Parcel not found!");
        } else {
            parcel->addEvent(parcel->currentLocation, MISSING, "Parcel marked as missing - investigation initiated");
            operationLog.push("MISSING", id, "Marked as missing");
            printWarning("Parcel PKG" + to_string(id) + " marked as MISSING");
        }
        
        waitForEnter();
    }
    
    void generateReport() {
        printHeader("SYSTEM ANALYTICS REPORT");
        
        int inPickup = pickupQueue.getSize();
        int atWarehouse = warehouseQueue.getSize();
        int inTransit = transitQueue.getSize();
        
        cout << BOLD << CYAN << "+==================================================================+" << RESET << endl;
        cout << CYAN << "| " << YELLOW << "                        SYSTEM STATISTICS                        " << CYAN << "|" << RESET << endl;
        cout << CYAN << "+==================================================================+" << RESET << endl;
        cout << CYAN << "| " << RESET << "Total Parcels Registered : " << BOLD << (nextParcelId - 1001);
        for(int i = 0; i < 31; i++) cout << " ";
        cout << CYAN << "|" << RESET << endl;
        cout << CYAN << "| " << RESET << "In Pickup Queue          : " << inPickup;
        for(int i = 0; i < 36; i++) cout << " ";
        cout << CYAN << "|" << RESET << endl;
        cout << CYAN << "| " << RESET << "At Warehouse             : " << atWarehouse;
        for(int i = 0; i < 36; i++) cout << " ";
        cout << CYAN << "|" << RESET << endl;
        cout << CYAN << "| " << RESET << "In Transit               : " << inTransit;
        for(int i = 0; i < 36; i++) cout << " ";
        cout << CYAN << "|" << RESET << endl;
        cout << CYAN << "+==================================================================+" << RESET << endl;
        
        waitForEnter();
    }
    
    void displayMainMenu() {
        printHeader("SwiftEx Intelligent Courier Logistics System");
        
        cout << BOLD << CYAN << "+==================================================================+" << RESET << endl;
        cout << CYAN << "| " << YELLOW << "                            MAIN MENU                            " << CYAN << "|" << RESET << endl;
        cout << CYAN << "+==================================================================+" << RESET << endl;
        cout << CYAN << "| " << GREEN << "1." << RESET << "  Register New Parcel                                        " << CYAN << "|" << RESET << endl;
        cout << CYAN << "| " << GREEN << "2." << RESET << "  Track Parcel (View Details & Timeline)                     " << CYAN << "|" << RESET << endl;
        cout << CYAN << "| " << GREEN << "3." << RESET << "  View Priority Queue (Sorted Parcels)                       " << CYAN << "|" << RESET << endl;
        cout << CYAN << "| " << GREEN << "4." << RESET << "  Process Pickup Queue                                       " << CYAN << "|" << RESET << endl;
        cout << CYAN << "| " << GREEN << "5." << RESET << "  Process Warehouse Queue                                    " << CYAN << "|" << RESET << endl;
        cout << CYAN << "| " << GREEN << "6." << RESET << "  Process Transit Queue (Attempt Delivery)                   " << CYAN << "|" << RESET << endl;
        cout << CYAN << "| " << GREEN << "7." << RESET << "  View All Queues Status                                     " << CYAN << "|" << RESET << endl;
        cout << CYAN << "| " << GREEN << "8." << RESET << "  Find Shortest Route (Dijkstra)                             " << CYAN << "|" << RESET << endl;
        cout << CYAN << "| " << GREEN << "9." << RESET << "  Manage Network (Block/Unblock Routes)                      " << CYAN << "|" << RESET << endl;
        cout << CYAN << "| " << GREEN << "10." << RESET << " System Analytics Report                                    " << CYAN << "|" << RESET << endl;
        cout << CYAN << "| " << GREEN << "11." << RESET << " View Operation Log                                         " << CYAN << "|" << RESET << endl;
        cout << CYAN << "| " << GREEN << "12." << RESET << " Mark Parcel as Missing                                     " << CYAN << "|" << RESET << endl;
        cout << CYAN << "+==================================================================+" << RESET << endl;
        cout << CYAN << "| " << RED << "0." << RESET << "   Exit System                                                " << CYAN << "|" << RESET << endl;
        cout << BOLD << CYAN << "+==================================================================+" << RESET << endl;
    }
    
    void run() {
        int choice;
        do {
            displayMainMenu();
            cout << "\n" << BOLD << CYAN << "Enter Choice: " << RESET;
            
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                choice = -1;
            }

            switch(choice) {
                case 1: registerParcel(); break;
                case 2: viewParcelDetails(); break;
                case 3: viewPriorityQueue(); break;
                case 4: processPickupQueue(); break;
                case 5: processWarehouseQueue(); break;
                case 6: processTransitQueue(); break;
                case 7: viewAllQueues(); break;
                case 8: findRoute(); break;
                case 9: manageRoutes(); break;
                case 10: generateReport(); break;
                case 11: viewOperationLog(); break;
                case 12: markParcelMissing(); break;
                case 0: 
                    cout << endl;
                    printSuccess("Thank you for using SwiftEx Logistics System!");
                    printInfo("Shutting down...");
                    break;
                default:
                    printError("Invalid choice! Please try again.");
                    waitForEnter();
            }
        } while(choice != 0);
    }
};

// ============================================================================
// MAIN EXECUTION
// ============================================================================
int main() {
    // Seed random number generator
    srand(static_cast<unsigned int>(time(0)));

    // Enable basic colors on Windows if possible
    #ifdef _WIN32
        system("color");
    #endif

    CourierLogisticsSystem app;
    app.run();

    return 0;
}