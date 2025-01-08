//
// Created by fernando on 09-04-24.
//

#ifndef KNN_SOLUTIONS_K2TREE_MAIN_STATS_H
#define KNN_SOLUTIONS_K2TREE_MAIN_STATS_H
#include <string>

typedef struct Stats {
    std::string dataset;
    int k;
    double countTime;
    double rangeTime;
    int cantPointsCount;
    int cantVueltasCount;
    double buildTime;
    unsigned long peakMemory;
    std::string typeDistance;
    std::string algoritmo;
    float step;
    double radioInicial;
    double radioFinal;
    double min_m;
    int  JUMP_FACTOR;
    int start_radio;
    int cantidad_puntos_init;
    // Constructor que inicializa todos los miembros en 0
    Stats() : dataset(""), k(0), countTime(0.0), rangeTime(0.0),
              cantPointsCount(0), cantVueltasCount(0), buildTime(0.0),
              peakMemory(0), typeDistance(""), algoritmo(""), step(0.0),
              radioInicial(0.0), radioFinal(0.0), min_m(0.0), cantidad_puntos_init(0), JUMP_FACTOR(0), start_radio(0){}
} Stats;

#endif //KNN_SOLUTIONS_K2TREE_MAIN_STATS_H
