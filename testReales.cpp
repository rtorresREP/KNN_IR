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
    if (argc < 2) {
        printf("%s <DIRECTORY>\n", argv[0]);
        return -1;
    }

    vector<string> files;

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
            }
        }
        closedir(dr); //close all directory
    }

    int k;
    double step;

    unsigned t0, t1;
    unsigned int **res;

    string dir = argv[1];
    vector<int> Ks;
    Ks.push_back(500);
    Ks.push_back(1000);
    Ks.push_back(5000);
    Ks.push_back(10000);
    Ks.push_back(15000);

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

        int qx,qy; 
        double time=0,inc, results[5][5][10];

        cout << "KNN CHEV" << endl;
        int aa=0,bb=0,cc=0;
        for(auto ks = Ks.begin(); ks != Ks.end(); ++ks){
            k = *ks;
            //cout << "Vecinos: " << k << endl;
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
                    results[aa][bb][cc] = time/50;
                    cc++;
                }
                bb++;
                //cout << endl;
            }
            aa++;
        }
        double mmin=100000,prom;
        int bestB,bestC;
        for(bb=0;bb<5;bb++){
            for(cc=0;cc<10;cc++){
                prom=0;
                for(aa=0;aa<5;aa++) prom += results[aa][bb][cc];
                if(prom<mmin){
                    bestB = bb;
                    bestC = cc;
                    mmin=prom;
                }
            }
        }
        
        cout << "Mejor resultado es INC " << pow(sqrt(2),bestB) << " STEP " << 1 + 0.1*bestC << endl;
        for(aa=0;aa<5;aa++) cout << results[aa][bestB][bestC] << " ";
        cout << endl;

        cout << "KNN EUC" << endl;
        aa=0;
        bb=0;
        cc=0;
        for(auto ks = Ks.begin(); ks != Ks.end(); ++ks){
            k = *ks;
            //cout << "Vecinos: " << k << endl;
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
                    results[aa][bb][cc] = time/50;
                    cc++;
                }
                bb++;
                //cout << endl;
            }
            aa++;
        }
        mmin=100000;
        for(bb=0;bb<5;bb++){
            for(cc=0;cc<10;cc++){
                prom=0;
                for(aa=0;aa<5;aa++) prom += results[aa][bb][cc];
                if(prom<mmin){
                    bestB = bb;
                    bestC = cc;
                    mmin=prom;
                }
            }
        }
        
        cout << "Mejor resultado es INC " << pow(sqrt(2),bestB) << " STEP " << 1 + 0.1*bestC << endl;
        for(aa=0;aa<5;aa++) cout << results[aa][bestB][bestC] << " ";
        cout << endl;

        knnPQ pq;
        Point q;

        cout << "KNN PQ CHEB" << endl;
        aa=0;
        for(auto ks = Ks.begin(); ks != Ks.end(); ++ks){
            k = *ks;
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
            results[aa][0][0] = time/50;
            aa++;
        }
        cout << "Mejor resultado es INC 0 STEP 0" << endl;
        for(aa=0;aa<5;aa++) cout << results[aa][0][0] << " ";
        cout << endl;

        cout << "KNN PQ EUC" << endl;
        aa=0;
        for(auto ks = Ks.begin(); ks != Ks.end(); ++ks){
            k = *ks;
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
            results[aa][0][0] = time/50;
            aa++;
        }
        cout << "Mejor resultado es INC 0 STEP 0" << endl;
        for(aa=0;aa<5;aa++) cout << results[aa][0][0] << " ";
        cout << endl;
    }
    return 0;
}
