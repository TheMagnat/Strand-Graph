
#include "searchingTree.h"
#include "graph.h"
#include "mcoGraph.h"

#include <time.h>

#define GRAPH_FILE "london.dat"


//int main(int argc, char const *argv[]){

//Empty for no warning
int main(){
	
	char buffer[1024];
	int start, end, tempo;

	clock_t startClock, endClock;
	double firstTime, secondTime;
	
	SearchingTree myTree;

	Graph myGraph;

	Matrix myMco;

	NodePath* nodePath;
	BrandPath* path;


	initTree(&myTree);

	fillMatrixGraph(&myMco, GRAPH_FILE, &myTree, 1);
	fillGraph(&myGraph, GRAPH_FILE, &myTree, 0);

	///Debug
	// printf("PRINT MCO : \n\n");
	// printMatrixGraph(&myMco);

	///Debug
	// printf("PRINT GRAPH : \n\n");
	// printGraph(&myGraph);

	///Debug
	// printf("PRINT TREE : \n\n");
	// printTree(&myTree);

	while(1){
		
		printf("Merci d'entrer une station de départ :\n");

		scanf("%1024s", buffer);
		fpurge(stdin);

		printf("\n");

		start = findWord(&myTree, buffer, 1);

		if(start != -1){
			printf("Vous partez de %s\n", buffer);
			break;
		}
		
	}
	printf("\n");
	while(1){
		
		printf("Merci d'entrer une station de d'arrivé :\n");

		scanf("%1024s", buffer);
		fpurge(stdin);

		printf("\n");

		end = findWord(&myTree, buffer, 1);

		if(end != -1){
			printf("Vous voulez aller à : %s\n", buffer);
			break;
		}
		
	}
	printf("\n");
	

	printf("Veuillez choisir une option :\n");
	printf("0 : Recherche sur le Graph à Brins.\n1 : Recherche sur le Graph de Matrices.\n2 : Comparez les deux en temps.\n");

	scanf("%d", &tempo);

	if(tempo == 0){
		path = dijkstra(&myGraph, start, end);

		if(path != NULL){

			humanPrintBrandPath(&myGraph, path);

			free(path->brandsIndex);
			free(path); ///CRASH ICI

		}
		else{
			printf("This should never happen.\n");
		}
	}
	else if(tempo == 1){
		nodePath = dijkstraMatrix(&myMco, start, end);

		if(nodePath){
			humanPrintNodePath(&myMco, nodePath);

			free(nodePath->nodes);
			free(nodePath);

		}
		else{
			printf("This should never happen.\n");
		}

	}
	else{
		
		startClock = clock();
		path = dijkstra(&myGraph, start, end);
		endClock = clock();

		firstTime = ((double) (endClock - startClock)) / CLOCKS_PER_SEC;


		startClock = clock();
		nodePath = dijkstraMatrix(&myMco, start, end);
		endClock = clock();

		secondTime = ((double) (endClock - startClock)) / CLOCKS_PER_SEC;

		printf("Graph à Brins :\t\t %f secondes.\nGraph de Matrices :\t %f secondes.\n", firstTime, secondTime);

	}
		

	freeGraph(&myGraph);
	freeMatrixGraph(&myMco);
	deleteTree(&myTree);

	return 0;
}
