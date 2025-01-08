#ifndef KNN_IRNO_HPP
#define KNN_IRNO_HPP

#include <utility>
#include <queue>
#include <cmath>
#include "Point.hpp"
#include "kTree.h"
#include "Count.hpp"

using namespace std;

// Función para calcular la distancia euclidiana entre dos puntos
double calculateDistance(Point p1, Point p2);

class knnIRNO {
public:

    knnIRNO() {}
    ~knnIRNO() {}
    vector<pair<int, int>> knn(MREP * rep, Point q, int k, double step);
    vector<pair<int, int>> getPointsinRadio(MREP *rep, int x1, int y1, int x2, int y2, int k, Point q);

    double obtenerMemoriaUtilizada();
    void setMemoriaUtilizada(size_t mem);

private:
    double memoria_utilizada;
};


#endif
