#include "TabuSearch.h"
#include <time.h>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <random>

// Konstruktor klasy TabuSearch.
TabuSearch::TabuSearch(Adjacency_Matrix graph, int time) {
    matrix = graph.getMatrix(); // Pobranie macierzy sąsiedztwa z grafu.
    size = graph.getNumVertices(); // Ustalenie liczby wierzchołków.
    searchTime = time; // Ustawienie maksymalnego czasu działania.
}

// Główna metoda algorytmu apply.
void TabuSearch::apply() {
    vector<vector<int>> tabuMatrix; // Tablica tabu do śledzenia niedozwolonych ruchów.
    vector<int> best = greedyPath(); // Ustalenie początkowej ścieżki metodą zachłanną.
    vector<int> permutation = randomPermutation(size); // Losowa permutacja wierzchołków.
    vector<int> next(permutation); // Kolejna permutacja do rozważenia.
    int result = calculatePath(best); // Koszt najlepszej ścieżki.
    int firstToSwap, secondToSwap; // Indeksy wierzchołków do zamiany.
    int nextCost; // Koszt kolejnej permutacji.
    std::clock_t start; // Początek pomiaru czasu.
    double time; // Czas działania algorytmu.
    double foundTime; // Czas znalezienia najlepszego rozwiązania.
    tabuMatrix.resize(size); // Inicjalizacja tablicy tabu.
    for (int j = 0; j < size; j++) {
        tabuMatrix[j].resize(size, 0); // Ustawienie początkowych wartości tablicy tabu.
    }
    const double iterationTimeLimit = 30.0; // Limit czasu na iterację w sekundach.
    start = std::clock();

    // Główna pętla algorytmu.
    while (true) {
        double iterationStart = std::clock(); // Początek iteracji.
        for (int step = 0; step < 15 * size; step++) {
            firstToSwap = 0;
            secondToSwap = 0;
            nextCost = INT_MAX;
            // Przeszukiwanie wszystkich par wierzchołków do zamiany.
            for (int first = 0; first < size; first++) {
                for (int second = first + 1; second < size; second++) {
                    std::swap(permutation[first], permutation[second]); // Zamiana wierzchołków.
                    int currentCost = calculatePath(permutation); // Obliczenie kosztu permutacji.

                    // Aktualizacja najlepszego wyniku i permutacji.
                    if (currentCost < result) {
                        result = currentCost;
                        best = permutation;
                        foundTime = (std::clock() - start) / (double)CLOCKS_PER_SEC;
                    }

                    // Sprawdzenie, czy aktualna permutacja jest lepsza od następnej.
                    // Sprawdzenie, czy ruch jest dozwolony (nie znajduje się na liście tabu).
                    if (currentCost < nextCost && tabuMatrix[first][second] < step) {
                        nextCost = currentCost;
                        next = permutation;
                        firstToSwap = first;
                        secondToSwap = second;
                    }
                    std::swap(permutation[first], permutation[second]); // Przywrócenie poprzedniego stanu.
                    time = (std::clock() - start) / (double)CLOCKS_PER_SEC;

                    // Sprawdzenie warunku zakończenia.
                    if (time >= searchTime) {
                        std::ofstream file("wynikiTS.txt");
                        cout << "Droga: ";
                        for (int d = 0; d < size; d++) {
                            cout << best[d] << " ";
                            file << best[d] << " "; // Zapis do pliku.
                        }
                        cout << "\nKoszt: " << result << endl;
                        cout << "Znaleziono po: " << foundTime << " s " << endl;
                        return; // Zakończenie algorytmu.
                    }
                }
            }

            // Aktualizacja tablicy tabu.
            // Zwiększenie wartości tabu dla ostatnio wykonanego ruchu.
            // Zapobiega to powtórzeniu tego samego ruchu w najbliższej przyszłości.
            permutation = next;
            tabuMatrix[firstToSwap][secondToSwap] += size;

            // Sprawdzenie, czy czas iteracji nie przekroczył limitu.
            double currentTime = (std::clock() - iterationStart) / (double)CLOCKS_PER_SEC;
            if (currentTime >= iterationTimeLimit) {
                break; // Przerwanie obecnej iteracji, jeśli limit czasu został przekroczony.
            }
        }

        // Resetowanie tablicy tabu i generowanie nowej losowej permutacji.
        permutation = randomPermutation(size);
        for (auto &row : tabuMatrix) {
            std::fill(row.begin(), row.end(), 0); // Resetowanie tablicy tabu.
        }
    }
}

// Generuje losową permutację wierzchołków grafu.
vector<int> TabuSearch::randomPermutation(int _size) {
    std::vector<int> temp;
    temp.reserve(_size);
    for (int i = 0; i < _size; i++) {
        temp.push_back(i); // Dodanie każdego wierzchołka do permutacji.
    }
    std::random_device rd; // Inicjalizacja generatora liczb losowych.
    std::mt19937 g(rd());  // Generator liczb losowych.
    std::shuffle(temp.begin(), temp.end(), g); // Mieszanie permutacji.
    return temp;
}

// Oblicza koszt danej ścieżki w grafie.
int TabuSearch::calculatePath(std::vector<int> path) {
    int cost = 0;
    for (int i = 0; i < path.size() - 1; i++) {
        cost += matrix[path[i]][path[i + 1]]; // Dodawanie kosztu krawędzi.
    }
    cost += matrix[path[path.size() - 1]][path[0]]; // Dodanie kosztu powrotu do punktu startowego.
    return cost;
}

// Generuje początkową ścieżkę metodą zachłanną.
std::vector<int> TabuSearch::greedyPath() {
    std::vector<int> path;
    std::vector<bool> visited(size, false);
    int current = 0; // Start z pierwszego wierzchołka.
    path.push_back(current);
    visited[current] = true;
    int totalCost = 0;
    for (int i = 1; i < size; i++) {
        int nearest = -1;
        int minDistance = INT_MAX;
        // Szukanie najbliższego nieodwiedzonego wierzchołka.
        for (int j = 0; j < size; j++) {
            if (!visited[j] && matrix[current][j] < minDistance) {
                nearest = j;
                minDistance = matrix[current][j];
            }
        }
        // Dodanie wierzchołka do ścieżki.
        if (nearest != -1) {
            path.push_back(nearest);
            visited[nearest] = true;
            totalCost += minDistance;
            current = nearest;
        }
    }
    totalCost += matrix[current][path[0]]; // Dodanie kosztu powrotu do startu.
    return path;
}

TabuSearch::~TabuSearch()
{
}
