#include <iostream>
#include <fstream>
#include <vector>
#include "libs/TimeMesure.h"
#include "libs/kTree.h"
#include "libs/Count.hpp"
#include "libs/utils.h"
#include "libs/Stats.h"
#include <algorithm>
#include <set> // Para std::set
#include <utility> // Para std::pair
using namespace std;

void printHeader() {
    printf(
           "%-12s|%10s|"
           "%-15s|"
           "%-15s|%-15s|\n",
           "SET NAME", "ALGORITMO",
           "RADIO",
           "TIEMPO us", "POINTS COUNT");
}

void printStats(Stats &stats) {
    printf(""
           "%-12s|%10s|"
           "%15.01f|"
           "%15.01f|%15d|",
           stats.dataset.c_str(),  stats.algoritmo.c_str(),
           stats.step,
           stats.buildTime, stats.cantPointsCount);
    printf("\n");
}
bool comparePoints(const std::vector<std::pair<int, int>>& puntitos, uint** rangeResult, uint totalPoints) {
    // Crear un set para los puntos en rangeResult
    std::set<std::pair<int, int>> setRangeResult;
    for (uint i = 1; i <= totalPoints; ++i) {
        int x = rangeResult[0][i];
        int y = rangeResult[1][i];
        setRangeResult.insert(std::make_pair(x, y));
    }

    // Crear un set para los puntos en puntitos
    std::set<std::pair<int, int>> setPuntitos(puntitos.begin(), puntitos.end());

    // Comparar los sets
    return setRangeResult == setPuntitos;
}
int main(int argc, char *argv[]) {

    Stats resultados; //para guardar el resultado de los experimentos.

    if (argc < 4) {
        printf("%s  <PATH K2TREE> <X_1> <Y_1>  <radio>  \n",
               argv[0]);
        return -1;
    }

    Cronometer *crono = cCronometer();
    MREP *rep = loadRepresentation(argv[1]);
    Count countFunction;
    int x = atoi(argv[2]);
    int y = atoi(argv[3]);
    double rad = std::ceil(atoi(argv[4]));
    double tamMatrix =  static_cast<double>(rep->numberOfNodes);
    double time = 0.0;
    uint x1 = std::max(0.0, x - rad);
    uint x2 = std::min(x + rad, tamMatrix);
    uint y1 = std::max(0.0, y - rad);
    uint y2 = std::min(y + rad, tamMatrix );
    cout << "Dataset: "<< argv[1]<<endl;
    cout << "Tamaño de matriz: " << tamMatrix << endl;
    cout << "RANGE IN: " <<"["<< x1 <<","<< y1 <<"]" << " / " << "["<< x2 <<","<< y2 <<"]"<<endl;
    cout << "***************************************"<<endl;

    start_clock(crono);
    uint **rangeResult = compactRangeQuery(rep, x1, x2, y1, y2);
    time = (double) (stop_clock(crono) * 1000000.0);
    cout << "compactRangeQuery ORIGINAL"<< endl;
    cout <<"Time: "<< time << " Microseconds"<<endl;

    uint totalPoints = rangeResult[0][0];
//    for (uint i = 1; i <= totalPoints; ++i) {
//        int x = rangeResult[0][i];
//        int y = rangeResult[1][i];
//        cout << "(" << x << "," << y << ")" << endl;
//    }

    cout << "***************************************"<<endl;

    start_clock(crono);
    std::vector < std::pair<int, int>> puntitos = rangeQuery(rep, x1, x2, y1, y2, 0,0,1,totalPoints);
    time = (double) (stop_clock(crono) * 1000000.0);
    cout << "range query rtorres"<< endl;
    cout <<"Time: "<< time << " Microseconds"<<endl;

//    for (const auto& punto : puntitos) {
//        std::cout << "(" << punto.first << "," << punto.second << ")" << std::endl;
//    }

    cout << "***************************************"<<endl;
// Comparar los puntos
    if (comparePoints(puntitos, rangeResult, totalPoints)) {
        std::cout << "Los puntos son los mismos." << std::endl;
    } else {
        std::cout << "Fallo: Los puntos no son los mismos." << std::endl;
    }
    cout << "***************************************"<<endl;

    start_clock(crono);
    int cantidad  = countFunction.compactCount(rep, y1, y2, x1, x2);
    time = (double) (stop_clock(crono) * 1000000.0);
    cout << "COMPACT COUNT ORIGINAL"<< endl;
    cout <<"Time: "<< time << " Microseconds"<<endl;
    cout <<"Conteo: "<< cantidad << " puntos encontrados"<<endl;
    cout << "***************************************"<<endl;

    start_clock(crono);
    uint cantidad2  = compactCount(rep,0, y1, y2, x1, x2,0,rep->numberOfNodes,0,rep->numberOfNodes,0);
    time = (double) (stop_clock(crono) * 1000000.0);
    cout << "COMPACT COUNT RTORRES"<< endl;
    cout <<"Time: "<< time << " Microseconds"<<endl;
    cout <<"Conteo: "<< cantidad2 << " puntos encontrados"<<endl;


    return 0;
}
