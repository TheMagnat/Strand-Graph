
#include "searchingTree.h"
#include "graph.h"
#include "mcoGraph.h"

#define GRAPH_FILE "london.dat"


//int main(int argc, char const *argv[]){

//Empty for no warning
int main(){
	
	char buffer[1024];
	int start, end;
	
	SearchingTree myTree;
	Graph myGraph;

	Matrix myMco;

	initTree(&myTree);

	fillMatrixGraph(&myMco, &myTree, GRAPH_FILE);

	//printMatrixGraph(&myMco);


	dijkstraMatrix(&myMco, 0, 50);

	// fillGraph(&myGraph, &myTree, GRAPH_FILE);

	// ///Debug
	// // printf("PRINT GRAPH : \n\n");
	// // printGraph(&myGraph);

	// ///Debug
	// //printf("PRINT TREE : \n\n");
	// //printTree(&myTree);

	// while(1){
		
	// 	printf("Merci d'entrer une station de départ :\n");

	// 	scanf("%1024s", buffer);
	// 	fpurge(stdin);

	// 	printf("\n");

	// 	start = findWord(&myTree, buffer, 1);

	// 	if(start != -1){
	// 		printf("Vous partez de %s\n", buffer);
	// 		break;
	// 	}
		
	// }
	// printf("\n");
	// while(1){
		
	// 	printf("Merci d'entrer une station de d'arrivé :\n");

	// 	scanf("%1024s", buffer);
	// 	fpurge(stdin);

	// 	printf("\n");

	// 	end = findWord(&myTree, buffer, 1);

	// 	if(end != -1){
	// 		printf("Vous voulez aller à : %s\n", buffer);
	// 		break;
	// 	}
		
	// }
	// printf("\n");
	
	// BrandPath* path;
	
	// //printf("Start dijkstra :\n");
	// path = dijkstra(&myGraph, start, end);


	// if(path != NULL){

	// 	humanPrintBrandPath(&myGraph, path);

	// 	//Crash ici si 'Kenton' vers 'Latimer_Road' choisi.
	// 	printf("Free brandsIndex of path\n");
	// 	free(path->brandsIndex);
	// 	printf("Free path\n");
	// 	free(path); ///CRASH ICI
	// 	printf("done\n");

	// }
	// else{
	// 	printf("This should never happen.\n");
	// }

	// freeGraph(&myGraph);
	// deleteTree(&myTree); //Crash ici aussi si on ne fait pas un test bizare cf la fonction deleteTree
	// //Ces deux crash semble similaire

	return 0;
}
