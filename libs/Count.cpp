#include "Count.hpp"

int Count::rangeCount(MREP *rep, int x1, int x2, int y1, int y2) {
    return (int) compactRangeQuery(rep, y1, y2, x1, x2)[0][0];
}
unsigned long Count::obtenerMemoriaUtilizada() {
    if(hashMapIsEmpty()){
        memoria_utilizada = max_value_pqueue * sizeof(pair<pair<int, int>, pair<int, int>>);
    }else{
        memoria_utilizada = max_value_pqueue * sizeof(pair<pair<int, int>, pair<int, int>>);
        memoria_utilizada+= hashMapSizeInBytes() ;
    }
    return memoria_utilizada;
}

void Count::saveMaxPqueue(unsigned long num) {
    if (num > max_value_pqueue) {
        max_value_pqueue = num;
    }
}

// Matriz: pair<pair<int,int>, pair<int, int>>: <minPunt, <n (tam), i(rep en T:L)>>
int Count::compactCount(MREP *rep, int x1, int x2, int y1, int y2) {
    queue<pair<pair<int, int>, pair<int, int>>> cola;
    cola.push(make_pair(make_pair(0, 0), make_pair(rep->numberOfNodes, -1)));

    pair<pair<int, int>, pair<int, int>> n, n_i;
    int firstChild, minX, minY, nChild, iChild;
    bool isMinInside, isMaxInside, isALeaf, intersectRN;

    int total = 0;

    while (cola.size() != 0) {
        saveMaxPqueue( cola.size()); //agregado por fdo para saber cuanto ocupa..
        n = cola.front();
        cola.pop();
        firstChild = 0;
        if (n.second.second != -1) {
            firstChild = rank1(rep->btl, n.second.second) * pow(K, 2);
        }
        for (uint i = 0; i < pow(K, 2); i++) {
            if (isBitSet(rep->btl, firstChild + i)) {
                minX = n.first.first + (n.second.first / K) * (i % K);
                minY = n.first.second + (n.second.first / K) * ceil(i / K);
                nChild = n.second.first / K;
                iChild = firstChild + i;
                n_i = make_pair(make_pair(minX, minY), make_pair(nChild, iChild));

                isMinInside = x1 <= minX && y1 <= minY && minX <= x2 && minY <= y2;
                isMaxInside = x1 <= minX + nChild - 1 && y1 <= minY + nChild - 1
                              && minX + nChild - 1 <= x2 && minY + nChild - 1 <= y2;

                if (isMinInside && isMaxInside) {
                    isALeaf = false;
                    if (firstChild + i != -1) {
                        isALeaf = firstChild + i >= rep->bt_len;
                    }

                    if (!isALeaf) {
                        total += countMatrix(rep, iChild);
                    } else {
                        total++;
                    }
                } else {
                    intersectRN = !(x2 < minX || minX + nChild - 1 < x1) &&
                                  !(y2 < minY || minY + nChild - 1 < y1);
                    if (intersectRN) {
                        cola.push(n_i);
                    }
                }
            }
        }
    }

    return total;
}
int Count::compactCountOptimized(MREP *rep, int x1, int x2, int y1, int y2) {
    queue<pair<pair<int, int>, pair<int, int>>> cola;
    cola.push(make_pair(make_pair(0, 0), make_pair(rep->numberOfNodes, -1)));

    pair<pair<int, int>, pair<int, int>> n, n_i;
    int firstChild, minX, minY, nChild, iChild;
    bool isMinInside, isMaxInside, isALeaf, intersectRN;

    int total = 0;

    while (cola.size() != 0) {
        saveMaxPqueue(cola.size()); //agregado por fdo para saber cuanto ocupa..
        n = cola.front();
        cola.pop();
        firstChild = 0;
        if (n.second.second != -1) {
            firstChild = rank1(rep->btl, n.second.second) * pow(K, 2);
        }
        for (uint i = 0; i < pow(K, 2); i++) {
            if (isBitSet(rep->btl, firstChild + i)) {
                minX = n.first.first + (n.second.first / K) * (i % K);
                minY = n.first.second + (n.second.first / K) * ceil(i / K);
                nChild = n.second.first / K;
                iChild = firstChild + i;
                n_i = make_pair(make_pair(minX, minY), make_pair(nChild, iChild));

                isMinInside = x1 <= minX && y1 <= minY && minX <= x2 && minY <= y2;
                isMaxInside = x1 <= minX + nChild - 1 && y1 <= minY + nChild - 1
                              && minX + nChild - 1 <= x2 && minY + nChild - 1 <= y2;

                if (isMinInside && isMaxInside) {
                    isALeaf = false;
                    if (firstChild + i != -1) {
                        isALeaf = firstChild + i >= rep->bt_len;
                    }

                    if (!isALeaf) {
                        total += countMatrixOptimized(rep, iChild);
                    } else {
                            total++;
                    }
                } else {
                    intersectRN = !(x2 < minX || minX + nChild - 1 < x1) &&
                                  !(y2 < minY || minY + nChild - 1 < y1);
                    if (intersectRN) {
                        cola.push(n_i);
                    }
                }
            }
        }
    }

    return total;
}
int Count::countMatrix(MREP *rep, int i) {
        bool isALeafstart, isALeafend;
        int start, end;
        if (i == -1) {
            isALeafstart = false;
            isALeafend = false;
            start = 0;
            end = pow(K, 2) - 1;
            if (isBitSet(rep->btl, 0) == 0) {
                start = select1(rep->btl, rank1(rep->btl, 0) + 1);
            }
            if (isBitSet(rep->btl, end) == 0) {
                end = select1(rep->btl, rank1(rep->btl, end));
            }
        } else {
            isALeafstart = i >= rep->bt_len;
            isALeafend = i >= rep->bt_len;
            start = i;
            end = i;
        }

        while (!isALeafstart && !isALeafend) {
            start = rank1(rep->btl, start) * pow(K, 2);
            if (isBitSet(rep->btl, start) == 0) {
                start = select1(rep->btl, rank1(rep->btl, start) + 1);
            }

            end = rank1(rep->btl, end) * pow(K, 2) + pow(K, 2) - 1;
            if (isBitSet(rep->btl, end) == 0) {
                end = select1(rep->btl, rank1(rep->btl, end));
            }

            isALeafstart = start >= rep->bt_len;
            isALeafend = end >= rep->bt_len;
        }
        start--;
        int total = rank1(rep->btl, end) - rank1(rep->btl, start);
        return total;
}
int Count::countMatrixOptimized(MREP *rep, int i) {
    int cantPuntosCuadrantes = devuelveCantidadPuntosCuadrante(i);
    if (cantPuntosCuadrantes!=-1) {
        return cantPuntosCuadrantes;
    } else {
        bool isALeafstart, isALeafend;
        int start, end;
        if (i == -1) {
            isALeafstart = false;
            isALeafend = false;
            start = 0;
            end = pow(K, 2) - 1;
            if (isBitSet(rep->btl, 0) == 0) {
                start = select1(rep->btl, rank1(rep->btl, 0) + 1);
            }
            if (isBitSet(rep->btl, end) == 0) {
                end = select1(rep->btl, rank1(rep->btl, end));
            }
        } else {
            isALeafstart = i >= rep->bt_len;
            isALeafend = i >= rep->bt_len;
            start = i;
            end = i;
        }

        while (!isALeafstart && !isALeafend) {
            start = rank1(rep->btl, start) * pow(K, 2);
            if (isBitSet(rep->btl, start) == 0) {
                start = select1(rep->btl, rank1(rep->btl, start) + 1);
            }

            end = rank1(rep->btl, end) * pow(K, 2) + pow(K, 2) - 1;
            if (isBitSet(rep->btl, end) == 0) {
                end = select1(rep->btl, rank1(rep->btl, end));
            }

            isALeafstart = start >= rep->bt_len;
            isALeafend = end >= rep->bt_len;
        }

        start--;
        int total = rank1(rep->btl, end) - rank1(rep->btl, start);

        guardaCuadranteNuevo(i, total);
        return total;
    }

}

/*
 * Metodos para trabajar con la 'CACHE'
 *
 * */
bool Count::estaElCuadrante(int i) {
    // Utiliza el método find() para buscar la posición i en el hashmap
    // Si find() devuelve el iterador end(), significa que la posición i no está en el hashmap
    return (hashMap.find(i) != hashMap.end());
}
int Count::devuelveCantidadPuntosCuadrante(int i) {
// Verifica si la posición i está en el hashmap
    if (hashMap.find(i) != hashMap.end()) {
        // Si está presente, devuelve el valor asociado a la posición i
        return hashMap[i];
    }
    return -1;
}
void Count::guardaCuadranteNuevo(int i, int val) {
   // cout <<"guarda nuevo:  "<< i << " total: "<<val<<endl ;
    hashMap[i] = val;
   // cout << "hasmap size: "<< hashMap.size()<<endl;
}

