#include <iostream>
#include <fstream>
#include <vector>
#include "libs/knnIR.hpp"
#include "libs/knnirNO.hpp"
#include "libs/knnPQ.hpp"
#include "libs/TimeMesure.h"
#include "libs/kTree.h"
#include "libs/utils.h"

using namespace std;


std::vector <Point> llenaQueryPoints(char *file) {
    ifstream input;
    uint32_t a, b;
    std::vector <Point> puntos;

    input.open(file);
    if (!input.is_open()) {
        cout << "ERROR: THE FILE " << file << " DOES NOT EXIST" << endl;
        exit(211);
    }
    int i = 0;
    while (input >> a >> b) {
        // cout << "consulta : "<< a<<" /" << b<<endl;
        Point nuevoPunto;
        nuevoPunto.x = a;
        nuevoPunto.y = b;
        puntos.push_back(nuevoPunto);
    }
    input.close();

    return puntos;
}

void printHeader() {
    printf(
           "%-12s|%10s|"
           "%-10s|%-6s|"
           "%-15s|%-15s|"
           "%-15s|%-15s|"
           "%-15s|%-15s|"
           "%-15s|%-15s|"
           "%-15s|%-15s|"
           "%15s\n",
           "SET NAME", "STEP",
           "ALGORITMO", "K",
           "TIME COUNT", "TIME RANGE",
           "TIEMPO us", "POINTS COUNT",
           "CANT VUELTAS","MEMORIA Bytes",
           "RADIO INICIAL", "RADIO FINAL",
           "min_m","distance",
           "puntos_init"
           );
}

void printStats(Stats &stats) {
    printf(""
           "%-12s|%10.1f|"
           "%-10s|%6d|"
           "%15.01f|%15.01f|"
           "%15.01f|%15d|"
           "%15d|%15.0ld|"
           "%15.01f|%15.01f|"
           "%15.01f|%15s| %15d",
           stats.dataset.c_str(), stats.step,
           stats.algoritmo.c_str(), stats.k,
           stats.countTime, stats.rangeTime,
           stats.buildTime, stats.cantPointsCount,
           stats.cantVueltasCount, stats.peakMemory,
           stats.radioInicial, stats.radioFinal,
           stats.min_m,stats.typeDistance.c_str(),stats.cantidad_puntos_init);
    printf("\n");
}

int main(int argc, char *argv[]) {

    Stats resultados; //para guardar el resultado de los experimentos.

    if (argc < 3) {
        printf("%s  <PATH K2TREE> <X_1> <Y_1> <K> <algorithm 1=knnpq, 2=IR-FULL, 3=IR-MED, 4=IR-NO> <step for IR> <TYPE OF DISTANCE 1 EUCLID, 2 MANHATTAN> \n",
               argv[0]);
        return -1;
    }
    Cronometer *crono = cCronometer();
    MREP *rep = loadRepresentation(argv[1]);

    int x = atoi(argv[2]);
    int y = atoi(argv[3]);
    int k = atoi(argv[4]);
    int algo = atoi(argv[5]);
    double step = atof(argv[6]);
    int typeDistance = atoi(argv[7]);

    if (typeDistance == 1) {
        resultados.typeDistance = "euclidean";
    } else {
        resultados.typeDistance = "manhattan";
    }

    double time = 0.0;
    switch (algo) {
        case 1:  //PRIORITY QUEUE ALGORITHM
        {
            knnPQ knn;
            start_clock(crono);
            Point q;
            q.x = x;
            q.y = y;
            knn.knn(rep, q, k, typeDistance);
            time = (double) (stop_clock(crono) * 1000000.0);
            resultados.peakMemory = knn.obtenerMemoriaUtilizada();
            resultados.algoritmo = "PQ";
            resultados.buildTime = time;
            resultados.countTime = 0.0;
            resultados.cantPointsCount = 0;
            resultados.cantVueltasCount = 0;
            resultados.rangeTime = 0;
            break;
        }
        case 2: // IR FULL OPTIMIZATION
        {
            knnIR knn;
            Point q;
            q.x = x;
            q.y = y;
            knn.knn(rep, q, k, step, typeDistance, resultados,2,50);
            resultados.algoritmo = "IR_FULL";
            break;
        }

        case 3: // IR MED OPTIMIZATION
        {
            knnIR knn;
            Point q;
            q.x = x;
            q.y = y;
            knn.knn(rep, q, k, step, typeDistance, resultados,1,50);
            resultados.algoritmo = "IR_MED";
            break;
        }

        default: {
            std::cout << "Opción del algoritmo no reconocida" << std::endl;
            exit(0);
            break;
        }

    }


    resultados.k = k;
    resultados.dataset = argv[1];
    resultados.step = step;


    char *homedir = getenv("HOME");
    strcat(homedir, "/RESULTADOS/resultKNN.txt");
    if (checkFileExist(homedir) == 1 && fileIsEmpty(homedir) == 1) {
        printHeader();
        fflush(stdout);
    }
    printStats(resultados);

    return 0;
}
