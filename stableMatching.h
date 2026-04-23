#ifndef STABLE_MATCHING_H
#define STABLE_MATCHING_H

#include <vector>

struct Table {
    int capacity;
    std::vector<int> guestPreferences;
    std::vector<int> guestRanks;
};

struct Guest {
    std::vector<int> tablePreferences;
};

void generateRandomData(int numGuests, int numTables, int tableCapacity, std::vector<Guest>& guests, std::vector<Table>& tables);
std::vector<std::vector<int>> runGaleShapley(int numGuests, int numTables, const std::vector<Guest>& guests, const std::vector<Table>& tables);
bool verifyStability(int numGuests, int numTables, const std::vector<Guest>& guests, const std::vector<Table>& tables, const std::vector<std::vector<int>>& assignments);

#endif
