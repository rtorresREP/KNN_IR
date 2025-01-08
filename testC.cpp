#include <iostream>
#include "libs/kTree.h"
#include "libs/Count.hpp"
#include <ctime> 
#include "libs/knnPQ.hpp"
#include <fstream>

using namespace std;

double calcularDistanciaEuclidiana(int x1, int y1, int x2, int y2) {
    double dx = x1 - x2;
    double dy = y1 - y2;
    return dx * dx + dy * dy;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("%s <PATH> <K> <STEP>\n", argv[0]);
        return -1;
    }
    int k = atoi(argv[2]);
    double step = atof(argv[3]);

    unsigned t0, t1;
    unsigned int **res;

    MREP *rep = loadRepresentation(argv[1]);

    string fn = argv[1];
    fn = fn + ".txt";

    fstream myfile;
    myfile.open(fn);

    if (!myfile)
    {
        cout << "Unable to open " << fn << endl;
        return(0);
    }

    int **p;
    p = new int*[2];
    p[0] = new int[rep->numberOfEdges];
    p[1] = new int[rep->numberOfEdges];

    int ii=0,a;
    while (myfile >> a){
        p[1][ii]=a;
        myfile >> a;
        p[0][ii]=a;
        ii++;
    }

    int qx,qy; 
    double time=0;
    
    double inc=2;
    for(int i=0;i<50;i++){
        ii = rand()%rep->numberOfEdges;
        qx=p[0][ii];
        qy=p[1][ii];
        //cout << i << " " << qx << "," << qy << " " << checkLink(rep,qx,qy) << endl;
        //getchar();
        t0 = clock();
        res = KNN_IR_CHEV(rep,qx,qy,k,step,inc);
        t1 = clock();
        
        time += (double(t1-t0)*1000000/CLOCKS_PER_SEC);
    }

    cout << " RTknnc: time= " << time/50 << endl;

    time=0;

    for(int i=0;i<50;i++){
        ii = rand()%rep->numberOfEdges;
        qx=p[0][ii];
        qy=p[1][ii];
        t0 = clock();
        res = KNN_IR_EUC(rep,qx,qy,k,step,inc);
        t1 = clock();
        
        time += (double(t1-t0)*1000000/CLOCKS_PER_SEC);
    }

    cout << "RTknne: time= " << time/50 << endl;

    knnPQ pq;
    Point q;

    time = 0;
    for(int i=0;i<50;i++){
        ii = rand()%rep->numberOfEdges;
        q.x=p[0][ii];
        q.y=p[1][ii];
        t0 = clock();
        auto pointsPQ = pq.knn(rep, q, k, 1);
        t1 = clock();

        time += (double(t1-t0)*1000000/CLOCKS_PER_SEC);
    }

    cout << "PQknne: time= " << time/50 << endl;

    return 0;
}
