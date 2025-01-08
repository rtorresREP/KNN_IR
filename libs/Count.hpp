#ifndef COUNT_H
#define COUNT_H

#include <utility>
#include <queue>
#include <cmath>
#include <unordered_map>

#include "kTree.h"

#define K 2

using namespace std;

class Count {
public:
    Count() : max_value_pqueue(0), memoria_utilizada(0){}
    ~Count() {}

    int rangeCount(MREP *rep, int x1, int x2, int y1, int y2);
    int compactCount(MREP *rep, int x1, int x2, int y1, int y2);
    int compactCountOptimized(MREP *rep, int x1, int x2, int y1, int y2);
    bool estaElCuadrante(int i);
    int devuelveCantidadPuntosCuadrante(int i);
    void guardaCuadranteNuevo(int i, int val);
    int countMatrix(MREP *rep, int i);
    int countMatrixOptimized(MREP *rep, int i);
    unsigned long obtenerMemoriaUtilizada();
    void saveMaxPqueue(unsigned long num);
    bool hashMapIsEmpty() {
        return hashMap.size() == 0;
    }
    size_t hashMapSizeInBytes() const {
        size_t elementSize = sizeof(std::pair<int, int>); // Tamaño de cada par clave-valor
        size_t size = elementSize * hashMap.size(); // Tamaño total en bytes
        return size;
    }

private:
    unordered_map<int, int> hashMap;
    unsigned long max_value_pqueue;
    unsigned long memoria_utilizada;
};

#endif