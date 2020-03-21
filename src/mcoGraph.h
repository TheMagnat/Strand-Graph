
#ifndef MCO_GRAPH_H
#define MCO_GRAPH_H

#include <stdlib.h>
#include <stdio.h>

#include "searchingTree.h"


//This constant contain the max size of a char buffer
#define MAXCHAR 1024

struct NodeAndLine {
	int node;
	int line;
};
typedef struct NodeAndLine NodeAndLine;

struct NodePath{

	NodeAndLine *nodes;
	unsigned int size;

};
typedef struct NodePath NodePath;



struct costAndLine{

	int *donePerLine;

	unsigned int line;
	unsigned int *costPerLine;

};
typedef struct costAndLine costAndLine;



struct Matrix{

	char **stationName;

	char **lineName;
	unsigned int nbLine;

	unsigned int *data;

	unsigned int size;
	unsigned int width;

};
typedef struct Matrix Matrix;


NodePath* dijkstraMatrix(Matrix* graph, unsigned int start, unsigned int end);

/**
 * To fill a matrix graph using a file.
 * It also require a SearchingTree to fill all the station name.
*/
void fillMatrixGraph(Matrix* toFill, SearchingTree* wordTree, char* filename);

void printMatrixGraph(Matrix* toPrint);

/**
 * To free a Matrix Graph
*/
void freeMatrixGraph(Matrix* toFree);


#endif
