#ifndef knnIR_HPP
#define knnIR_HPP

#include <utility>
#include <queue>
#include <cmath>
#include <vector>
#include <algorithm>
#include "Point.hpp"
#include "kTree.h"
#include "Count.hpp"
#include "basic.h"
#include "TimeMesure.h"
#include "Stats.h"

using namespace std;

class knnIR {
public:

    knnIR() : memoria_utilizada(0) {}

    ~knnIR() {}

    vector<pair<int, int>>
    knn(MREP *rep, Point q, int k, double step, int typeDistance, Stats &resultados, int typeKNN, int i);

    unsigned long obtenerMemoriaUtilizada(){return memoria_utilizada;}
    void setMemoriaUtilizada(unsigned long mem) { memoria_utilizada = mem; }

private:
    unsigned long memoria_utilizada;
};


#endif
