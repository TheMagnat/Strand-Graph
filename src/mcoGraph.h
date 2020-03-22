
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


/**
 * This structure help us to know if a node is done on certain line,
 * And if it's not done on that line, the old cost of the path to this node.
*/
struct CostAndDone{

	int *donePerLine;

	unsigned int *costPerLine;

};
typedef struct CostAndDone CostAndDone;



struct Matrix{

	char **stationName;

	char **lineName;
	unsigned int nbLine;

	unsigned int *data;

	unsigned int size;
	unsigned int width;

};
typedef struct Matrix Matrix;


/**
 * Pretty printer for to print a NodePath.
*/
void humanPrintNodePath(Matrix* graph, NodePath* path);

/**
 * The dijkstra algorithm for a Matrix graph.
 * 
 * @param start Starting Node.
 * @param end ending Node.
 * 
 * @return A NodePath structure that contain the path from start to end.
 */
NodePath* dijkstraMatrix(Matrix* graph, unsigned int start, unsigned int end);

/**
 * To fill a matrix graph using a file.
 * It also require a SearchingTree to fill all the station name if fillWord is not equal to 0.
*/
void fillMatrixGraph(Matrix* toFill, char* filename, SearchingTree* wordTree, uint8_t fillWord);

void printMatrixGraph(Matrix* toPrint);

/**
 * To free a Matrix Graph
*/
void freeMatrixGraph(Matrix* toFree);


#endif
