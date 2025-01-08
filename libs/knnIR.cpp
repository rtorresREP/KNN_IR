#include "knnIR.hpp"
#include "TimeMesure.c"

using namespace std;

int conteoPuntos(MREP *rep, int y1, int y2, int x1, int x2, int typeKNN, int cantidad, int JUMP_FACTOR,
                 Count *countFunction, int kvecino) {
    //   cout << "entre a conteo de puntos ->";
    //  cout << "x1: "<<x1<<" y1: "<<y1<<" x2: "<<x2<<" y2: "<<y2<<endl;
    int cantReturn = 0;

    if (cantidad <= JUMP_FACTOR) {
        uint **rangeResult = compactRangeQuery(rep, x1, x2, y1, y2); //consulta por rango normal.
        cantReturn = rangeResult[0][0];
    } else {
        //  cout <<" COUNT FUNCTION !!"<< endl;
        cantReturn = typeKNN == 1 ? countFunction->compactCount(rep, x1, x2, y1, y2)
                                  : countFunction->compactCountOptimized(rep, x1, x2, y1, y2);
    }

    return cantReturn;
}
vector<pair<int, int>> knnIR::knn(MREP *rep, Point q, int k, double step, int typeDistance, Stats &resultados, int typeKNN, int inc) {
    Cronometer *cronox = cCronometer();
    std::vector<pair<int, int>> puntillos;
    resultados.JUMP_FACTOR = inc;
    Count countFunction;
    int startRadio = 0;
    double min_m = 0;
    int cantidad_vueltas = 1;
    int cant_puntos_devueltos = 0;
    int JUMP = 150; //150 era originamelte
    double r = 0;
    double tamMatrix =  static_cast<double>(rep->numberOfNodes) -1;
    double densidad = static_cast<double>(rep->numberOfEdges) / (static_cast<double>(tamMatrix) * static_cast<double>(tamMatrix));

    //startRadio = static_cast<int> (std::sqrt(k/densidad) / 2) / std::pow(step,inc);
    min_m =  std::ceil((-1 + std::sqrt(1 + 4 * k / 4)) / 2);
    min_m =  min_m * inc;
   // cout << "start radio is: "<< startRadio <<endl;
   // cout << "min m is: "<< min_m << endl;

    resultados.min_m = min_m;
    resultados.start_radio = startRadio;
    r = static_cast<double>(min_m);

  /*  if(startRadio > min_m ){
         r = static_cast<double>(startRadio);
    }else{
        r = static_cast<double>(min_m);
    }*/

    double rad = std::ceil(r);
    resultados.radioInicial = rad;
    uint x1 = std::max(0.0, q.x - rad);
    uint x2 = std::min(q.x + rad, tamMatrix);
    uint y1 = std::max(0.0, q.y - rad);
    uint y2 = std::min (q.y + rad, tamMatrix);

    if (k > 1500) {
        cant_puntos_devueltos = countFunction.compactCount(rep, x1, x2, y1, y2);
    }else{
        cant_puntos_devueltos = conteoPuntos(rep, y1,y2,x1,x2,typeKNN, cant_puntos_devueltos, JUMP, &countFunction, k);
    }

   // cout << "cantidad es: "<< cant_puntos_devueltos << endl;
    resultados.cantidad_puntos_init = cant_puntos_devueltos; //para medición, guardarmos el primer registro de los puntos devueltos.

    while (cant_puntos_devueltos < k) {  //conseguimos los k vecinos??,
        r = r * step;
        rad = std::ceil(r);
        cantidad_vueltas ++; //cuantas vueltas nos dimos para conseguir los k?

         x1 = std::max(0.0, q.x - rad);
         x2 = std::min(q.x + rad, tamMatrix);
         y1 = std::max(0.0, q.y - rad);
         y2 = std::min (q.y + rad, tamMatrix);

        cant_puntos_devueltos = conteoPuntos(rep, y1,y2,x1,x2,typeKNN, cant_puntos_devueltos, JUMP, &countFunction, k);
    }
   // resultados.countTime = (double) (stop_clock(cronox) * 1000000.0) ;
    resultados.radioFinal = rad; //radio final utilizado para la consulta.
    resultados.cantVueltasCount =  cantidad_vueltas; //cantidad de vueltas finales.
    resultados.cantPointsCount = cant_puntos_devueltos; //cantidad de puntos finales.
    setMemoriaUtilizada(countFunction.obtenerMemoriaUtilizada());

    // Ajuste final del radio en base al tipo de distancia utilizada.
    double a = std::sqrt(2);

    r = (typeDistance == 1) ? std::ceil (r *a) : rad; //ajustamos en base al tipo de distancia, euclid o manhattan

    uint p1 = std::max(0.0, q.x - r);
    uint p2 = std::min(q.x + r, tamMatrix);
    uint q1 = std::max(0.0, q.y - r);
    uint q2 = std::min(q.y + r, tamMatrix);

    //start_clock(cronox);
    compactRangeQuery2(rep, p1, p2, q1, q2,  k, q.x,  q.y, puntillos, typeDistance); //ocupa un minheap de tamaño k, para no sacar todos los puntos de la consulta por rango.
    //resultados.rangeTime = (double) (stop_clock(cronox) * 1000000.0) ;
   // resultados.buildTime = resultados.rangeTime+resultados.countTime;
    //resultados.peakMemory = countFunction.getMemoryUsed();

    return puntillos;
}

