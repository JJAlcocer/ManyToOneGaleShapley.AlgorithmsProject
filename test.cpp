#include <iostream>
#include <chrono>
#include "stableMatching.h"

long long calculateSpaceAllocated(long long numGuests, long long numTables) {
    long long guestPrefs = numGuests * numTables * sizeof(int);
    long long tablePrefs = numTables * numGuests * sizeof(int);
    long long tableRanks = numTables * numGuests * sizeof(int);
    long long queuesAndArrays = (numGuests * 2) * sizeof(int);
    long long assignments = numGuests * sizeof(int);
    return guestPrefs + tablePrefs + tableRanks + queuesAndArrays + assignments;
}

void runPerformanceTests() {
    std::vector<int> testSizes = {50, 500, 1000, 2500, 5000};
    int capacityPerTable = 5;

    std::cout << "Guests,Tables,Time(ms),Space(Bytes),TotalProposals,MaxPossibleProposals,Stable\n";

    for(int size : testSizes) {
        int tables = size / capacityPerTable;
        std::vector<Guest> guests;
        std::vector<Table> tableList;

        generateRandomData(size, tables, capacityPerTable, guests, tableList);

        long long spaceAllocated = calculateSpaceAllocated(size, tables);
        long long maxProposals = (long long)size * tables;

        auto start = std::chrono::high_resolution_clock::now();
        MatchResult result = runGaleShapley(size, tables, guests, tableList);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> duration = end - start;
        bool isStable = verifyStability(size, tables, guests, tableList, result.assignments);

        std::cout << size << "," 
                  << tables << "," 
                  << duration.count() << "," 
                  << spaceAllocated << "," 
                  << result.totalProposals << "," 
                  << maxProposals << "," 
                  << (isStable ? "Yes" : "No") << "\n";
    }
}

int main() {
    runPerformanceTests();
    return 0;
}
