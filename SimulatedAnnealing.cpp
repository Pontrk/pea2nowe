#include "SimulatedAnnealing.h"
#include <algorithm>
#include <ctime>
#include <iostream>
#include <cmath>
#include <random>
#include <climits>

using namespace std;

// Konstruktor klasy SimulatedAnnealing.
SimulatedAnnealing::SimulatedAnnealing(Adjacency_Matrix& graph, int time, double rate) {
    matrix = graph.getMatrix(); // Pobranie macierzy sąsiedztwa z grafu.
    size = matrix.size();       // Ustalenie liczby wierzchołków.
    timeBound = time;           // Ustawienie maksymalnego czasu działania.
    coolingRate = rate;         // Ustawienie współczynnika schładzania.
    temperatureBuffer = calculateTemperature(); // Obliczenie początkowej temperatury.
    numVertices = graph.getNumVertices();       // Ustalenie liczby wierzchołków.
    std::vector<int> initialPath = greedyPath(); // Wygenerowanie początkowej ścieżki.
}

// Główna metoda algorytmu apply.
void SimulatedAnnealing::apply() {
    int numberOfIterations = 1000; // Liczba iteracji na każdym poziomie temperatury.
    best = greedyPath();            // Ustalenie początkowej ścieżki.
    vector<int> currentSolution = best; // Aktualna rozpatrywana ścieżka.
    int bestCost = calculatePath(best); // Koszt najlepszej ścieżki.
    int currentCost = bestCost;         // Koszt aktualnej ścieżki.
    double temperature = temperatureBuffer; // Aktualna temperatura.
    double time = 0;                       // Czas działania algorytmu.
    double foundTime = 0;                  // Czas znalezienia najlepszego rozwiązania.
    std::clock_t start;                    // Początek pomiaru czasu.
    start = std::clock();

    // Główna pętla algorytmu.
    while (true) {
        while (temperature > 0.1) { // Dopóki temperatura jest wystarczająco wysoka.
            for (int i = 0; i < numberOfIterations; i++) {
                // Generowanie nowego rozwiązania.
                vector<int> newSolution = currentSolution;
                int pos1 = rand() % numVertices;
                int pos2 = rand() % numVertices;
                swap(currentSolution[pos1], currentSolution[pos2]);
                int newCost = calculatePath(currentSolution);

                // Decyzja o akceptacji nowego rozwiązania.
                if (newCost < currentCost || exp((currentCost - newCost) / temperature) > ((double)rand() / RAND_MAX)) {
                    currentCost = newCost;
                } else {
                    swap(currentSolution[pos1], currentSolution[pos2]); // Przywrócenie poprzedniego stanu.
                }

                // Aktualizacja najlepszego rozwiązania.
                if (currentCost < bestCost) {
                    best = currentSolution;
                    bestCost = currentCost;
                }
            }

            // Obniżanie temperatury.
            temperature *= coolingRate;
            time = (std::clock() - start) / (double)CLOCKS_PER_SEC; // Aktualizacja czasu działania.

            // Sprawdzenie warunku zakończenia.
            if (time >= timeBound) {
                cout << "Droga: ";
                std::ofstream file("wynikiSA.txt");
                for (int d = 0; d < size; d++) {
                    cout << best[d] << " ";
                    file << best[d] << " "; // Zapis do pliku.
                }
                cout << "\nKoszt: " << bestCost << endl;
                cout << "Znaleziono po: " << foundTime << " s " << endl;
                cout << "Temperatura koncowa: " << temperature << endl;
                cout << "Wartosc wyrazenia e^(-1/Tk): " << exp(-1/temperature);
                cout << endl;
                return; // Zakończenie algorytmu.
            }
        }
        temperature = temperatureBuffer; // Resetowanie temperatury.
    }
}

// Generuje losową permutację wierzchołków grafu.
std::vector<int> SimulatedAnnealing::random_permutation(int _size) {
    std::vector<int> temp;
    temp.reserve(_size);
    for (int i = 0; i < _size; i++) {
        temp.push_back(i);
    }
    std::random_device rd; // Inicjalizacja generatora liczb losowych.
    std::mt19937 g(rd());  // Generator liczb losowych.
    std::shuffle(temp.begin(), temp.end(), g); // Mieszanie permutacji.
    return temp;
}

