#include <iostream>
#include <chrono>
#include "stableMatching.h"

void runPerformanceTests() {
    std::vector<int> testSizes = {50, 100, 500, 1000, 1500, 2500, 5000};
    int capacityPerTable = 5;

    std::cout << "Performance and Stability Scaling Tests:\n";
    std::cout << "Guests\tTables\tTime(ms)\tStable?\n";
    std::cout << "----------------------------------------\n";

    for(int size : testSizes) {
        int tables = size / capacityPerTable;
        std::vector<Guest> guests;
        std::vector<Table> tableList;

        generateRandomData(size, tables, capacityPerTable, guests, tableList);

        auto start = std::chrono::high_resolution_clock::now();
        std::vector<std::vector<int>> assignments = runGaleShapley(size, tables, guests, tableList);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> duration = end - start;
        bool isStable = verifyStability(size, tables, guests, tableList, assignments);

        std::cout << size << "\t" << tables << "\t" << duration.count() << "\t\t" << (isStable ? "Yes" : "No") << "\n";
    }
}

int main() {
    runPerformanceTests();
    return 0;
}
