#include "kTree.h"
#include <queue>
#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

// Estructura para representar un punto con coordenadas (x, y) y una distancia asociada
struct Punto {
    int x, y;
    double distancia;

    Punto(int _x, int _y, double _distancia) : x(_x), y(_y), distancia(_distancia) {}
};

// Clase para comparar puntos en base a su distancia
class ComparePoint {
        public:
        bool operator()(const Punto& p1, const Punto& p2) {
            return p1.distancia < p2.distancia; // Max heap, mayor distancia primero
        }
};

typedef struct edgeinfo {
    uint xval;
    uint yval;
    uint kval;
} tedge;
typedef struct QUEUEOFFCONS {
    uint offsetL;
    uint offsetR;
    struct QUEUEOFFCONS *link;

} QUEUEOFFCONS;

typedef struct dq dq;
QUEUEOFFCONS *finalQUEUEOFFCONS;

double calcularDistanciaEuclidiana(const Punto& p1, const Punto& p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return dx * dx + dy * dy;
}

double calcularDistanciaManhattan(const Punto& p1, const Punto& p2) {
    double dx = std::abs(p2.x - p1.x);
    double dy = std::abs(p2.y - p1.y);
    return dx + dy;
}

double calcularDistanciaChebishev(const Punto& p1, const Punto& p2) {
    double dx = abs(p2.x - p1.x);
    double dy = abs(p2.y - p1.y);
    if(dx>dy) return dx;
    else return dy;
}

//------------------------------------DESDE ACA MODIFICACIONES RTORRES

bool isCircleIntersectingRectangle(uint circle_x, uint circle_y, uint radius,
                                   uint rect_x1, uint rect_y1, uint rect_x2, uint rect_y2) {
    // Find the closest point to the circle within the rectangle
    double closest_x = max(rect_x1, min(circle_x, rect_x2));
    double closest_y = max(rect_y1, min(circle_y, rect_y2));

    // Calculate the distance between the circle's center and this closest point
    double distance_x,distance_y;
    if(circle_x > closest_x) distance_x = circle_x - closest_x;
    else distance_x = closest_x - circle_x;
    if(circle_y > closest_y) distance_y = circle_y - closest_y;
    else distance_y = closest_y - circle_y;
    double distance_squared = (distance_x * distance_x) + (distance_y * distance_y);

    // If the distance is less than or equal to the circle's radius squared, then they intersect
    double rr = radius;
    return distance_squared <= (rr * rr);
}

uint childXY(uint x, uint y, uint dim){
    uint output = (y/dim)*K;
    output += (x/dim);
    return output;
}

uint checkLink(MREP * rep, uint x, uint y){
    uint actual=0, level=0, x1=0, y1=0;
    uint x2=rep->numberOfNodes, y2=rep->numberOfNodes;
    
    while(level<=rep->maxLevel){
        uint hijo = childXY(x-x1,y-y1,(x2-x1)/K);
        if(bitget(rep->btl->data,actual+hijo)==1){
            actual = rank1(rep->btl,actual+hijo)*K*K;
            uint dim = (x2-x1)/K;
            y1 = y1 + (hijo/K)*dim;
            y2 = y1 + dim;
            x1 = x1 + dim*(hijo%K);
            x2 = x1 + dim;
        }
        else return 0;
        level++;
    }
    return 1;
}

//------------RECURSIVOS

void heapRange(MREP * rep, uint pos, uint xi, uint xf, uint yi, uint yf, uint x1, uint x2, uint y1, uint y2, uint l, uint k, Punto &queryPoint, priority_queue<Punto, vector<Punto>, ComparePoint>& minHeap){
    uint sum=0;
    for(int hijo=0;hijo<K*K;hijo++){
        if(bitget(rep->btl->data,pos+hijo)==1){
            if(l<rep->maxLevel){
                uint dim = (x2-x1)/K;
                uint y1t = y1 + (hijo/K)*dim;
                uint y2t = y1t + dim;
                uint x1t = x1 + dim*(hijo%K);
                uint x2t = x1t + dim;
                if(xf>=x1t && xi<=x2t && yf>=y1t && yi<=y2t){
                    uint posR = rank1(rep->btl,pos+hijo)*K*K;
                    heapRange(rep, posR, xi, xf, yi, yf, x1t, x2t, y1t, y2t, l+1,k,queryPoint,minHeap);
                }
            }
            else {
                uint dim = (x2-x1)/K;
                uint y1t = y1 + (hijo/K)*dim;
                uint x1t = x1 + dim*(hijo%K);
                if(x1t>=xi && x1t<=xf && y1t>=yi && y1t<=yf){;
                    Punto aux(x1t, y1t, 0);
                    double distancia = calcularDistanciaChebishev(aux, queryPoint);
                    aux.distancia = distancia;
                    if (minHeap.size() < k) {
                        minHeap.push(aux);
                    } else {
                    if (aux.distancia < minHeap.top().distancia) {
                            minHeap.pop();
                            minHeap.push(aux);
                        }
                    }
                }
            }
        }
    }
}

void circularHeapRange(MREP * rep, uint pos, uint x, uint y, uint r, uint x1, uint x2, uint y1, uint y2, uint l, uint k, Punto &queryPoint, priority_queue<Punto, vector<Punto>, ComparePoint>& minHeap){
    int sum=0;
    for(int hijo=0;hijo<K*K;hijo++){
        if(bitget(rep->btl->data,pos+hijo)==1){
            if(l<rep->maxLevel){
                uint dim = (x2-x1)/K;
                uint y1t = y1 + (hijo/K)*dim;
                uint y2t = y1t + dim;
                uint x1t = x1 + dim*(hijo%K);
                uint x2t = x1t + dim;
                if(isCircleIntersectingRectangle(x,y,r,x1t,y1t,x2t,y2t)){
                    uint posR = rank1(rep->btl,pos+hijo)*K*K;
                    circularHeapRange(rep, posR, x, y, r, x1t, x2t, y1t, y2t, l+1,k,queryPoint,minHeap);
                }
            }
            else {
                uint dim = (x2-x1)/K;
                uint y1t = y1 + (hijo/K)*dim;
                uint x1t = x1 + dim*(hijo%K);
                double xx,yy,rr;
                if(x>x1t) xx = x-x1t;
                else xx = x1t-x;
                if(y>y1t) yy = y-y1t;
                else yy = y1t-y;
                rr = r;
                if(xx*xx + yy*yy <= rr*rr){
                    Punto aux(x1t, y1t, 0);
                    double distancia = calcularDistanciaEuclidiana(aux, queryPoint);
                    aux.distancia = distancia;
                    if (minHeap.size() < k) {
                        minHeap.push(aux);
                    } else {
                    if (aux.distancia < minHeap.top().distancia) {
                            minHeap.pop();
                            minHeap.push(aux);
                        }
                    }
                }
            }
        }
    }
}

