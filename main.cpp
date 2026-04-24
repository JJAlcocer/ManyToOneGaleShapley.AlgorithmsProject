#include <iostream>
#include "stableMatching.h"

int main() {
    int totalGuests = 50;
    int totalTables = 10;
    int capacityPerTable = 5;

    std::vector<Guest> guestList;
    std::vector<Table> tableList;

    generateRandomData(totalGuests, totalTables, capacityPerTable, guestList, tableList);
    
    MatchResult result = runGaleShapley(totalGuests, totalTables, guestList, tableList);

    std::cout << "Seating Arrangement (50 Guests):\n";
    for(int i = 0; i < totalTables; ++i) {
        std::cout << "Table " << i << ": ";
        for(int guest : result.assignments[i]) {
            std::cout << guest << " ";
        }
        std::cout << "\n";
    }

    return 0;
}
