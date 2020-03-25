
#ifndef GRAPH_H
#define GRAPH_H


#include <stdlib.h>
#include <stdio.h>

#include "searchingTree.h"


//This constant contain the max size of a char buffer
#define MAXCHAR 1024


struct BrandPath{

	int* brandsIndex;
	unsigned int size;

};
typedef struct BrandPath BrandPath;



/**
 * A Strand contain the number of the vertice it's on,
 * and the index of the next strand of this vertice.
 * To access to it's even strand index,
 * you have to take a brand index and add type (1 or -1).
 * lineId correspond to the subway line it's on.
*/
struct Strand{ //Brin
	unsigned int vertice;
	int nextStrand;
	
	//If it's neg or pos
	int type;

	unsigned int lineId;

};
typedef struct Strand Strand;


/*
	A vertice contain the number of it's first strand.
*/
struct Vertice{ // Sommet
	int firstStrand;

	char* label;
};
typedef struct Vertice Vertice;


/*
	A Graph contain 2 vector, one of vertices and one of edges.
*/
struct Graph{

	Vertice* vertices;
	unsigned int nbVertice;

	Strand* strands;
	unsigned int nbStrand;


	char** idToLinesName;
	unsigned int nbLine;


};
typedef struct Graph Graph;


/**
 * To fill a graph using a file.
 * It also require a SearchingTree to fill all the station name.
*/
void fillGraph(Graph* toFill, char* filename, SearchingTree* wordTree, uint8_t fillWord);

///Debug function to print a graph
void printGraph(Graph* graph);

///Debug function to print a graph
void printLinearGraph(Graph* graph);

/**
 * To free a Graph
*/
void freeGraph(Graph* toFree);


///
/**
 * Implementation of the dijkastra function.
 * 
 * @param start Starting Node.
 * @param end ending Node.
 * 
 * @param timeOrChange This parameter allow us to select if you prefer the less time or the less line change. On 0 it mean time, on any other it mean line change.
 *
 * @return A BandPath structure that contain the path from start to end.
*/
BrandPath* dijkstra(Graph* graph, unsigned int start, unsigned int end, uint8_t timeOrChange);

/**
 * To print a pretty brand path.
*/
void humanPrintBrandPath(Graph* graph, BrandPath* path);

#endif
