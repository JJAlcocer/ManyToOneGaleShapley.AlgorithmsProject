#include "stableMatching.h"
#include <queue>
#include <algorithm>
#include <random>
#include <numeric>

void generateRandomData(int numGuests, int numTables, int tableCapacity, std::vector<Guest>& guests, std::vector<Table>& tables) {
	std::random_device randomDevice;
	std::mt19937 generator(randomDevice());

	guests.resize(numGuests);
	tables.resize(numTables);

	std::vector<int> baseTablePrefs(numTables);
	std::iota(baseTablePrefs.begin(), baseTablePrefs.end(), 0);

	for(int i = 0; i < numGuests; ++i) {
		guests[i].tablePreferences = baseTablePrefs;
		std::shuffle(guests[i].tablePreferences.begin(), guests[i].tablePreferences.end(), generator);
		
		if (numGuests > 1) {
			std::uniform_int_distribution<int> distribution(0, numGuests - 1);
			int randomEnemy = distribution(generator);
			while (randomEnemy == i) {
				randomEnemy = distribution(generator);
			}
			guests[i].enemies.push_back(randomEnemy);
		}
	}

	std::vector<int> baseGuestPrefs(numGuests);
	std::iota(baseGuestPrefs.begin(), baseGuestPrefs.end(), 0);

	for(int j = 0; j < numTables; ++j) {
		tables[j].capacity = tableCapacity;
		tables[j].guestPreferences = baseGuestPrefs;
		std::shuffle(tables[j].guestPreferences.begin(), tables[j].guestPreferences.end(), generator);
		
		tables[j].guestRanks.resize(numGuests);
		for(int rank = 0; rank < numGuests; ++rank) {
			tables[j].guestRanks[tables[j].guestPreferences[rank]] = rank;
		}
	}
}

MatchResult runGaleShapley(int numGuests, int numTables, const std::vector<Guest>& guests, const std::vector<Table>& tables) {
	std::queue<int> unassignedGuests;
	for(int i = 0; i < numGuests; ++i) {
		unassignedGuests.push(i);
	}

	std::vector<int> guestNextProposal(numGuests, 0);
	std::vector<std::vector<int>> tableAssignments(numTables);
	long long proposalCount = 0;

	while(!unassignedGuests.empty()) {
		int currentGuest = unassignedGuests.front();
		unassignedGuests.pop();

		if(guestNextProposal[currentGuest] >= numTables) {
			continue;
		}

		int preferredTable = guests[currentGuest].tablePreferences[guestNextProposal[currentGuest]++];
		proposalCount++;
		
		bool enemyFound = false;
		int seatedEnemyIndex = -1;

		for(size_t i = 0; i < tableAssignments[preferredTable].size(); ++i) {
			int seatedGuest = tableAssignments[preferredTable][i];
			auto enemyIt = std::find(guests[currentGuest].enemies.begin(), guests[currentGuest].enemies.end(), seatedGuest);
			if(enemyIt != guests[currentGuest].enemies.end()) {
				enemyFound = true;
				seatedEnemyIndex = i;
				break;
			}
		}

		if(enemyFound) {
			int seatedEnemy = tableAssignments[preferredTable][seatedEnemyIndex];
			int currentGuestRank = tables[preferredTable].guestRanks[currentGuest];
			int enemyRank = tables[preferredTable].guestRanks[seatedEnemy];

			if(currentGuestRank < enemyRank) {
				tableAssignments[preferredTable][seatedEnemyIndex] = currentGuest;
				unassignedGuests.push(seatedEnemy);
			} else {
				unassignedGuests.push(currentGuest);
			}
		} else if(tableAssignments[preferredTable].size() < tables[preferredTable].capacity) {
			tableAssignments[preferredTable].push_back(currentGuest);
		} else {
			int worstGuestIndex = -1;
			int worstRank = -1;
			
			for(size_t i = 0; i < tableAssignments[preferredTable].size(); ++i) {
				int seatedGuest = tableAssignments[preferredTable][i];
				int rank = tables[preferredTable].guestRanks[seatedGuest];
				if (rank > worstRank) {
					worstRank = rank;
					worstGuestIndex = i;
				}
			}

			int currentGuestRank = tables[preferredTable].guestRanks[currentGuest];
			if(currentGuestRank < worstRank) {
				int rejectedGuest = tableAssignments[preferredTable][worstGuestIndex];
				tableAssignments[preferredTable][worstGuestIndex] = currentGuest;
				unassignedGuests.push(rejectedGuest);
			} else {
				unassignedGuests.push(currentGuest);
			}
		}
	}
	return {tableAssignments, proposalCount};
}

bool verifyStability(int numGuests, int numTables, const std::vector<Guest>& guests, const std::vector<Table>& tables, const std::vector<std::vector<int>>& assignments) {
	std::vector<int> guestCurrentTable(numGuests, -1);
	for(int t = 0; t < numTables; ++t) {
		for(int g : assignments[t]) {
			guestCurrentTable[g] = t;
		}
	}

	for(int g = 0; g < numGuests; ++g) {
		int currentTable = guestCurrentTable[g];
		if(currentTable == -1) continue;

		for(int preferredTable : guests[g].tablePreferences) {
			if(preferredTable == currentTable) {
				break;
			}

			int guestRankForPreferred = tables[preferredTable].guestRanks[g];
			
			if(assignments[preferredTable].size() < tables[preferredTable].capacity) {
				return false;
			}

			int worstSeatedRank = -1;
			for(int seatedGuest : assignments[preferredTable]) {
				int rank = tables[preferredTable].guestRanks[seatedGuest];
				if(rank > worstSeatedRank) {
					worstSeatedRank = rank;
				}
			}

			if(guestRankForPreferred < worstSeatedRank) {
				return false;
			}
		}
	}
	return true;
}
