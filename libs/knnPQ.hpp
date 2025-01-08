#ifndef KNNPQFDO_H
#define KNNPQFDO_H

#include <utility>
#include <queue>
#include <cmath>
#include "iostream"
#include "kTree.h"
#include "Point.hpp"
#include <cstdint> // Agrega la inclusión para uint64_t

using namespace std;

struct knnpqElement;
class Quadrant;

class knnPQ {
public:
    knnPQ() : memoria_utilizada(0), distance_calculations(0), max_value_pqueue(0) {}
    ~knnPQ() {}

    vector<pair<int, int>> knn(MREP *rep, Point q, int k, int typeDistance);
    unsigned long obtenerMemoriaUtilizada();
    void setMemoriaUtilizada (unsigned long mem){ memoria_utilizada = mem;}
    void saveMaxPqueue( unsigned long num);
private:
    long int distance_calculations;
    unsigned long memoria_utilizada;
    unsigned long max_value_pqueue;
};

#endif
