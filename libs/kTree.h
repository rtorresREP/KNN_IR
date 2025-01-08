#ifndef KTREE_H
#define KTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitrankw32int.h"
#include "basic.h"
#include <vector>
#include <utility>
#include <iostream>

#define MAX_INFO 1024*1024+10
#define K 2

typedef struct matrixRep
{
    bitRankW32Int * btl;        //Bitmap representando T:L
    uint btl_len;               //Numero de bits de T:L
    uint bt_len;                //Numero de bits de T
    int maxLevel;           //Nivel maximo del arbol
    uint numberOfNodes;
    ulong numberOfEdges;
    uint * div_level_table;
    uint * info;
    uint * info2[2];
    uint * element;
    uint * basex;
    uint * basey;
    int iniq;
    int finq;
}MREP;


MREP * compactCreateKTree(uint * xedges, uint *yedges, uint numberOfNodes,ulong numberOfEdges, uint maxl);

uint checkLink(MREP * rep, uint x, uint y);
uint callerRangeCount(MREP * rep, uint xi, uint xf, uint yi, uint yf);
uint callerCompactCount(MREP * rep, uint xi, uint xf, uint yi, uint yf);
uint callerCircularRangeCount(MREP * rep, uint x, uint y, uint r);
uint callerCircularCompactCount(MREP * rep, uint x, uint y, uint r);
uint **callerHeapRange(MREP * rep, uint xi, uint xf, uint yi, uint yf, uint k,uint x, uint y);
uint **callerCircularHeapRange(MREP * rep, uint qx, uint qy, uint r, uint k,uint x, uint y);
uint **KNN_IR_CHEV(MREP * rep, uint x, uint y, uint k, double step, double inc);
uint **KNN_IR_EUC(MREP * rep, uint x, uint y, uint k, double step, double inc);

uint * compactAdjacencyList(MREP * rep, int x);
uint * compact2AdjacencyList(MREP * rep, int x);
uint * compactInverseList(MREP * rep, int y);
uint ** compactRangeQuery(MREP * rep, uint p1, uint p2, uint q1, uint q2);
uint ** compactRangeQuery2(MREP *rep, uint p1, uint p2, uint q1, uint q2, int k, uint x, uint y, std::vector<std::pair<int, int>>& puntillos, int typeDistance);
uint compactCheckLinkQuery(MREP * rep, uint p, uint q);
uint compact2CheckLinkQuery(MREP * rep, uint p, uint q);
uint compactCheckRangeQuery(MREP * rep, uint p1, uint p2, uint q1, uint q2);

int * compactFullDecompression(MREP * rep);

MREP * loadRepresentation(char * basename);
void saveRepresentation(MREP * rep, char * basename);
void destroyRepresentation(MREP * rep);


#endif
