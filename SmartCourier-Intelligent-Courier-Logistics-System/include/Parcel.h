#ifndef PARCEL_H
#define PARCEL_H

#include <string>
#include <iostream>
#include <iomanip>
#include "GlobalConstants.h"

// --- History Linked List Node ---
// A Linked List is used here because a parcel can have an unlimited number of events.
struct EventLog {
    std::string timestamp;
    std::string location;
    ParcelStatus status;
    std::string description;
    EventLog* next; // Pointer to the next event in history

    EventLog(std::string ts, std::string loc, ParcelStatus st, std::string desc) 
        : timestamp(ts), location(loc), status(st), description(desc), next(nullptr) {}
};

// --- Main Parcel Structure ---
struct Parcel {
    int id;                     // Unique Tracking ID (Key)
    std::string senderName;
    std::string recipientName;
    std::string origin;
    std::string destination;
    Priority priority;          // Used for Heap Sorting
    WeightCategory weightCat;
    double weightKg;
    ParcelStatus status;        // Current State
    std::string currentLocation;
    std::string registrationTime;
    EventLog* eventHistory;     // HEAD of the Linked List for tracking history

    // Constructor: Initializes a new parcel and creates the first history log.
    Parcel(int _id, std::string sender, std::string recipient, std::string orig, std::string dest, 
           Priority pri, WeightCategory wt, double wkg, std::string regTime)
        : id(_id), senderName(sender), recipientName(recipient), 
          origin(orig), destination(dest), priority(pri), weightCat(wt), 
          weightKg(wkg), status(REGISTERED), currentLocation(orig),
          registrationTime(regTime), eventHistory(nullptr) 
    {
        // Automatically add the "Registered" event so history isn't empty on creation.
        addEvent(regTime, orig, REGISTERED, "Parcel Registered in System");
    }

    // --- Add Event Function ---
    // Appends a new node to the end of the `eventHistory` Linked List.
    // Complexity: O(N) where N is number of events.
    void addEvent(std::string timestamp, std::string location, ParcelStatus newStatus, std::string description) {
        EventLog* newEvent = new EventLog(timestamp, location, newStatus, description);
        
        if(!eventHistory) {
            eventHistory = newEvent; // First event
        } else {
            // Traverse to the end of the list
            EventLog* temp = eventHistory;
            while(temp->next) temp = temp->next;
            temp->next = newEvent;
        }
        // Update current status properties
        status = newStatus;
        currentLocation = location;
    }

    // --- UI Helper: Print Ticket ---
    // Displays a formatted summary of the parcel.
    void printTicket() {
        std::cout << "  ------------------------------------------" << std::endl;
        std::cout << "  | ID: PKG" << std::setw(6) << std::setfill('0') << id << std::setfill(' ') 
                  << " | " << std::setw(15) << statusToString(status) << " |" << std::endl;
        std::cout << "  ------------------------------------------" << std::endl;
        std::cout << "  From:     " << origin << std::endl;
        std::cout << "  To:       " << destination << std::endl;
        std::cout << "  Priority: " << priorityToString(priority) << std::endl;
        std::cout << "  Weight:   " << weightKg << "kg" << std::endl;
        std::cout << "  Current:  " << currentLocation << std::endl;
        std::cout << "  ------------------------------------------" << std::endl;
    }
};

#endif
