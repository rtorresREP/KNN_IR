#include <iostream>

#include "libs//knnPQ.hpp"
#include "libs/knnIR.hpp"
#include "libs/knnirNO.hpp"
#include "libs/kTree.h"
#include "libs/TimeMesure.h"
#include "libs/Stats.h"

using namespace std;
double calculateDist(Point p1, Point p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return std::sqrt(dx * dx + dy * dy);
}
void showPoints(const std::vector<std::pair<int, int>>& puntillos) {
    for (const auto& point : puntillos) {
        std::cout << "(" << point.first << "," << point.second << ")";
    }
    std::cout << std::endl;
}
int main(int argc, char *argv[]) {

    if (argc < 5) {
        printf("%s <PATH> <X_1> <Y_1> <K>\n", argv[0]);
        return -1;
    }
    Stats resultados; //para guardar el resultado de los experimentos.
    Point q;
    q.x = std::stod(argv[2]);
    q.y = std::stod(argv[3]);
    int k = atoi(argv[4]);

    MREP *rep = loadRepresentation(argv[1]);
    cout << "Tamaño de matriz: " << rep->numberOfNodes << endl;
    cout << "Cantidad de puntos: " << rep->numberOfEdges << endl;
    cout << "Profundidad del k2tree: " << rep->maxLevel << endl;
    cout << "puntos del k2tree" << endl;
    double densidad = static_cast<double>(rep->numberOfEdges) / ( static_cast<double>( rep->numberOfNodes) *  static_cast<double>( rep->numberOfNodes) );
    cout << "Densidad: " << densidad<<endl;
    int tamMatrix  =   static_cast<int> (rep->numberOfNodes);
    std::vector <pair<int, int>> puntillos;
    uint **rangeResult = compactRangeQuery(rep, 0, tamMatrix, 0, tamMatrix);
    uint totalPoints = rangeResult[0][0];

    cout << "Punto de consulta q es: (" << q.x << "," << q.y << ")" << endl;

 for (uint i = 1; i <= totalPoints; ++i) {
        int x = rangeResult[0][i];
        int y = rangeResult[1][i];
        Point aux;
        aux.x = x;
        aux.y = y;
        cout << " (" << x << "," << y << ")" << "dist: "<< calculateDist(q,aux)<< endl;
    }

    knnPQ pq;
    knnIR knn;
    knnIRNO knnNO;
    int typeDistance = 1; //euclidean


    cout << "Usando la Distancia: " << (typeDistance == 1 ? "Euclidiana" : "Manhattan") << endl;
    auto pointsPQ = pq.knn(rep, q, k, typeDistance);
    auto pointsIRMED = knn.knn(rep, q, k, 2.5, typeDistance, resultados, 1, 1);
   // auto pointsFULL = knn.knn(rep, q, k, 1.1, typeDistance, resultados, 2 ,1);
    //auto pointsNO = knnNO.knn(rep, q, k, 1.1);

    cout << "PUNTOS PARA KNN PQ: ";
    showPoints(pointsPQ);

   //cout << "PUNTOS PARA KNN IR FULL: ";
  //  showPoints(pointsFULL);

    cout << "PUNTOS PARA KNN IR MED: ";
   showPoints(pointsIRMED);

  /*  cout << "PUNTOS PARA KNN IR NO: ";
    for (pair<int, int> point: pointsNO) {
        cout << "(" << point.first << "," << point.second << ")";
    }
    cout << endl;*/
    return 0;
}
