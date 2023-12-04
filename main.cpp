#include <iostream>
#include "adjacency_matrix.h"
#include "SimulatedAnnealing.h"
#include "TabuSearch.h"


int main() {
    Adjacency_Matrix graf;
    graf.loadFromFile("ftv55.atsp");
    srand(static_cast<unsigned int>(time(nullptr)));
    // Define the search time for Tabu Search
    int searchTime = 60; // For example, 60 seconds

    // Create an instance of TabuSearch with the adjacency matrix and search time
    TabuSearch tabuSearch(graf, searchTime);
    // Apply the Tabu Search algorithm
    tabuSearch.apply();
    return 0;
}
