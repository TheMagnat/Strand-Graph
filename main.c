
#include "hashmap.h"


#include "graph.h"

#define GRAPH_FILE "london.dat"


int main(int argc, char const *argv[]){
	
	Graph myGraph;

	map_t myMap = hashmap_new();



	// hashmap_put(myMap, "Salut", 10);

	// int test;

	// hashmap_get(myMap, "Salut", &test);




	fillGraph(&myGraph, GRAPH_FILE);

	printf("PRINT GRAPH : \n\n");
	printGraph(&myGraph);

	freeGraph(&myGraph);

	hashmap_free(myMap);

	return 0;
}