#ifndef TRACKER_H
#define TRACKER_H

#include "../SharedData.h"

struct HistoryNode {
    string location;
    string timestamp;
    HistoryNode* next;
};

class ParcelTracker {
private:
    HistoryNode* head;

public:
    ParcelTracker();
    void addUpdate(string location, string time);
    void showHistory(int parcelID);
};

#endif
