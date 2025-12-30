// File: SharedData.h
#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include <string>
#include <iostream>
using namespace std;

// Represents a single package
struct Parcel {
    int parcelID;
    string destination;
    int priority; // 3 = High (Overnight), 2 = Medium, 1 = Low
    string status; // "Sorting", "In Transit", "Delivered"
    
    // Constructor for easy creation
    Parcel(int id = 0, string dest = "", int prio = 1) {
        parcelID = id;
        destination = dest;
        priority = prio;
        status = "At Hub";
    }
};

#endif
