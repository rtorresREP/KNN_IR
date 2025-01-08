#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <algorithm>
#include <cmath>
#include "libs/kTree.h"
/*
 * Este codigo permite crear el k2tree a patir de una lista de puntos que vengan en formato:
 * x y
 * una x espacio y
 * super importante: esta cuestion es mañosa.
 * no pueden haber repetidos en el arrchivo. la construcción da error cuando hay puntos repetidos.
 *
 */

using namespace std;

bool cmp(pair<int, int> &a, pair<int, int> &b) {
    if (a.second != b.second) {
        return a.second < b.second;
    }
    return a.first < b.first;
}
void imprimirCoordenadas(uint *xedges, uint *yedges, int cantidadPuntos) {
    for (int i = 0; i < cantidadPuntos; ++i) {
        std::cout <<i << "-(" << xedges[i] << ", " << yedges[i] << ")" << std::endl;
    }
}
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("%s <PATH>\n", argv[0]);
        return -1;
    }

    /* Carga de puntos */
    cout << "cargando puntos" << endl;

    string completo(argv[1]);
    completo += ".txt";
    ifstream fe(completo.c_str());
    if (!fe.is_open()) {
        printf("Error al abrir el archivo %s.txt\n", argv[1]);
        return -1;
    }
    vector<pair<int, int>> points;
    int x = 0, y = 0;
    int tamMatrix = 0;
    int cantidadPuntos = 0;
    pair<int, int> p;
    while (fe >> y >> x) { // Intenta leer y luego x, y usa la condición de salida del bucle
        if (x > tamMatrix) {
            tamMatrix = x;
        }
        if (y > tamMatrix) {
            tamMatrix = y;
        }
        p = make_pair(x + 1, y + 1);
        points.push_back(p);
    }

    fe.close();
    tamMatrix++;
    uint pot2 = 2;
    while (pot2 <= tamMatrix) {
        pot2 *= 2;
    }
    tamMatrix = pot2;
    cantidadPuntos = points.size();

    cout << "cantidad de nodos finales (tam matrix): " << tamMatrix << endl;
    cout << "cantidad de puntos: " << cantidadPuntos << endl;



    cout << "transformando a adlist" << endl;
    /* Transformacion a adlist */

    sort(points.begin(), points.end(), cmp);

    int *listNumber = new int[tamMatrix + cantidadPuntos];
    int listIndex = 0;
    int pointIndex = 0;
    for (int i = 1; i <= tamMatrix; i++) {
        listNumber[listIndex] = -i;
        listIndex++;
        if (pointIndex < cantidadPuntos) {
            while (points[pointIndex].second == i) {
                listNumber[listIndex] = points[pointIndex].first;
                listIndex++;
                pointIndex++;
            }
        }
    }
// Swap trick to release memory and destruct vector
    vector<pair<int, int>>().swap(points);

    cout << "transformando a ktree" << endl;
    /* Transformacion a ktree */

    uint max_level = floor(log(tamMatrix) / log(K));
/*    if (floor(log(tamMatrix) / log(K)) == (log(tamMatrix) / log(K))) {
        max_level = max_level - 1;
    }*/
    cout << "max level sin floor: " << (log(tamMatrix) / log(K)) << endl;

    if (max_level  == (log(tamMatrix) / log(K))) {
        max_level = max_level - 1;
    }
    cout <<"max level final is:"<< max_level << endl;

    uint nodes_read = 0;
    uint *xedges = new uint[cantidadPuntos];
    uint *yedges = new uint[cantidadPuntos];
    uint cedg = 0;
    int k = 0;
    for (int i = 0; i < cantidadPuntos + tamMatrix; i++) {
        k = listNumber[i];
        if (k < 0) {
            nodes_read++;
        } else {
            k--;
            xedges[cedg] = nodes_read - 1;
            yedges[cedg] = k;
            cedg++;
        }
    }
    //imprimirCoordenadas(xedges, yedges, cantidadPuntos);

    delete[] listNumber;
    cout << "creando el k2tree, en la clase k2tree " << endl;
    MREP *rep = compactCreateKTree(xedges, yedges, tamMatrix, cantidadPuntos, max_level);

    saveRepresentation(rep, argv[1]);
    cout << "guardado!!!! listeilor" << endl;
}