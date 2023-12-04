#include <iostream>
#include "adjacency_matrix.h"
#include "SimulatedAnnealing.h"
#include "TabuSearch.h"


int main() {
    Adjacency_Matrix graf;
    string dane;
    int searchTime=120;
    srand(static_cast<unsigned int>(time(nullptr)));
    // Create an instance of TabuSearch with the adjacency matrix and search time
    double coolingRate=0.99;
    int opcja;
    do {
        cout << "Wybierz opcje:" << endl;
        cout << "1. Wczytaj dane z pliku" << endl;
        cout << "2. Wyprowadz kryterium stopu" << endl;
        cout << "3. TabuSearch" << endl;
        cout << "4. Wprowadz wspolczynnik temperatury" << endl;
        cout << "5. Algorytm Helda-Karpa "<< endl;
        cout << "0. Zakoncz program" << endl;
        cin >> opcja;
        switch (opcja) {
            case 1: {
                cout<<"podaj nazwe pliku: ";
                cin>>dane;
                graf.loadFromFile(dane);
                break;
            }
            case 2: {
                cout<<"Podaj czas po jakim algorytm ma sie zatrzymac(w sekundach)"<<endl;
                cin>>searchTime;
                break;
            }
            case 3: {
                TabuSearch tabuSearch(graf, searchTime);
                tabuSearch.apply();
                break;
            }
            case 4: {
                cout<<"Podaj wspolczynnik temperatury (proponowane 0.99)"<<endl;
                cin>>coolingRate;
                break;
            }
            case 5: {
                SimulatedAnnealing sa(graf, searchTime, coolingRate);
                sa.apply();
                break;
            }
            case 6: {
                ofstream wyniki("wyniki.txt");
                if(wyniki.is_open())
                    cout<<"otwarto"<<endl;
                wyniki.close();
            }
        }

    } while (opcja != 0);

    return 0;
}
