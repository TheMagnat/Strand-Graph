
#include "searchingTree.h"
#include "graph.h"

#define GRAPH_FILE "london.dat"


int main(int argc, char const *argv[]){
	
	char buffer[1024];
	int tempo;
	
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




	fillGraph(&myGraph, &myTree, GRAPH_FILE);

	printf("PRINT GRAPH : \n\n");
	//printGraph(&myGraph);

	freeGraph(&myGraph);

	// printf("Map size: %d\n", hashmap_length(AllStations));

	printf("PRINT TREE : \n\n");

	//printTree(&myTree);

	while(1){
		
		printf("Please enter a subway station name :\n");

		scanf("%1024s", buffer);
		fpurge(stdin);

		printf("\n");

		tempo = findWord(&myTree, buffer, 1);

		if(tempo != -1){
			printf("%s selected, index %d\n", buffer, tempo);
		}
		else{
			printf("%s is not valid.\n", buffer);
		}
		
	}

	//autoCompleteWord(&myTree, "Black");

	return 0;
}