#include "knnirNO.hpp"
#include "basic.h"
#include "kTree.h"
#include <unistd.h>
#include "bitrankw32int.h"
#include "Count.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;
double knnIRNO::obtenerMemoriaUtilizada() {
    return memoria_utilizada;
}

void knnIRNO:: setMemoriaUtilizada(size_t mem){
    memoria_utilizada = mem;
}

double calculateDistance(Point p1, Point p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return std::sqrt(dx * dx + dy * dy);
}


vector<pair<int, int>> knnIRNO::knn(MREP *rep, Point q, int k, double  step) {
    //   Cronometer *cronox = cCronometer();
    /*transform*/

    vector <pair<int, int>> C;
    double epsilon = 0.0;
    int min_m = 0;
    Count countFunction;
    double tamMatrix =  static_cast<double>(rep->numberOfNodes);

    /* encontrar minimo . MINIMUM RADIUS IN A GRID TO OBTAIN K POINTS*/
    min_m =   std::ceil((-1 + std::sqrt(1 + 4 * k / 4)) / 2);

    epsilon = static_cast<double>(min_m);
    double a = std::sqrt(2);
    double r = epsilon ;
    double rad = std::ceil(r);

    uint x1 = std::max(0.0, q.x - rad);
    uint x2 = std::min(q.x + rad, tamMatrix);
    uint y1 = std::max(0.0, q.y - rad);
    uint y2 = std::min(q.y + rad, tamMatrix );

    while ((countFunction.compactCount(rep, y1, y2, x1, x2)) < k) {
        r = r * step;
        rad = std::ceil(r);
        x1 = std::max(0.0, q.x - rad);
        x2 = std::min(q.x + rad, tamMatrix);
        y1 = std::max(0.0, q.y - rad);
        y2 = std::min (q.y + rad, tamMatrix);
    }

    // Ajuste final del radio
    double radioFinal = r * a;
    r = std::ceil(radioFinal);

    uint p1 = std::max(0.0, q.x - r);
    uint p2 = std::min(q.x + r, tamMatrix);
    uint q1 = std::max(0.0, q.y - r);
    uint q2 = std::min(q.y + r, tamMatrix);

    uint **rangeResult = compactRangeQuery(rep, p1, p2, q1, q2);

    uint totalPoints = rangeResult[0][0];
    vector<pair<int, int>> rangePoints;

    for (uint i = 1; i <= totalPoints; ++i) {
        int x = rangeResult[0][i];
        int y = rangeResult[1][i];
        rangePoints.push_back(make_pair(x, y));
    }
  /*  for (pair<int, int> point: rangePoints) {
        cout << "(" << point.second << "," << point.first << ")";
    }*/
    // Calcular la distancia y ordenar los puntos
    sort(rangePoints.begin(), rangePoints.end(), [q](const pair<int, int> &point1, const pair<int, int> &point2) {
        Point p1 = {static_cast<double>(point1.first), static_cast<double>(point1.second)};
        Point p2 = {static_cast<double>(point2.first), static_cast<double>(point2.second)};
        return calculateDistance(p1, q) < calculateDistance(p2, q);
    });

    // Limitar el número de puntos devueltos según el parámetro k
    if (rangePoints.size() > static_cast<size_t>(k)) {
        rangePoints.resize(k);
    }
   // setMemoriaUtilizada(countFunction.getMemoryUsed());
    return rangePoints;
}

