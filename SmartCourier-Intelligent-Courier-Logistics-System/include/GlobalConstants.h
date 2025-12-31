#ifndef GLOBAL_CONSTANTS_H
#define GLOBAL_CONSTANTS_H

#include <string>

// --- ANSI Color Codes for UI ---
// We use these macros to make the console output colorful and professional.
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BG_BLUE "\033[44m"

// --- Enums for Type Safety ---
// Using Enums prevents errors (e.g., passing a weight as a priority).

// Priority levels: Overnight is the most urgent (1).
enum Priority { OVERNIGHT = 1, TWO_DAY = 2, NORMAL = 3 };

// Weight categories for billing/sorting logic.
enum WeightCategory { LIGHT = 1, MEDIUM = 2, HEAVY = 3 };

// The lifecycle states of a parcel.
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

// --- Inline Helper Functions ---
// Convert Enum values to readable Strings for UI display.

inline std::string priorityToString(Priority p) {
    switch(p) {
        case OVERNIGHT: return "Overnight";
        case TWO_DAY: return "2-Day";
        case NORMAL: return "Normal";
        default: return "Unknown";
    }
}

inline std::string statusToString(ParcelStatus s) {
    switch(s) {
        case REGISTERED: return "Registered";
        case IN_PICKUP_QUEUE: return "Pending Pickup";
        case PICKED_UP: return "Picked Up";
        case AT_WAREHOUSE: return "At Warehouse";
        case IN_TRANSIT: return "In Transit";
        case OUT_FOR_DELIVERY: return "Out for Delivery";
        case DELIVERED: return "Delivered";
        case RETURNED: return "Returned";
        case MISSING: return "Missing";
        default: return "Unknown";
    }
}

#endif