// Oblicza początkową temperaturę dla algorytmu Symulowanego Wyżarzania.
double SimulatedAnnealing::calculateTemperature() {
    vector<int> origin; // Wektor przechowujący oryginalną permutację wierzchołków.
    int firstToSwap, secondToSwap; // Indeksy wierzchołków do zamiany.
    int delta = 0, buffer = 0; // Zmienne do obliczania średniej różnicy kosztów.

    // Pętla wykonująca się określoną liczbę razy do obliczenia średniej różnicy kosztów.
    for (int i = 0; i < 10000; i++) {
        // Losowe wybieranie dwóch różnych wierzchołków do zamiany.
        do {
            firstToSwap = rand() % size;
            secondToSwap = rand() % size;
        } while (firstToSwap == secondToSwap); // Zapewnienie, że wierzchołki są różne.

        origin = random_permutation(size); // Generowanie losowej permutacji wierzchołków.
        vector<int> neighbour(origin); // Tworzenie kopii permutacji.
        std::swap(neighbour[firstToSwap], neighbour[secondToSwap]); // Zamiana wierzchołków.

        // Obliczanie różnicy kosztów między oryginalną a zmodyfikowaną permutacją.
        delta = fabs(calculatePath(origin) - calculatePath(neighbour));
        buffer += delta; // Dodawanie różnicy do bufora.
    }

    buffer /= 10000; // Obliczenie średniej różnicy kosztów.

    // Obliczenie początkowej temperatury na podstawie średniej różnicy kosztów.
    // Używamy wzoru z teorii Symulowanego Wyżarzania.
    return (-1 * buffer) / log(0.99);
}

// Oblicza koszt danej ścieżki w grafie.
int SimulatedAnnealing::calculatePath(vector<int> path) {
    int cost = 0;
    for (int i = 0; i < path.size() - 1; ++i) {
        cost += matrix[path[i]][path[i + 1]]; // Dodawanie kosztu krawędzi.
    }
    cost += matrix[path[size - 1]][path[0]]; // Dodanie kosztu powrotu do punktu startowego.
    return cost;
}


// Generuje początkową ścieżkę metodą zachłanną.
std::vector<int> SimulatedAnnealing::greedyPath() {
    std::vector<int> path;
    std::vector<bool> visited(size, false);
    int current = 0; // Start z pierwszego wierzchołka.
    path.push_back(current);
    visited[current] = true;
    int totalCost = 0;
    for (int i = 1; i < size; ++i) {
        int nearest = -1;
        int minDistance = INT_MAX;
        // Szukanie najbliższego nieodwiedzonego wierzchołka.
        for (int j = 0; j < size; ++j) {
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
    // Print the greedy path and its total cost
    std::cout << "Greedy Path: ";
    for (int node : path) {
        std::cout << node << " ";
    }
    std::cout << "\nTotal Cost of Greedy Path: " << totalCost << std::endl;
    return path;
}

// Zapisuje najlepszą ścieżkę do pliku.
void SimulatedAnnealing::savePathToFile() {
    std::ofstream file("wynikiSA.txt");
    if (file.is_open()) {
        for (int node : best) {
            file << node << " "; // Zapis każdego wierzchołka do pliku.
        }
        file.close();
    } else {
        std::cerr << "Nie można otworzyć pliku do zapisu: wynikiSA.txt" << std::endl;
    }
}

// Wczytuje ścieżkę z pliku i zwraca ją jako wektor.
std::vector<int> SimulatedAnnealing::loadPathFromFile(const std::string& filename) {
    std::vector<int> path;
    std::ifstream file(filename);
    int node;
    if (file.is_open()) {
        while (file >> node) {
            path.push_back(node); // Wczytywanie wierzchołków do ścieżki.
        }
        file.close();
    } else {
        std::cerr << "Nie można otworzyć pliku do odczytu: " << filename << std::endl;
    }
    return path;
}
SimulatedAnnealing::~SimulatedAnnealing()
{
}
