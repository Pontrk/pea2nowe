#include <vector>
#include "adjacency_matrix.h" // Include the Adjacency_Matrix header

class TabuSearch
{
private:
    std::vector<std::vector<int>> matrix; // Use a 2D vector for the matrix
    int size = 0;
    int searchTime = 0;

    std::vector<int> randomPermutation(int _size);
    int calculatePath(std::vector<int> path);

public:
    void apply();
    TabuSearch(Adjacency_Matrix graph, int time); // Constructor now takes Adjacency_Matrix
    ~TabuSearch();
};
