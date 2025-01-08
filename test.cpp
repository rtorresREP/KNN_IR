#include <iostream>
#include "libs/kTree.h"
#include "libs/Count.hpp"
#include <ctime> 
#include "libs/knnPQ.hpp"
#include <fstream>
#include <dirent.h>
#include <sys/types.h>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("%s <DIRECTORY> <K>\n", argv[0]);
        return -1;
    }
    int k = atoi(argv[2]);
    vector<string> files;

    int numFiles=0;
    DIR *dr;
    struct dirent *en;
    dr = opendir(argv[1]); //open all directory
    if (dr) {
        while ((en = readdir(dr)) != NULL) {
            int tam = strlen(en->d_name);
            if(en->d_name[tam-2]=='k' && en->d_name[tam-1]=='t'){
                string auxS = en->d_name;
                auxS = auxS.substr(0, tam-3);
                files.push_back(auxS);
                numFiles++;
            }
        }
        closedir(dr); //close all directory
    }

    double step;

    unsigned t0, t1;
    unsigned int **res;

    string dir = argv[1];

    double results[numFiles][4][5][10];

    int itfiles=0;
    for (auto ff = files.begin(); ff != files.end(); ++ff){
        string fn = *ff;
        cout << fn << endl;
        fn = dir + "/" + fn;

        char* fc = &fn[0];

        MREP *rep = loadRepresentation(fc);

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

        int randomIndexes[50];
        for(int i=0;i<50;i++) randomIndexes[i] = rand()%rep->numberOfEdges;

        int ii=0,a;
        while (myfile >> a){
            p[1][ii]=a;
            myfile >> a;
            p[0][ii]=a;
            ii++;
        }

        myfile.close();

        int qx,qy; 
        double time=0,inc;

        int aa=0,bb=0,cc=0;
        cout << "KNN IR CHEB" << endl;
        bb=0;
        for(inc=1;inc<5;inc=inc*sqrt(2)){
            //cout << "INC: " << inc << "\t";
            cc=0;
            for(step=1.1;step<2.1;step=step+0.1){
                time = 0;
                double incc;
                for(int i=0;i<50;i++){
                    ii = randomIndexes[i];
                    qx=p[0][ii];
                    qy=p[1][ii];
                    incc = round(inc * 1000.0) / 1000.0;
                    t0 = clock();
                    res = KNN_IR_CHEV(rep,qx,qy,k,step,incc);
                    t1 = clock();
                    
                    time += (double(t1-t0)*1000000/CLOCKS_PER_SEC);
                }
                //cout << time/50 << " ";
                results[itfiles][0][bb][cc] = time/50;
                cc++;
            }
            bb++;
            //cout << endl;
        }

        aa=0;
        bb=0;
        cc=0;
        cout << "KNN IR EUC" << endl;
        bb=0;
        for(inc=1;inc<5;inc=inc*sqrt(2)){
            //cout << "INC: " << inc << "\t";
            cc=0;
            for(step=1.1;step<2.1;step=step+0.1){
                time = 0;
                double incc;
                for(int i=0;i<50;i++){
                    ii = randomIndexes[i];
                    qx=p[0][ii];
                    qy=p[1][ii];
                    incc = round(inc * 1000.0) / 1000.0;
                    t0 = clock();
                    res = KNN_IR_EUC(rep,qx,qy,k,step,incc);
                    t1 = clock();
                    
                    time += (double(t1-t0)*1000000/CLOCKS_PER_SEC);
                }
                //cout << time/50 << " ";
                results[itfiles][1][bb][cc] = time/50;
                cc++;
            }
            bb++;
            //cout << endl;
        }

        knnPQ pq;
        Point q;

        aa=0;
        cout << "KNN PQ CHEB" << endl;
        time = 0;
        for(int i=0;i<50;i++){
            ii = randomIndexes[i];
            q.x=p[0][ii];
            q.y=p[1][ii];
            t0 = clock();
            auto pointsPQ = pq.knn(rep, q, k, 2);
            t1 = clock();

            time += (double(t1-t0)*1000000/CLOCKS_PER_SEC);
        }
        //cout << time/50 << " ";
        results[itfiles][2][0][0] = time/50;
        //cout << endl;

        aa=0;
        cout << "KNN PQ EUC" << endl;
        time = 0;
        for(int i=0;i<50;i++){
            ii = randomIndexes[i];
            q.x=p[0][ii];
            q.y=p[1][ii];
            t0 = clock();
            auto pointsPQ = pq.knn(rep, q, k, 1);
            t1 = clock();

            time += (double(t1-t0)*1000000/CLOCKS_PER_SEC);
        }
        //cout << time/50 << " ";
        results[itfiles][3][0][0] = time/50;
        //cout << endl;
        itfiles++;
        destroyRepresentation(rep);
    }
    
    cout << "Teminó" << endl;

    double mmin,prom;
    int bestB,bestC;
    for(int al=0;al<2;al++){
        mmin=100000;
        for(int bb=0;bb<5;bb++){
            for(int cc=0;cc<10;cc++){
                prom=0;
                for(int zz=0;zz<numFiles;zz++) prom += results[zz][al][bb][cc];
                if(prom<mmin){
                    bestB = bb;
                    bestC = cc;
                    mmin=prom;
                }
            }
        }
    
        cout << "Mejor resultado k=" << k << " algoritmo " << al << " es INC " << pow(sqrt(2),bestB) << " STEP " << 1 + 0.1*bestC << endl;
        for(int zz=0;zz<numFiles;zz++) cout << results[zz][al][bestB][bestC] << " ";
        cout << endl;
    }
    cout << "Mejor resultado k=" << k << " algoritmo KNN PQ Cheb es INC 0 STEP 0" << endl;
    for(int zz=0;zz<numFiles;zz++) cout << results[zz][2][0][0] << " ";
    cout << endl;
    cout << "Mejor resultado k=" << k << " algoritmo KNN PQ EUC es INC 0 STEP 0" << endl;
    for(int zz=0;zz<numFiles;zz++) cout << results[zz][3][0][0] << " ";
    cout << endl;

    return 0;
}
