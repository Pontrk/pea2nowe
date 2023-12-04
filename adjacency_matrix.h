#ifndef PEA1_ADJACENCY_MATRIX_H
#define PEA1_ADJACENCY_MATRIX_H
#pragma once

#include <math.h>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

class Adjacency_Matrix {
public:
    Adjacency_Matrix();
    void printMatrix();
    void loadFromFile(const string& filename);
    vector<vector<int>> getMatrix();
    void generate(int numberOfNodes);
    int getCount();
private:
    vector<vector<int>> matrix;
    int liczbaWierzcholkow;
};


#endif //PEA1_ADJACENCY_MATRIX_H
