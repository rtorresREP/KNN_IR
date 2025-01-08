#include <iostream>
#include <fstream>
#include <vector>
#include "libs/knnIR.hpp"
#include "libs/knnirNO.hpp"
#include "libs/knnPQ.hpp"
#include "libs/TimeMesure.h"
#include "libs/utils.h"
#include "libs/kTree.h"
#include "libs/Stats.h"

using namespace std;

string extractFilename(const string &path) {
    size_t pos = path.find_last_of('/');
    if (pos == string::npos) {
        return path; // Path doesn't contain a '/' separator
    } else {
        return path.substr(pos + 1); // Extract filename from position after '/'
    }
}

std::vector<Point> llenaQueryPoints(char *file) {
    ifstream input;
    uint32_t a, b;
    std::vector<Point> puntos;

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
            "%-8s|%-5s|"
            "%-9s|%-6s|"
            "%-15s|%-15s|"
            "%-15s|%-15s|"
            "%-15s|%-15s|"
            "%-15s|%-15s|"
            "%-15s|%10s|%-9s|"
            "%-15s|"
            "%-4s|\n",
            "SET NAME", "STEP",
            "ALGORITMO", "K",
            "TIME COUNT", "TIME RANGE",
            "TIEMPO us", "POINTS COUNT",
            "CANT VUELTAS", "MEMORIA Bytes",
            "RADIO INICIAL", "RADIO FINAL",
            "min_m", "START_RADIO","distance",
            "puntos_init",
            "inc"
    );
}

void printStats(Stats &stats) {
    printf(""
           "%-8s|%5.1f|"
           "%-9s|%6d|"
           "%15.01f|%15.01f|"
           "%15.01f|%15d|"
           "%15d|%15lu|"
           "%15.01f|%15.01f|"
           "%15.01f|%10d|%9s|%15d|%4d|",
           stats.dataset.c_str(), stats.step,
           stats.algoritmo.c_str(), stats.k,
           stats.countTime, stats.rangeTime,
           stats.buildTime, stats.cantPointsCount,
           stats.cantVueltasCount, stats.peakMemory,
           stats.radioInicial, stats.radioFinal,
           stats.min_m, stats.start_radio,stats.typeDistance.c_str(), stats.cantidad_puntos_init, stats.JUMP_FACTOR);
    printf("\n");
}

/*void printHeader() {
    printf("%-12s|%10s|%-10s|%-6s|%-15s|%-15s|%15s|\n",
           "SET NAME", "STEP", "ALGORITMO", "K", "TIEMPO us","MEMORIA Bytes","distance");
}

void printStats(Stats &stats) {
    printf("%-12s|%10.1f|%-10s|%6d|%15.01f|%15d|%15s|",
           stats.dataset.c_str(), stats.step, stats.algoritmo.c_str(), stats.k,stats.buildTime,stats.peakMemory,stats.typeDistance.c_str());
    printf("\n");
}*/

int main(int argc, char *argv[]) {
    if (argc < 7) {
        printf("%s  <PATH> <PATH QUERY FILE> <K> <algorithm 1=knnpq, 2=IR-FULL, 3=IR-MED, 4=IR-NO> <step for IR>  <type distance> <JUMP_FACTOR>\n",
               argv[0]);
        return -1;
    }

    Stats resultados; //para guardar el resultado de los experimentos.
    Cronometer *crono = cCronometer();
    MREP *rep = loadRepresentation(argv[1]);
    std::vector<Point> puntos = llenaQueryPoints(argv[2]);
    int k = atoi(argv[3]);
    int algo = atoi(argv[4]);
    double step = atof(argv[5]);
    int typeDistance = atoi(argv[6]);
    int inc = atoi(argv[7]);
    resultados.typeDistance = typeDistance == 1 ? "euclidean" : "manhattan";

    switch (algo) {
        case 1:  //PRIORITY QUEUE ALGORITHM
        {
            double time = 0.0;
            unsigned long mem = 0;
            int conta = 0;
            double acumTime = 0.0;
            Point q;
            for (auto i = puntos.begin(); i != puntos.end(); ++i) {
                knnPQ knn;
                q.x = i->x;
                q.y = i->y;
                start_clock(crono);
                    knn.knn(rep, q, k, typeDistance);
                acumTime += (double) (stop_clock(crono) * 1000000.0);
                mem += knn.obtenerMemoriaUtilizada();
                conta++;
            }
            resultados.algoritmo = "PQ";
            resultados.buildTime = acumTime / conta;
            resultados.peakMemory = mem / conta;
            resultados.JUMP_FACTOR = inc; //aunque no se utiliza.
            break;
        }
        case 2: // IR FULL OPTIMIZATION
        {
            /*  knnIR knn;
              int conta = 0;
              unsigned long mem = 0;
              start_clock(crono);
              for (auto i = puntos.begin(); i != puntos.end(); ++i) {
                  Point q;
                  q.x = i->x;
                  q.y = i->y;
                  knn.knn(rep, q, k, step, typeDistance, resultados, 2,inc);
                  conta++;
                  mem = mem + knn.obtenerMemoriaUtilizada();
                  knn.setMemoriaUtilizada(0.0);
              }
              time = (double) (stop_clock(crono) * 1000000.0) / conta;
              resultados.peakMemory = mem / conta;
              resultados.algoritmo = "IR_FULL";
              resultados.buildTime = time;
  */
            break;
        }

        case 3: // IR MED OPTIMIZATION
        {
            int conta = 0;
            double time = 0.0;
            unsigned long mem = 0;
            double acumTime = 0.0;
            for (auto i = puntos.begin(); i != puntos.end(); ++i) {
                Point q;
                knnIR knn;
                q.x = i->x;
                q.y = i->y;
              //  std::cout << "q.x = " << q.x << ";" << std::endl;
               // std::cout << "q.y = " << q.y << ";" << std::endl;
                start_clock(crono);
                knn.knn(rep, q, k, step, typeDistance, resultados, 1, inc);
                acumTime += (double) (stop_clock(crono) * 1000000.0) ;
                mem = mem + knn.obtenerMemoriaUtilizada();
                conta++;
            }
            resultados.algoritmo = "IR_MED";
            resultados.buildTime = acumTime / conta;
            resultados.peakMemory = mem / conta;
            break;
        }
        case 4: //IR WITH NO OPTIMIZATION
        {
            /*   knnIRNO knn;
               int conta = 0;
               start_clock(crono);
               for (auto i = puntos.begin(); i != puntos.end(); ++i) {
                   Point q;
                   q.x = i->x;
                   q.y = i->y;
                   knn.knn(rep, q, k, step);
                   conta++;
               }
               time = (double) (stop_clock(crono) * 1000000.0) / conta;
               resultados.peakMemory = knn.obtenerMemoriaUtilizada();
               resultados.algoritmo = "IR_NO";
               resultados.buildTime = time;
               */
            break;
        }

        default: {
            std::cout << "Opción del algoritmo no reconocida" << std::endl;
            exit(0);
            break;
        }

    }


    resultados.k = k;
    resultados.dataset = extractFilename(argv[1]);
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
