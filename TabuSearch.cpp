#include "TabuSearch.h"
#include <time.h>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <random>

void TabuSearch::apply()
{
    vector<vector<int>> tabuMatrix;
    vector<int> best=greedyPath();
    vector<int> permutation = randomPermutation(size);
    vector<int> next(permutation);
    int result = INT_MAX;
    int firstToSwap;
    int secondToSwap;
    int nextCost;
    std::clock_t start;
    double time;
    double foundTime;
    tabuMatrix.resize(size);
    for (int j = 0; j < size; ++j)
    {
        tabuMatrix[j].resize(size, 0);
    }
    start = std::clock();
    while(true)
    {
        for (int step = 0; step < 15 * size; ++step)
        {
            firstToSwap = 0;
            secondToSwap = 0;
            nextCost = INT_MAX;

            for (int first = 0; first < size; ++first)
            {
                for (int second = first + 1; second < size; ++second)
                {
                    std::swap(permutation[first], permutation[second]);
                    int currentCost = calculatePath(permutation);
                    if (currentCost < result)
                    {
                        result = currentCost;
                        best = permutation;
                        foundTime = (std::clock() - start) / (double)CLOCKS_PER_SEC;
                    }
                    if (currentCost < nextCost)
                    {
                        if (tabuMatrix[second][first] < step)
                        {
                            nextCost = currentCost;
                            next = permutation;

                            firstToSwap = second;
                            secondToSwap = first;
                        }
                    }
                    std::swap(permutation[first], permutation[second]);

                    time = (std::clock() - start) / (double)CLOCKS_PER_SEC;

                    if (time >= searchTime)
                    {
                        cout << "Droga: ";
                        for (int d = 0; d < size; d++)
                        {
                            cout << best[d] << " ";
                        }
                        cout << "\nKoszt: " << result << endl;
                        cout << "Znaleziono po: " << foundTime << " s " << endl;
                        return;
                    }
                }
            }
            permutation = next;
            tabuMatrix[firstToSwap][secondToSwap] += size;
        }
        permutation = randomPermutation(size);
        for (auto &row : tabuMatrix)
        {
            std::fill(row.begin(), row.end(), 0);
        }
    }
}

vector<int> TabuSearch::randomPermutation(int _size)
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

int TabuSearch::calculatePath(std::vector<int> path)
{
    int cost = 0;
    for (int i = 0; i < path.size() - 1; ++i)
    {
        cost += matrix[path[i]][path[i + 1]];
    }
    cost += matrix[path[path.size() - 1]][path[0]];
    return cost;
}

TabuSearch::TabuSearch(Adjacency_Matrix graph, int time)
{
    matrix = graph.getMatrix(); // Get the matrix from Adjacency_Matrix
    size = graph.getCount();    // Get the size (number of vertices)
    searchTime = time;
}
std::vector<int> TabuSearch::greedyPath() {
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

TabuSearch::~TabuSearch()
{
}
