#include "SimulatedAnnealing.h"
#include <algorithm>
#include <ctime>
#include <iostream>
#include <cmath>
#include <random>
#include <climits>

 using namespace std;

SimulatedAnnealing::SimulatedAnnealing(Adjacency_Matrix& graph, int time, double rate) {
    matrix = graph.getMatrix();
    size = matrix.size();
    timeBound = time;
    coolingRate = rate;
    temperatureBuffer = calculateTemperature();
    numVertices = graph.getNumVertices();
    std::vector<int> initialPath = greedyPath();
}

void SimulatedAnnealing::apply()
{
    int numberOfIterations=10000;
    best = greedyPath();
    vector<int> currentSolution = best;
    int bestCost = calculatePath(best);
    int currentCost=bestCost;
    double temperature = temperatureBuffer;
    double time = 0;
    double foundTime = 0;
    std::clock_t start;
    start = std::clock();
    while(true)
    {
        while (temperature > 0.1)
        {
            for(int i =0; i < numberOfIterations; i++)
            {
                vector<int> newSolution = currentSolution;
                int pos1 = rand() % numVertices;
                int pos2 = rand() % numVertices;
                swap(currentSolution[pos1], currentSolution[pos2]);
                int newCost = calculatePath(currentSolution);
                if (newCost < currentCost || exp((currentCost - newCost) / temperature) > ((double)rand() / RAND_MAX))
                {
                    currentCost = newCost;
                }
                else swap(currentSolution[pos1], currentSolution[pos2]);
                if (currentCost < bestCost)
                {
                    best = currentSolution;
                    bestCost = currentCost;
                }
            }
            temperature *=coolingRate;
            time = (std::clock() - start) / (double)CLOCKS_PER_SEC;
            if (time >= timeBound)
            {
                cout << "Droga: ";
                for (int d = 0; d < size; d++)
                {
                    cout << best[d] << " ";
                }
                cout << "\nKoszt: " << bestCost << endl;
                cout << "Znaleziono po: " << foundTime << " s " << endl;
                cout << "Temperatura koncowa: "<< temperature << endl;
                cout << endl;
                return;
            }
        }
        temperature=temperatureBuffer;
    }
}

vector<int> SimulatedAnnealing::random_permutation(int _size)
{
    std::vector<int> temp;
    temp.reserve(_size);
    for (int i = 0; i < _size; i++)
    {
        temp.push_back(i);
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(temp.begin(), temp.end(), g);
    return temp;
}

double SimulatedAnnealing::calculateTemperature()
{
    vector<int> origin;
    int firstToSwap;
    int secondToSwap;
    int delta = 0;
    int buffer = 0;
    for (int i = 0; i < 10000; i++)
    {
        do
        {
            firstToSwap = rand() % size;
            secondToSwap = rand() % size;
        } while (firstToSwap == secondToSwap);
        origin = random_permutation(size);
        vector<int> neighbour(origin);
        std::swap(neighbour[firstToSwap], neighbour[secondToSwap]);
        delta = fabs(calculatePath(origin) - calculatePath(neighbour));
        buffer += delta;
    }
    buffer /= 10000;
    return (-1*buffer)/log(0.99);
}

int SimulatedAnnealing::calculatePath(vector<int> path)
{
    int cost = 0;
    for (int i = 0; i < path.size() - 1; ++i)
    {
        cost += matrix[path[i]][path[i + 1]];
    }
    cost += matrix[path[size - 1]][path[0]];
    return cost;
}

double SimulatedAnnealing::getProbability(int diff, double temperature)
{
    return exp(diff / temperature);
}

std::vector<int> SimulatedAnnealing::greedyPath() {
    std::vector<int> path;
    std::vector<bool> visited(size, false);
    // Start from a random vertex
    int current = 0;
    path.push_back(current);
    visited[current] = true;
    int totalCost = 0;
    for (int i = 1; i < size; ++i) {
        int nearest = -1;
        int minDistance = INT_MAX;

        for (int j = 0; j < size; ++j) {
            if (!visited[j] && matrix[current][j] < minDistance) {
                nearest = j;
                minDistance = matrix[current][j];
            }
        }

        if (nearest != -1) {
            path.push_back(nearest);
            visited[nearest] = true;
            totalCost += minDistance;
            current = nearest;
        }
    }
    // Add the cost to return to the starting node
    totalCost += matrix[current][path[0]];
    // Print the greedy path and its total cost
    std::cout << "Greedy Path: ";
    for (int node : path) {
        std::cout << node << " ";
    }
    std::cout << "\nTotal Cost of Greedy Path: " << totalCost << std::endl;
    return path;
}
void SimulatedAnnealing::savePathToFile() {
    std::ofstream file("wynikiSA.txt");
    if (file.is_open()) {
        for (int node : best) {
            cout << node << " "; // Wypisanie każdego wierzchołka oddzielnie
        }
        file.close();
    } else {
        std::cerr << "Nie można otworzyć pliku do zapisu: wynikiSA.txt" << std::endl;
    }
}

SimulatedAnnealing::~SimulatedAnnealing()
{
}