uint rangeCount(MREP * rep, uint pos, uint xi, uint xf, uint yi, uint yf, uint x1, uint x2, uint y1, uint y2, uint l){
    uint sum=0;
    for(int hijo=0;hijo<K*K;hijo++){
        if(bitget(rep->btl->data,pos+hijo)==1){
            if(l<rep->maxLevel){
                uint dim = (x2-x1)/K;
                uint y1t = y1 + (hijo/K)*dim;
                uint y2t = y1t + dim;
                uint x1t = x1 + dim*(hijo%K);
                uint x2t = x1t + dim;
                if(xf>=x1t && xi<=x2t && yf>=y1t && yi<=y2t){
                    uint posR = rank1(rep->btl,pos+hijo)*K*K;
                    sum += rangeCount(rep, posR, xi, xf, yi, yf, x1t, x2t, y1t, y2t, l+1);
                }
            }
            else {
                uint dim = (x2-x1)/K;
                uint y1t = y1 + (hijo/K)*dim;
                uint x1t = x1 + dim*(hijo%K);
                if(x1t>=xi && x1t<=xf && y1t>=yi && y1t<=yf){;
                    sum++;
                }
            }
        }
    }
    return sum;
}

uint circularRangeCount(MREP * rep, uint pos, uint x, uint y, uint r, uint x1, uint x2, uint y1, uint y2, uint l){
    int sum=0;
    for(int hijo=0;hijo<K*K;hijo++){
        if(bitget(rep->btl->data,pos+hijo)==1){
            if(l<rep->maxLevel){
                uint dim = (x2-x1)/K;
                uint y1t = y1 + (hijo/K)*dim;
                uint y2t = y1t + dim;
                uint x1t = x1 + dim*(hijo%K);
                uint x2t = x1t + dim;
                if(isCircleIntersectingRectangle(x,y,r,x1t,y1t,x2t,y2t)){
                    uint posR = rank1(rep->btl,pos+hijo)*K*K;
                    sum += circularRangeCount(rep, posR, x, y, r, x1t, x2t, y1t, y2t, l+1);
                }
            }
            else {
                uint dim = (x2-x1)/K;
                uint y1t = y1 + (hijo/K)*dim;
                uint x1t = x1 + dim*(hijo%K);
                double xx,yy,rr;
                if(x>x1t) xx = x-x1t;
                else xx = x1t-x;
                if(y>y1t) yy = y-y1t;
                else yy = y1t-y;
                rr = r;
                if(xx*xx + yy*yy <= rr*rr){
                    sum++;
                }
            }
        }
    }
    return sum;
}

