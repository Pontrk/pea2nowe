#ifndef SIMULATED_ANNEALING_H
#define SIMULATED_ANNEALING_H

#include "adjacency_matrix.h"
#include <vector>

class SimulatedAnnealing {
public:
    SimulatedAnnealing(Adjacency_Matrix& graf, int time, double rate);
    ~SimulatedAnnealing();
    void apply();


private:
    std::vector<std::vector<int>> matrix;
    int size;
    int timeBound;
    double coolingRate;
    double temperatureBuffer;
    int numVertices;

    std::vector<int> greedyPath();
    std::vector<int> random_permutation(int _size);
    double calculateTemperature();
    int calculatePath(std::vector<int> path);
    double getProbability(int diff, double temperature);

};

#endif // SIMULATED_ANNEALING_H