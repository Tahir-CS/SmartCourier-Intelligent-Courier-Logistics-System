// Shared parcel structure and enums for all modules
#pragma once
#include <string>

namespace SmartCourier {
    enum class Priority { Overnight, TwoDay, Normal };
    enum class Status { AtWarehouse, InTransit, Delivered, Returned };

    struct Parcel {
        int id;
        std::string sender;
        std::string recipient;
        std::string destinationZone;
        double weight;
        Priority priority;
        Status status;
        // Add more attributes as needed
    };
}
