#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "../SharedData.h"
#include <queue>

class Operations {
private:
    queue<Parcel> processingQueue;

public:
    void addParcelToQueue(Parcel p);
    void processNextParcel();
    void showQueueStatus();
};

#endif
