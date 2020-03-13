
#include "searchingTree.h"
#include "graph.h"

#define GRAPH_FILE "london.dat"


int main(int argc, char const *argv[]){
	
	Graph myGraph;

	SearchingTree myTree;

	initTree(&myTree);

	//Testing Searching Tree
	// addWord(&myTree, "UnMot");
	// addWord(&myTree, "autreMot");
	// addWord(&myTree, "lol");
	// addWord(&myTree, "UnGros");
	// addWord(&myTree, "UnMolle");

	// printTree(&myTree);

	// hashmap_put(AllStations, "Salut", 10);

	// int test;

	// hashmap_get(AllStations, "Salut", &test);




	// fillGraph(&myGraph, GRAPH_FILE);

	// printf("PRINT GRAPH : \n\n");
	// printGraph(&myGraph);

	// freeGraph(&myGraph);

	//printf("Map size: %d\n", hashmap_length(AllStations));


	return 0;
}