uint quadrantCount(MREP *rep, uint i) {
        bool isALeafstart, isALeafend;
        int start, end;
        if (i == -1) {
            isALeafstart = false;
            isALeafend = false;
            start = 0;
            end = K*K - 1;
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
            start = rank1(rep->btl, start) * K*K;
            if (isBitSet(rep->btl, start) == 0) {
                start = select1(rep->btl, rank1(rep->btl, start) + 1);
            }

            end = rank1(rep->btl, end) * K*K + K*K - 1;
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

uint compactCount(MREP * rep, uint pos, uint xi, uint xf, uint yi, uint yf, uint x1, uint x2, uint y1, uint y2, uint l){
    uint sum=0;
    for(int hijo=0;hijo<K*K;hijo++){
        if(bitget(rep->btl->data,pos+hijo)==1){
            if(l<rep->maxLevel){
                uint dim = (x2-x1)/K;
                uint y1t = y1 + (hijo/K)*dim;
                uint y2t = y1t + dim;
                uint x1t = x1 + dim*(hijo%K);
                uint x2t = x1t + dim;
                if(xf>=x1t && xi<=x2t && yf>=y1t && yi<=y2t){
                    if(xi<=x1t && xf>=x2t && yi<=y1t && yf>=y2t) sum += quadrantCount(rep,pos+hijo);
                    else{
                        uint posR = rank1(rep->btl,pos+hijo)*K*K;
                        sum += compactCount(rep, posR, xi, xf, yi, yf, x1t, x2t, y1t, y2t, l+1);
                    }
                }
            }
            else {
                uint dim = (x2-x1)/K;
                uint y1t = y1 + (hijo/K)*dim;
                uint x1t = x1 + dim*(hijo%K);
                if(x1t>=xi && x1t<=xf && y1t>=yi && y1t<=yf){;
                    sum++;
                }
            }
        }
    }
    return sum;
}

bool insideCircle(uint x, uint y, uint r, uint x1, uint x2, uint y1, uint y2){
    double xx1,xx2,yy1,yy2;
    if(x1>x) xx1 = (x1-x);
    else xx1 = (x-x1);
    if(x2>x) xx2 = (x2-x);
    else xx2 = (x-x2);
    if(y1>y) yy1 = (y1-y);
    else yy1 = (y-y1);
    if(y2>y) yy2 = (y2-y);
    else yy2 = (y-y2);
    double rr = r;

    return xx1*xx1+yy1*yy1<=rr*rr && xx1*xx1+yy2*yy2<=rr*rr && xx2*xx2+yy1*yy1<=rr*rr && xx2*xx2+yy2*yy2<=rr*rr;
}

uint circularCompactCount(MREP * rep, uint pos, uint x, uint y, uint r, uint x1, uint x2, uint y1, uint y2, uint l){
    int sum=0;
    for(int hijo=0;hijo<K*K;hijo++){
        if(bitget(rep->btl->data,pos+hijo)==1){
            if(l<rep->maxLevel){
                uint dim = (x2-x1)/K;
                uint y1t = y1 + (hijo/K)*dim;
                uint y2t = y1t + dim;
                uint x1t = x1 + dim*(hijo%K);
                uint x2t = x1t + dim;
                if(isCircleIntersectingRectangle(x,y,r,x1t,y1t,x2t,y2t)){
                    if(insideCircle(x,y,r,x1t,x2t,y1t,y2t)) sum += quadrantCount(rep,pos+hijo);
                    else {
                        uint posR = rank1(rep->btl,pos+hijo)*K*K;
                        sum += circularCompactCount(rep, posR, x, y, r, x1t, x2t, y1t, y2t, l+1);
                    }
                }
            }
            else {
                uint dim = (x2-x1)/K;
                uint y1t = y1 + (hijo/K)*dim;
                uint x1t = x1 + dim*(hijo%K);
                double xx,yy,rr;
                if(x>x1t) xx = x-x1t;
                else xx = x1t-x;
                if(y>y1t) yy = y-y1t;
                else yy = y1t-y;
                rr = r;
                if(xx*xx + yy*yy <= rr*rr){
                    sum++;
                }
            }
        }
    }
    return sum;
}

//------------CALLERS

uint **callerHeapRange(MREP * rep, uint xi, uint xf, uint yi, uint yf, uint k,uint x, uint y){
    uint **output;
    output = new uint*[2];
    output[0] = new uint[k];
    output[1] = new uint[k];

    priority_queue<Punto, vector<Punto>, ComparePoint> minHeap;
    Punto queryPoint(x,y, 0);

    heapRange(rep, 0, xi, xf, yi, yf, 0,rep->numberOfNodes,0,rep->numberOfNodes,0, k, queryPoint, minHeap);

    int i=0;
    while(!minHeap.empty()) {
        struct Punto p = minHeap.top(); // Obtener el elemento superior
        minHeap.pop(); // Eliminar el elemento superior
        output[0][i]=p.x;
        output[1][i]=p.y;
        i++;
    }
    return output;
}

uint **callerCircularHeapRange(MREP * rep, uint qx, uint qy, uint r, uint k,uint x, uint y){
    uint **output;
    output = new uint*[2];
    output[0] = new uint[k];
    output[1] = new uint[k];
    priority_queue<Punto, vector<Punto>, ComparePoint> minHeap;
    Punto queryPoint(x,y, 0);

    circularHeapRange(rep, 0, qx, qy, r, 0,rep->numberOfNodes,0,rep->numberOfNodes,0, k, queryPoint, minHeap);
    int i=0;
    while(!minHeap.empty()) {
        struct Punto p = minHeap.top(); // Obtener el elemento superior
        minHeap.pop(); // Eliminar el elemento superior
        //cout << p.x << "," << p.y << " d=" << p.distancia << endl;
        output[0][i]=p.x;
        output[1][i]=p.y;
        i++;
    }
    return output;
}

uint callerRangeCount(MREP * rep, uint xi, uint xf, uint yi, uint yf){
    return rangeCount(rep,0,xi, xf, yi, yf,0,rep->numberOfNodes,0,rep->numberOfNodes,0);
}

uint callerCircularRangeCount(MREP * rep, uint x, uint y, uint r){
    return circularRangeCount(rep,0,x, y, r,0,rep->numberOfNodes,0,rep->numberOfNodes,0);
}

uint callerCompactCount(MREP * rep, uint xi, uint xf, uint yi, uint yf){
    return compactCount(rep,0,xi, xf, yi, yf,0,rep->numberOfNodes,0,rep->numberOfNodes,0);
}

uint callerCircularCompactCount(MREP * rep, uint x, uint y, uint r){
    return circularCompactCount(rep,0,x, y, r,0,rep->numberOfNodes,0,rep->numberOfNodes,0);
}

//------------------------------KNN IRs

uint **KNN_IR_CHEV(MREP * rep, uint x, uint y, uint k, double step, double inc){
    uint tamMatrix = rep->numberOfNodes;
    uint rad = ceil((-1 + std::sqrt(1 + 4 * k / 4)) / 2);
    rad = ceil(rad*inc);

    uint x1,x2,y1,y2;
    if(rad>x) x1 = 0;
    else x1=x-rad;
    x2 = min(x + rad, tamMatrix);
    if(rad>y) y1 = 0;
    else y1=y-rad;
    y2 = min (y + rad, tamMatrix);

    uint cant = callerCompactCount(rep,x1,x2,y1,y2);
    while(cant < k){
        rad = ceil(rad*step);
        if(rad>x) x1 = 0;
        else x1=x-rad;
        x2 = min(x + rad, tamMatrix);
        if(rad>y) y1 = 0;
        else y1=y-rad;
        y2 = min (y + rad, tamMatrix);
        cant = callerCompactCount(rep,x1,x2,y1,y2);
    }
    return callerHeapRange(rep,x1,x2,y1,y2,k,x,y);
}

uint **KNN_IR_EUC(MREP * rep, uint x, uint y, uint k, double step, double inc){
    uint rad = ceil((-1 + std::sqrt(1 + 4 * k / 4)) / 2);
    rad = ceil(rad*inc);

    uint cant = callerCircularCompactCount(rep,x,y,rad);
    while(cant < k){
        rad = ceil(rad*step);
        cant = callerCircularCompactCount(rep,x,y,rad);
    }
    return callerCircularHeapRange(rep,x,y,rad,k,x,y);
}

//------------------------------------HASTA ACA MODIFICACIONES RTORRES

void AddItem2(MREP *rep, int elem, int cantx, int canty) {
    if (rep->iniq != -1) {
        rep->finq++;
        rep->element[rep->finq] = elem;
        rep->basex[rep->finq] = cantx;
        rep->basey[rep->finq] = canty;

    } else {
        rep->iniq = 0;
        rep->finq = 0;
        rep->element[rep->iniq] = elem;
        rep->basex[rep->iniq] = cantx;
        rep->basey[rep->iniq] = canty;
    }
}

void RemoveItem2(MREP *rep) {
    rep->iniq++;
}

QUEUEOFFCONS *AddItemOFFCONS(QUEUEOFFCONS *listpointer, uint offsetL, uint offsetR) {

    if (listpointer != NULL) {
        QUEUEOFFCONS *lp = (QUEUEOFFCONS *) malloc(sizeof(struct QUEUEOFFCONS));
        finalQUEUEOFFCONS->link = lp;
        lp->link = NULL;
        lp->offsetL = offsetL;
        lp->offsetR = offsetR;
        finalQUEUEOFFCONS = lp;
        return listpointer;
    } else {
        listpointer = (QUEUEOFFCONS *) malloc(sizeof(struct QUEUEOFFCONS));
        listpointer->link = NULL;
        listpointer->offsetL = offsetL;
        listpointer->offsetR = offsetR;

        finalQUEUEOFFCONS = listpointer;
        return listpointer;
    }
}

QUEUEOFFCONS *RemoveItemOFFCONS(QUEUEOFFCONS *listpointer) {
    QUEUEOFFCONS *tempp;
    tempp = listpointer->link;
    free(listpointer);
    return tempp;
}

uint exp_pow(uint base, uint pow) {
    uint i, result = 1;
    for (i = 0; i < pow; i++)
        result *= base;

    return result;
}

MREP *compactCreateKTree(uint *xedges, uint *yedges, uint numberOfNodes, ulong numberOfEdges, uint maxl) {

    MREP *rep;
    rep = (MREP *) malloc(sizeof(struct matrixRep));
    rep->maxLevel = maxl;
    rep->numberOfNodes = numberOfNodes;
    rep->numberOfEdges = numberOfEdges;
    rep->div_level_table = 0;
    rep->info = 0;
    rep->info2[0] = 0;
    rep->info2[1] = 0;
    rep->element = 0;
    rep->basex = 0;
    rep->basey = 0;

    ulong nedges = numberOfEdges;

    uint sizebt = 0;

    uint i, k, j, z, queuecont, conttmp, div_level;

    uint offsetL = 0;
    uint offsetR = 0;

    uint postotal = 0;
    tedge *tedges = (tedge *) malloc(sizeof(tedge) * nedges);


    for (i = 0; i < nedges; i++) {
        tedges[i].xval = xedges[i];
        tedges[i].yval = yedges[i];
    }

    QUEUEOFFCONS *q = NULL;
    finalQUEUEOFFCONS = q;


    uint *counterK = (uint *) malloc(sizeof(uint) * (K * K));
    uint *boundariesK = (uint *) malloc(sizeof(uint) * (K * K + 1));
    uint *pointerK = (uint *) malloc(sizeof(uint) * (K * K));

    uint tempk, tempx, tempy;

    uint *bits;
    bitRankW32Int *BTL;

    uint *bits_BTL = (uint *) malloc(sizeof(uint) * ((nedges * maxl * K * K + W - 1) / W));
    for (i = 0; i < ((nedges * maxl * K * K + W - 1) / W); i++)
        bits_BTL[i] = 0;
    bits = bits_BTL;

    q = AddItemOFFCONS(q, 0, nedges);
    queuecont = 1;
    for (i = 0; i < maxl; i++) {
        conttmp = 0;
        div_level = exp_pow(K, maxl - i);
        for (k = 0; k < queuecont; k++) {
            sizebt += K * K;
            offsetL = q->offsetL;
            offsetR = q->offsetR;

            for (j = 0; j < K * K; j++) {
                counterK[j] = 0;
                pointerK[j] = 0;
            }

            for (j = offsetL; j < offsetR; j++) {
                tedges[j].kval = (tedges[j].xval / div_level) * K + tedges[j].yval / div_level;
                tedges[j].xval = tedges[j].xval % div_level;
                tedges[j].yval = tedges[j].yval % div_level;

                counterK[tedges[j].kval]++;
            }

            boundariesK[0] = offsetL;
            for (j = 0; j < K * K; j++) {
                boundariesK[j + 1] = boundariesK[j] + counterK[j];
                pointerK[j] = boundariesK[j];

                if (boundariesK[j + 1] != boundariesK[j]) {
                    conttmp++;
                    q = AddItemOFFCONS(q, boundariesK[j], boundariesK[j + 1]);

                    bitset(bits, postotal);
                }
                postotal++;
            }

            for (z = 0; z < K * K; z++) {
                while (pointerK[z] < boundariesK[z + 1]) {
                    if (tedges[pointerK[z]].kval != z) {
                        tempk = tedges[pointerK[z]].kval;
                        tempx = tedges[pointerK[z]].xval;
                        tempy = tedges[pointerK[z]].yval;

                        while (tedges[pointerK[tempk]].kval == tempk) {
                            pointerK[tempk]++;
                        }

                        tedges[pointerK[z]].kval = tedges[pointerK[tempk]].kval;
                        tedges[pointerK[z]].xval = tedges[pointerK[tempk]].xval;
                        tedges[pointerK[z]].yval = tedges[pointerK[tempk]].yval;

                        tedges[pointerK[tempk]].kval = tempk;
                        tedges[pointerK[tempk]].xval = tempx;
                        tedges[pointerK[tempk]].yval = tempy;
                        pointerK[tempk]++;
                    } else
                        pointerK[z]++;

                }
            }

            for (z = offsetL; z < offsetR - 1; z++)
                if (tedges[z].kval > tedges[z + 1].kval) {
                    fprintf(stderr, "error: %d\n", z);
                    exit(-1);
                }
            q = (QUEUEOFFCONS *) RemoveItemOFFCONS(q);
        }
        queuecont = conttmp;
    }

    rep->bt_len = postotal;

    uint counttotal = 0;
    while (q != NULL) {

        offsetL = q->offsetL;
        offsetR = q->offsetR;

        for (j = 0; j < K * K; j++) {
            counterK[j] = 0;
        }

        div_level = K;
        for (j = offsetL; j < offsetR; j++) {
            tedges[j].xval = tedges[j].xval % K;
            tedges[j].yval = tedges[j].yval % K;
            tedges[j].kval = tedges[j].xval * K + tedges[j].yval;
            counterK[tedges[j].kval]++;
        }

        for (j = 0; j < K * K; j++) {

            if (counterK[j] > 0) {
                conttmp++;
                counttotal++;
                bitset(bits_BTL, postotal);
            }
            postotal++;

            if (counterK[j] > 1) {
                fprintf(stderr, "error\n");
                exit(-1);
            }
        }
        q = (QUEUEOFFCONS *) RemoveItemOFFCONS(q);
    }

    free(counterK);
    free(boundariesK);
    free(pointerK);
    free(tedges);

    BTL = createBitRankW32Int(bits_BTL, postotal, 1, 20);


    rep->btl = BTL;
    rep->btl_len = postotal;


    return rep;
}

void saveRepresentation(MREP *rep, char *basename) {
    if (rep == NULL) {
        printf("Error! La estructura a guardar no existe\n");
        return;
    }

    char *filename = (char *) malloc(sizeof(char) * (strlen(basename) + 4));
    strcpy(filename, basename);
    strcat(filename, ".kt");
    FILE *ft = fopen(filename, "w");
    fwrite(&(rep->numberOfNodes), sizeof(uint), 1, ft);
    fwrite(&(rep->numberOfEdges), sizeof(ulong), 1, ft);
    fwrite(&(rep->maxLevel), sizeof(uint), 1, ft);
    uint s, n, n2;
    s = rep->btl->s;
    n = rep->btl_len;
    n2 = rep->btl_len;
    fwrite(&(rep->btl_len), sizeof(uint), 1, ft);
    fwrite(&(rep->bt_len), sizeof(uint), 1, ft);
    fwrite(&(rep->btl->factor), sizeof(uint), 1, ft);
    fwrite(rep->btl->data, sizeof(uint), n2 / W + 1, ft);
    fwrite(rep->btl->Rs, sizeof(uint), n / s + 1, ft);
    fclose(ft);
    free(filename);
}

MREP *loadRepresentation(char *basename) {
    MREP *rep;
    int i;
    rep = (MREP *) malloc(sizeof(struct matrixRep));
    rep->btl = (bitRankW32Int *) malloc(sizeof(struct sbitRankW32Int));
    char *filename = (char *) malloc(sizeof(char) * (strlen(basename) + 4));
    strcpy(filename, basename);
    strcat(filename, ".kt");
    FILE *ft = fopen(filename, "r");
    fread(&(rep->numberOfNodes), sizeof(uint), 1, ft);
    fread(&(rep->numberOfEdges), sizeof(ulong), 1, ft);
    fread(&(rep->maxLevel), sizeof(uint), 1, ft);

    fread(&(rep->btl_len), sizeof(uint), 1, ft);
    fread(&(rep->bt_len), sizeof(uint), 1, ft);
    rep->btl->b = 32;
    uint b = rep->btl->b;
    fread(&(rep->btl->factor), sizeof(uint), 1, ft);
    rep->btl->s = b * rep->btl->factor;
    uint s = rep->btl->s;
    rep->btl->n = rep->btl_len;
    uint n = rep->btl->n;
    uint n2 = rep->btl_len;
    rep->btl->integers = n / W;
    rep->btl->data = (uint *) malloc(sizeof(uint) * (n2 / W + 1));


    fread(rep->btl->data, sizeof(uint), n2 / W + 1, ft);
    rep->btl->owner = 1;
    rep->btl->Rs = (uint *) malloc(sizeof(uint) * (n / s + 1));
    fread(rep->btl->Rs, sizeof(uint), n / s + 1, ft);

    rep->info = (uint *) malloc(sizeof(uint) * MAX_INFO);
    rep->element = (uint *) malloc(sizeof(uint) * MAX_INFO);
    rep->basex = (uint *) malloc(sizeof(uint) * MAX_INFO);
    rep->basey = (uint *) malloc(sizeof(uint) * MAX_INFO);
    rep->iniq = -1;
    rep->finq = -1;

    rep->info2[0] = (uint *) malloc(sizeof(uint) * MAX_INFO);

    rep->info2[1] = (uint *) malloc(sizeof(uint) * MAX_INFO);


    rep->div_level_table = (uint *) malloc(sizeof(uint) * (rep->maxLevel + 1));
    for (i = 0; i <= rep->maxLevel; i++)
        rep->div_level_table[i] = exp_pow(K, rep->maxLevel - i);
    free(filename);
    return rep;
}

void destroyRepresentation(MREP *rep) {
    destroyBitRankW32Int(rep->btl);
    if (rep->info != NULL)
        free(rep->info);
    if (rep->info2[0] != NULL)
        free(rep->info2[0]);
    if (rep->info2[1] != NULL)
        free(rep->info2[1]);
    if (rep->element)
        free(rep->element);
    if (rep->basex)
        free(rep->basex);
    if (rep->basey)
        free(rep->basey);
    if (rep->div_level_table)
        free(rep->div_level_table);
    free(rep);
}

uint *compactAdjacencyList(MREP *rep, int x) {
    rep->iniq = -1;
    rep->finq = -1;
    uint posInf;
    uint totalAdyNodes = 0;
    int i, k, j, queuecont, conttmp, node, div_level, xrelat;
    AddItem2(rep, 0, 0, x);
    queuecont = 1;
    for (i = 0; i < rep->maxLevel; i++) {
        conttmp = 0;
        div_level = rep->div_level_table[i];
        for (k = 0; k < queuecont; k++) {
            for (j = 0; j < K; j++) {
                xrelat = rep->basey[rep->iniq];
                node = xrelat / div_level * K + j;
                node += rep->element[rep->iniq];

                if (isBitSet(rep->btl, node)) {
                    conttmp++;
                    if (i == rep->maxLevel - 1)
                        AddItem2(rep, rank1(rep->btl, node) * K * K, rep->basex[rep->iniq] + j * K,
                                 rep->basey[rep->iniq]);
                    else
                        AddItem2(rep, rank1(rep->btl, node) * K * K, rep->basex[rep->iniq] + j * div_level,
                                 rep->basey[rep->iniq] % div_level);
                }
            }
            RemoveItem2(rep);
        }
        queuecont = conttmp;
    }
    while (rep->iniq <= rep->finq) {
        posInf = rep->element[rep->iniq];
        for (i = 0; i < K; i++) {
            if (bitget(rep->btl->data, posInf + (i + (rep->basey[rep->iniq] % K) * K))) {
                totalAdyNodes++;
                rep->info[totalAdyNodes] = rep->basex[rep->iniq] + i;
            }
        }
        RemoveItem2(rep);
    }
    rep->info[0] = totalAdyNodes;
    return rep->info;
}

uint *compactInverseList(MREP *rep, int y) {
    rep->iniq = -1;
    rep->finq = -1;
    uint posInf;

    uint totalAdyNodes = 0;
    int i, k, j, queuecont, conttmp, node, div_level, yrelat;

    AddItem2(rep, 0, y, 0);
    queuecont = 1;
    for (i = 0; i < rep->maxLevel; i++) {
        conttmp = 0;
        div_level = rep->div_level_table[i];
        for (k = 0; k < queuecont; k++) {
            for (j = 0; j < K; j++) {
                yrelat = rep->basex[rep->iniq];
                node = K * j + yrelat / div_level;
                node += rep->element[rep->iniq];
                if (isBitSet(rep->btl, node)) {
                    conttmp++;
                    if (i == rep->maxLevel - 1)
                        AddItem2(rep, rank1(rep->btl, node) * K * K, rep->basex[rep->iniq],
                                 rep->basey[rep->iniq] + j * K);
                    else
                        AddItem2(rep, rank1(rep->btl, node) * K * K, rep->basex[rep->iniq] % div_level,
                                 rep->basey[rep->iniq] + j * div_level);
                }
            }
            RemoveItem2(rep);
        }
        queuecont = conttmp;
    }
    while (rep->iniq <= rep->finq) {
        posInf = rep->element[rep->iniq];
        for (i = 0; i < K; i++) {
            if (bitget(rep->btl->data, posInf + (i * K + (rep->basex[rep->iniq] % K)))) {
                totalAdyNodes++;
                rep->info[totalAdyNodes] = rep->basey[rep->iniq] + i;
            }
        }
        RemoveItem2(rep);
    }

    rep->info[0] = totalAdyNodes;
    return rep->info;
}

void recursiveAdjacencyList(MREP *rep, uint node, uint basex, uint basey, uint level) {
    uint posInf;
    int i, j, div_level, xrelat, newnode;
    if (level < rep->maxLevel) {
        div_level = exp_pow(K, rep->maxLevel - level);
        for (j = 0; j < K; j++) {
            xrelat = basey;
            newnode = xrelat / div_level * K + j;
            newnode += node;
            if (isBitSet(rep->btl, newnode)) {
                if (level < rep->maxLevel - 1)
                    recursiveAdjacencyList(rep, rank1(rep->btl, newnode) * K * K, basex + j * div_level,
                                           basey % div_level, level + 1);
                else {
                    posInf = (rank1(rep->btl, newnode)) * K * K;
                    for (i = 0; i < K; i++) {
                        if (bitget(rep->btl->data, posInf + (i + (basey % K) * K))) {
                            rep->info[0]++;
                            rep->info[rep->info[0]] = basex + i + K * j;
                        }
                    }
                }
            }
        }
    }
}

uint *compact2AdjacencyList(MREP *rep, int x) {
    rep->info[0] = 0;
    recursiveAdjacencyList(rep, 0, 0, x, 0);
    uint *retorno = (uint *) malloc(sizeof(uint) * (rep->info[0] + 1));
    uint i;
    for (i = 0; i <= rep->info[0]; i++) {
        retorno[i] = rep->info[i];
    }
    return retorno;
}

void recursiveRangeQuery(MREP *rep, uint p1, uint p2, uint q1, uint q2, uint dp, uint dq, uint x, int l);

void recursiveRangeQuery(MREP *rep, uint p1, uint p2, uint q1, uint q2, uint dp, uint dq, uint x, int l) {
    int i, j, leaf;
    uint y, divlevel, p1new, p2new, q1new, q2new;

    if (l == rep->maxLevel) {
        for (i = p1; i <= p2; i++) {
            for (j = q1; j <= q2; j++) {
                leaf = x + j;
                if (bitget(rep->btl->data, leaf)) {
                    (rep->info2)[0][0] = (rep->info2)[0][0] + 1;
                    (rep->info2)[0][(rep->info2)[0][0]] = dp + i;
                    (rep->info2)[1][(rep->info2)[0][0]] = dq + j;
                }
            }
            leaf += K;
        }
    }


    if (((l == rep->maxLevel - 1) && (bitget(rep->btl->data, x)))) {
        y = rank1(rep->btl, x) * K * K;

        for (i = p1; i <= p2; i++) {
            for (j = q1; j <= q2; j++) {
                recursiveRangeQuery(rep, 0, 0, 0, 0, dp + i, dq + j, y + K * i + j, l + 1);
            }
        }
    }

    if ((x == -1) || ((l < rep->maxLevel - 1) && (bitget(rep->btl->data, x)))) {
        y = (rank1(rep->btl, x)) * K * K;

        divlevel = rep->div_level_table[l + 1];
        for (i = p1 / divlevel; i <= p2 / divlevel; i++) {
            p1new = 0;
            if (i == p1 / divlevel)
                p1new = p1 % divlevel;

            p2new = divlevel - 1;
            if (i == p2 / divlevel)
                p2new = p2 % divlevel;

            for (j = q1 / divlevel; j <= q2 / divlevel; j++) {
                q1new = 0;
                if (j == q1 / divlevel)
                    q1new = q1 % divlevel;

                q2new = divlevel - 1;
                if (j == q2 / divlevel)
                    q2new = q2 % divlevel;

                recursiveRangeQuery(rep, p1new, p2new, q1new, q2new, dp + divlevel * i, dq + divlevel * j,
                                    y + K * i + j, l + 1);
            }
        }
    }
}

void recursiveRangeQuery2(MREP *rep, uint p1, uint p2, uint q1, uint q2, uint dp, uint dq, uint x, int l);

void recursiveRangeQuery2(MREP *rep, uint p1, uint p2, uint q1, uint q2, uint dp, uint dq, uint x, int l, Punto &queryPoint, std::priority_queue<Punto, vector<Punto>, ComparePoint>& minHeap, int k, int typeDistance) {
    int i, j, leaf;
    uint y, divlevel, p1new, p2new, q1new, q2new;
    if (l == rep->maxLevel) {
        for (i = p1; i <= p2; i++) {
            for (j = q1; j <= q2; j++) {
                leaf = x + j;
                if (bitget(rep->btl->data, leaf)) {
                  //  cout << "Punto find is: (" << dp+i << "," << dq+j << ")" << endl;

                        /****** codigo agregado ************/
                                    Punto aux(dp + i,  dq + j, 0);
                                    double  distancia = (typeDistance == 1)?calcularDistanciaEuclidiana (aux, queryPoint) : calcularDistanciaManhattan(aux, queryPoint);
                                    aux.distancia = distancia;
                                    if (minHeap.size() < k) {
                                        minHeap.push(aux);
                                    } else {
                                    if (aux.distancia < minHeap.top().distancia) {
                                            minHeap.pop();
                                            minHeap.push(aux);
                                        }
                                    }
                        /****** FIN codigo agregado ************/
                        (rep->info2)[0][0] = (rep->info2)[0][0] + 1;
                        (rep->info2)[0][(rep->info2)[0][0]] = dp + i;
                        (rep->info2)[1][(rep->info2)[0][0]] = dq + j;
                }
            }
            leaf += K;
        }
    }
    if (((l == rep->maxLevel - 1) && (bitget(rep->btl->data, x)))) {
        y = rank1(rep->btl, x) * K * K;

        for (i = p1; i <= p2; i++) {
            for (j = q1; j <= q2; j++) {
                recursiveRangeQuery2(rep, 0, 0, 0, 0, dp + i, dq + j, y + K * i + j, l + 1, queryPoint, minHeap, k, typeDistance);
            }
        }
    }

    if ((x == -1) || ((l < rep->maxLevel - 1) && (bitget(rep->btl->data, x)))) {
        y = (rank1(rep->btl, x)) * K * K;

        divlevel = rep->div_level_table[l + 1];
        for (i = p1 / divlevel; i <= p2 / divlevel; i++) {
            p1new = 0;
            if (i == p1 / divlevel)
                p1new = p1 % divlevel;

            p2new = divlevel - 1;
            if (i == p2 / divlevel)
                p2new = p2 % divlevel;

            for (j = q1 / divlevel; j <= q2 / divlevel; j++) {
                q1new = 0;
                if (j == q1 / divlevel)
                    q1new = q1 % divlevel;

                q2new = divlevel - 1;
                if (j == q2 / divlevel)
                    q2new = q2 % divlevel;

                recursiveRangeQuery2(rep, p1new, p2new, q1new, q2new, dp + divlevel * i, dq + divlevel * j,
                                    y + K * i + j, l + 1,queryPoint,minHeap, k, typeDistance);
            }
        }
    }
}


uint **compactRangeQuery(MREP *rep, uint p1, uint p2, uint q1, uint q2) {
    rep->info2[0][0] = 0;
    recursiveRangeQuery(rep, p1, p2, q1, q2, 0, 0, -1, -1);
    return rep->info2;
}

uint ** compactRangeQuery2(MREP *rep, uint p1, uint p2, uint q1, uint q2, int k, uint x, uint y, std::vector<std::pair<int, int>>& puntillos, int typeDistance){
/*
 * Este compact RangeQuery2 , es semejante al RangeQuery, pero con diferencia, que le digo  cuantos puntos debe devolver la consulta.
 * estos k puntos a devolver, son los mas cercanos al punto [x,y].
 * esta función es utilizada exclusivamente para optimizar el tiempo del KNN por radio
 * */
  /*  cout << "Punto de consulta q es: (" << x << "," << y << ")" << endl;
    std::cout << "p1: " << p1 << ", p2: " << p2 << ", q1: " << q1 << ", q2: " << q2 << std::endl;*/

    rep->info2[0][0] = 0;
    priority_queue<Punto, vector<Punto>, ComparePoint> minHeap;
    Punto queryPoint(x,y, 0);

   recursiveRangeQuery2(rep, p1, p2, q1, q2, 0, 0, -1, -1, queryPoint, minHeap, k, typeDistance);
    while(!minHeap.empty()) {
        struct Punto p = minHeap.top(); // Obtener el elemento superior
        minHeap.pop(); // Eliminar el elemento superior
        puntillos.push_back(make_pair(p.x, p.y));
    }


    return rep->info2;
}

uint recursiveCheckLinkQuery(MREP *rep, uint p, uint q, uint node, uint level);

uint recursiveCheckLinkQuery(MREP *rep, uint p, uint q, uint node, uint level) {
    uint posInf;

    int div_level, newnode;

    div_level = rep->div_level_table[level];

    newnode = p / div_level * K + q / div_level;
    newnode += node;
    if (isBitSet(rep->btl, newnode)) {
        if (level < rep->maxLevel - 1) {

            return recursiveCheckLinkQuery(rep, p % div_level, q % div_level, rank1(rep->btl, newnode) * K * K,
                                           level + 1);
        } else {
            posInf = (rank1(rep->btl, newnode)) * K * K;
            if (bitget(rep->btl->data, posInf + (q % K + (p % K) * K))) {
                return 1;
            }

        }
    }
    return 0;
}

uint compact2CheckLinkQuery(MREP *rep, uint p, uint q) {
    return recursiveCheckLinkQuery(rep, p, q, 0, 0);
}
uint recursiveCheckRangeQuery(MREP *rep, uint p1, uint p2, uint q1, uint q2, uint dp, uint dq, uint x, int l);

uint recursiveCheckRangeQuery(MREP *rep, uint p1, uint p2, uint q1, uint q2, uint dp, uint dq, uint x, int l) {
    int i, j, leaf, result;
    uint y, divlevel, divlevel2, p1new, p2new, q1new, q2new;

    if (l == rep->maxLevel) {
        for (i = p1; i <= p2; i++) {
            for (j = q1; j <= q2; j++) {
                leaf = x + j;
                if (bitget(rep->btl->data, leaf)) {
                    return 1;
                }
            }
            leaf += K;
        }
        return 0;
    }

    if (((l == rep->maxLevel - 1) && (bitget(rep->btl->data, x)))) {
        divlevel = rep->div_level_table[l + 1];
        divlevel2 = rep->div_level_table[l];

        if ((p1 == 0) && (q1 == 0) && (p2 == divlevel2 - 1) && (q2 == divlevel2 - 1)) {
            return 1;
        }

        y = (rank1(rep->btl, x)) * K * K;

        for (i = p1; i <= p2; i++) {
            for (j = q1; j <= q2; j++) {
                result = recursiveCheckRangeQuery(rep, 0, 0, 0, 0, dp + i, dq + j, y + K * i + j, l + 1);
                if (result) {
                    return 1;
                }
            }
        }
        return 0;
    }

    ///else{ //internal node
    if ((x == -1) || ((l < rep->maxLevel - 1) && (bitget(rep->btl->data, x)))) {
        divlevel = rep->div_level_table[l + 1];
        if (l >= 0) {
            divlevel2 = rep->div_level_table[l];
            if ((p1 == 0) && (q1 == 0) && (p2 == divlevel2 - 1) && (q2 == divlevel2 - 1)) {
                return 1;
            }
        }

        y = (rank1(rep->btl, x)) * K * K;

        for (i = p1 / divlevel; i <= p2 / divlevel; i++) {
            p1new = 0;
            if (i == p1 / divlevel)
                p1new = p1 % divlevel;

            p2new = divlevel - 1;
            if (i == p2 / divlevel)
                p2new = p2 % divlevel;


            for (j = q1 / divlevel; j <= q2 / divlevel; j++) {
                q1new = 0;
                if (j == q1 / divlevel)
                    q1new = q1 % divlevel;

                q2new = divlevel - 1;
                if (j == q2 / divlevel)
                    q2new = q2 % divlevel;

                result = recursiveCheckRangeQuery(rep, p1new, p2new, q1new, q2new, dp + divlevel * i, dq + divlevel * j,
                                                  y + K * i + j, l + 1);

                if (result) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

uint compactCheckRangeQuery(MREP *rep, uint p1, uint p2, uint q1, uint q2) {
    return recursiveCheckRangeQuery(rep, p1, p2, q1, q2, 0, 0, -1, -1);
}

uint compactCheckLinkQuery(MREP *rep, uint p, uint q) {

    uint posInf;
    uint prelat, qrelat;
    int i, node = 0, div_level;

    for (i = 0; i < rep->maxLevel; i++) {


        div_level = rep->div_level_table[i];

        prelat = p / div_level;
        qrelat = q / div_level;

        node = node + prelat * K + qrelat;
        if (isBitSet(rep->btl, node) == 0) {
            return 0;
        }
        node = rank1(rep->btl, node) * K * K;
        p = p % div_level;
        q = q % div_level;
    }

    posInf = node;
    if (bitget(rep->btl->data, posInf + (q + (p) * K))) {
        return 1;
    }

    return 0;
}

/*
void recursiveDecompression(MREP * rep, uint * tmp_in, uint * tmp_out, ulong * tmp_p, ulong * sizein, uint dp, uint dq,uint x,int l);

void recursiveDecompression(MREP * rep, uint * tmp_in, uint * tmp_out, ulong * tmp_p, ulong * sizein, uint dp, uint dq,uint x,int l){
	int i,j;
	uint y, divlevel;
	if(l==rep->maxLevel){	
			if(bitget(rep->btl->data,x)){
					tmp_in[*tmp_p]=dp;
					tmp_out[*tmp_p]=dq;
					sizein[dp]++;
					(*tmp_p)++;
				}
	}
		
		
	if(((l==rep->maxLevel-1)&&(bitget(rep->btl->data,x)))){
		y=rank(rep->btl,x)*K*K;
		
		for(i=0;i<K;i++){
 				for(j=0;j<K;j++){
	 				recursiveDecompression(rep,tmp_in, tmp_out, tmp_p,sizein,dp + i,dq+j,y+K*i+j,l+1);	 		
 			}
		}
 			

		
	}
 		if((x==-1)||((l<rep->maxLevel-1)&&(bitget(rep->btl->data,x)))){
 			y = (rank(rep->btl,x))*K*K;
 			
 			divlevel =	rep->div_level_table[l+1];
			for(i=0;i<K;i++){
 				for(j=0;j<K;j++){
	 				recursiveDecompression(rep,tmp_in, tmp_out, tmp_p,sizein,dp + divlevel*i,dq+divlevel*j,y+K*i+j,l+1);	 		
 				}
 			}
 			
 		}
 	}
		
int * compactFullDecompression(MREP * rep){
	ulong p = 0;
	uint * in = (uint *) malloc(sizeof(uint)*rep->numberOfEdges);
	uint * out = (uint *) malloc(sizeof(uint)*rep->numberOfEdges);
	ulong * sizein = (ulong *) malloc(sizeof(ulong)*(rep->numberOfNodes+1));
	int i;
	for (i=0;i<rep->numberOfNodes;i++)
		sizein[i]=0;
	recursiveDecompression(rep, in, out, &p, sizein, 0,0,-1,-1);
	
	int *lista = (int *) malloc(sizeof(int)*(rep->numberOfEdges+rep->numberOfNodes));

	ulong suma=0;
	ulong tmp = 0;
	for (i=0;i<rep->numberOfNodes;i++){
		tmp = sizein[i];
		lista[suma]=-(i+1);
		sizein[i]=suma+1;
		suma = suma + tmp+1;
	}

	sizein[rep->numberOfNodes] = suma;
	
	for(i=0;i<rep->numberOfEdges;i++){
		lista[sizein[in[i]]] = out[i]+1;
		sizein[in[i]]++;
	}

	return lista;
	
}

*/

void recursiveDecompression(MREP *rep, uint *tmp_in, uint *tmp_out, ulong *tmp_p, ulong *sizein, ulong *pointerL, uint dp,uint dq, uint x, int l);

void recursiveDecompression(MREP *rep, uint *tmp_in, uint *tmp_out, ulong *tmp_p, ulong *sizein, ulong *pointerL, uint dp,uint dq, uint x, int l) {
    int i, j;
    uint y, divlevel;
    if (l == rep->maxLevel) {
        if (bitget(rep->btl->data, x)) {
            tmp_in[*tmp_p] = dp;
            tmp_out[*tmp_p] = dq;
            sizein[dp]++;
            (*tmp_p)++;
        }
    }


    if (((l == rep->maxLevel - 1) && (bitget(rep->btl->data, x)))) {
        //y=rank(rep->btl,x)*K*K;
        y = pointerL[l + 1];
        pointerL[l + 1] += K * K;
        for (i = 0; i < K; i++) {
            for (j = 0; j < K; j++) {
                recursiveDecompression(rep, tmp_in, tmp_out, tmp_p, sizein, pointerL, dp + i, dq + j, y + K * i + j,
                                       l + 1);
            }
        }
    }

    if ((x == -1) || ((l < rep->maxLevel - 1) && (bitget(rep->btl->data, x)))) {
        y = pointerL[l + 1];
        pointerL[l + 1] += K * K;

        divlevel = rep->div_level_table[l + 1];
        for (i = 0; i < K; i++) {
            for (j = 0; j < K; j++) {
                recursiveDecompression(rep, tmp_in, tmp_out, tmp_p, sizein, pointerL, dp + divlevel * i,
                                       dq + divlevel * j, y + K * i + j, l + 1);
            }
        }

    }
}

int *compactFullDecompression(MREP *rep) {
    ulong p = 0;
    uint *in = (uint *) malloc(sizeof(uint) * rep->numberOfEdges);
    uint *out = (uint *) malloc(sizeof(uint) * rep->numberOfEdges);
    ulong *sizein = (ulong *) malloc(sizeof(ulong) * (rep->numberOfNodes + 1));
    int i;
    for (i = 0; i < rep->numberOfNodes; i++)
        sizein[i] = 0;

    ulong *pointerL = (ulong *) malloc(sizeof(ulong) * (rep->numberOfNodes + 1));

    pointerL[0] = 0;
    pointerL[1] = K * K;
    for (i = 2; i <= rep->maxLevel; i++) {
        pointerL[i] = (rank1(rep->btl, pointerL[i - 1] - 1) + 1) * K * K;
    }

    recursiveDecompression(rep, in, out, &p, sizein, pointerL, 0, 0, -1, -1);

    int *lista = (int *) malloc(sizeof(int) * (rep->numberOfEdges + rep->numberOfNodes));

    ulong suma = 0;
    ulong tmp = 0;
    for (i = 0; i < rep->numberOfNodes; i++) {
        tmp = sizein[i];
        lista[suma] = -(i + 1);
        sizein[i] = suma + 1;
        suma = suma + tmp + 1;
    }

    sizein[rep->numberOfNodes] = suma;

    for (i = 0; i < rep->numberOfEdges; i++) {
        lista[sizein[in[i]]] = out[i] + 1;
        sizein[in[i]]++;
    }

    return lista;
}
