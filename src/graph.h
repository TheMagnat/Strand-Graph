
#ifndef GRAPH_H
#define GRAPH_H


#include <stdlib.h>
#include <stdio.h>

#include "searchingTree.h"


//This constant contain the max size of a char buffer
#define MAXCHAR 1024




/*
	A Strand contain the number of the vertice it's on,
	and the number of the next strand of this vertice.
*/
struct Strand{ //Brin
	int vertice;
	int nextStrand;

	//If it's neg or pos
	int type;
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
	An edge contain 2 Strand,
	One for the start of the edge and one for the end of the edge.
*/
struct Edge{ // Arête

	Strand from;
	Strand to;

};
typedef struct Edge Edge;

/*
	A Graph contain 2 vector, one of vertices and one of edges.
*/
struct Graph{

	Vertice* vertices;
	unsigned int nbVertice;

	Edge* edges;
	unsigned int nbEdge;

	Strand* strands;
	unsigned int nbStrand;

};
typedef struct Graph Graph;


void fillGraph(Graph* toFill, SearchingTree* wordTree, char* filename);

void printGraph(Graph* graph);

void printLinearGraph(Graph* graph);

void freeGraph(Graph* toFree);

#endif