cmake_minimum_required(VERSION 3.26)
project(Pea2Projekt)

set(CMAKE_CXX_STANDARD 17)

add_executable(Pea2Projekt main.cpp
        adjacency_matrix.cpp
        adjacency_matrix.h
        SimulatedAnnealing.cpp
        SimulatedAnnealing.h
        TabuSearch.cpp
        TabuSearch.h
)
