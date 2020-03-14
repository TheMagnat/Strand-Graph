
#include "graph.h"

#include <string.h>

//Cost of movement
#define CHANGE_LINE 9
#define TWO_STATION 9


void dijkstra(Graph* graph, unsigned int start, unsigned int end){

	Vertice* actualNode;
	Strand *actualStrand, *pairStrand;

	int i;
	int tempoNodeIndex, tempoValue;
	int actualStrandIndex, actualLineId;

	int actualCost, newCost;

	actualNode = &graph->vertices[start];


	//TO DO
	//Find a way to save the best path to a brand. (instead of -1 store brand -1 * (index + 1) ? )

	//0 if not checked, a value > 0 if checked a dist is evaluated, -1 if done.
	int* visitedStrand;

	visitedStrand = malloc(graph->nbStrand * sizeof(int));
	for(i = 0; i < graph->nbVertice; ++i){
		visitedStrand[i] = 0;
	}

	actualCost = 0;
	actualLineId = -1;
	while(1){
		
		actualStrandIndex = actualNode->firstStrand;

		do{
			
			actualStrand = &graph->strands[actualStrandIndex];
			
			//It's pair strand.
			pairStrand = &graph->strands[actualStrandIndex + actualStrand->type];
			
			//tempoNodeIndex = pairStrand->vertice;
			
			tempoValue = visitedStrand[actualStrandIndex + actualStrand->type];

			//The strand is not done
			if(tempoValue != -1){
				
				newCost = actualCost + TWO_STATION;
				if(actualStrand->lineId != actualLineId) newCost += CHANGE_LINE;

				//If the checked strand got a higher value or if it's not checked (0).
				if(tempoValue > newCost){
				/*
					Store this possibility as a pair of :
						int (the cost to here) and int (index of the strand of arrival = (actualStrandIndex + actualStrand->type))
				
					in a sorted list, smallest first.
				*/
				}
			 	else{

				/*
					Store this possibility as a pair of :
						int (the cost to here) and int (index of the strand of arrival = (actualStrandIndex + actualStrand->type))
				
					in a sorted list, smallest first.
				*/

				}


			}



			
			actualStrandIndex = actualStrand->nextStrand;

		}
		while(actualStrandIndex != -1);

		//Mark the actual node index to finished.



		/*
			Now select the first possibility in the sorted list (smallest first)

			actualStrandIndex = winnerStrand

		*/

		actualStrand = &graph->strands[actualStrandIndex];

		if(actualStrand->vertice == end){
			printf("END\n");
			break;
		}

		actualNode = &graph->vertices[actualStrand->vertice];

		//Mark the selected strand and it's par as done.
		visitedStrand[actualStrandIndex] = -1;
		visitedStrand[actualStrandIndex + actualStrand->type] = -1;

	}


}

void addSubway(char* str, Graph* toFill, unsigned int lineId){

	int nbNode, from, to, i;

	const char s[3] = " \n";
	char *token;


	token = strtok(str, s);
	nbNode = atoi(token);

	if(nbNode < 2){
		exit(1);
	}

	token = strtok(NULL, s);

	from = atoi(token);
	for(i = 1; i < nbNode; ++i){

		token = strtok(NULL, s);
		to = atoi(token);

		//We fill the two strands :

		//The vertice is the starting of the edge
		toFill->strands[toFill->nbStrand].vertice = from;
		//The next strand is the old first strand of the vertice...
		toFill->strands[toFill->nbStrand].nextStrand = toFill->vertices[from].firstStrand;
		/*
			The way to access the other strand of the edge.
			Note : You can access this strand by doing graph.strands[thisStrandIndex+1]
		*/
		toFill->strands[toFill->nbStrand].type = 1;
		
		toFill->strands[toFill->nbStrand].lineId = lineId;

		// printf("%d Strand : vert = %d, first = %d, type = %d\n", toFill->nbStrand, toFill->strands[toFill->nbStrand].vertice, toFill->strands[toFill->nbStrand].nextStrand, toFill->strands[toFill->nbStrand].type);

		/*
			...and the old first strand of the vertice become the actual strand,
			it's like a front chained list
		*/
		toFill->vertices[from].firstStrand = toFill->nbStrand;


		++toFill->nbStrand;
		
		//The vertice is the end of the edge
		toFill->strands[toFill->nbStrand].vertice = to;
		//Same comment as above

		//printf("to : %d, first : %d\n", to, toFill->vertices[to].firstStrand);
		toFill->strands[toFill->nbStrand].nextStrand = toFill->vertices[to].firstStrand;
		/*
			The way to access the other strand of the edge.
			Note : You can access this strand by doing graph.strands[thisStrandIndex-1]
		*/
		toFill->strands[toFill->nbStrand].type = -1;
		
		toFill->strands[toFill->nbStrand].lineId = lineId;
		
		//Same comment as above
		toFill->vertices[to].firstStrand = toFill->nbStrand;
		
		// printf("%d Strand : vert = %d, first = %d, type = %d\n", toFill->nbStrand, toFill->strands[toFill->nbStrand].vertice, toFill->strands[toFill->nbStrand].nextStrand, toFill->strands[toFill->nbStrand].type);

		++toFill->nbStrand;


		from = to;

	}

	token = strtok(NULL, s);
											//Still not sure for the +1
	toFill->idToLinesName[lineId] = malloc((strlen(token)+1) * sizeof(char));
	strcpy(toFill->idToLinesName[lineId], token);

	// printf("Metro : %s, added.\n", token);

}


void fillGraph(Graph* toFill, SearchingTree* wordTree, char* filename){

	int nbVertice, nbEdge, nbSubway, tempo, count;

	FILE *fp;
	char str[MAXCHAR];

    fp = fopen(filename, "r");


	fscanf(fp, "%d %d %d\n", &nbVertice, &nbEdge, &nbSubway);


	toFill->nbVertice = nbVertice;
	toFill->vertices = malloc(nbVertice * sizeof(Vertice));

	// toFill->nbEdge = 0;
	// toFill->edges = malloc(nbEdge * sizeof(Edge));

	toFill->nbStrand = 0;
	toFill->strands = malloc((2 * nbEdge) * sizeof(Strand));

	toFill->nbLine = nbSubway;
	toFill->idToLinesName = malloc(nbSubway * sizeof(char*));


	if (fp == NULL){
		printf("Could not open file %s",filename);
		exit(1);
	}

	for(count = 0; count < nbVertice; ++count){
		
		fscanf(fp, "%d %s\n", &tempo, str);

		
		toFill->vertices[tempo].firstStrand = -1;
	
												//Not sure for the +1
		toFill->vertices[tempo].label = malloc((strlen(str)+1) * sizeof(char));
		strcpy(toFill->vertices[tempo].label, str);

		addWord(wordTree, str, tempo);

		//hashmap_put(AllStations, str, 0);

	}
	///DEBUG
	// for(count = 0; count < nbVertice; ++count){

	// 	printf("%d = %s\n", count, toFill->vertices[count].label);

	// }

	for(count = 0; count < nbSubway; ++count){

		if(!fgets(str, MAXCHAR, fp)){
			printf("Error, can't read line %d\n", count);
		}

		addSubway(str, toFill, count);

	}


	fclose(fp);

}


void freeGraph(Graph* toFree){

	int count;

	for(count = 0; count < toFree->nbVertice; ++count){
		free(toFree->vertices[count].label);
	}

	free(toFree->vertices);
	free(toFree->edges);
	free(toFree->strands);
}


void printGraph(Graph* graph){

	int i, j, index;

	for(i = 0; i < graph->nbVertice; ++i){

		printf("Node %d, %s. Strands :\n", i, graph->vertices[i].label);

		for(index = graph->vertices[i].firstStrand; index != -1; index = graph->strands[index].nextStrand){
			printf("index : %d, Vert : %d, next : %d, type : %d\n", index, graph->strands[index].vertice, graph->strands[index].nextStrand, graph->strands[index].type);
		}


	}

}

void printLinearGraph(Graph* graph){

	int i;

	for(i = 0; i < graph->nbVertice; ++i){

		printf("Node %d, first %d, %s.\n", i, graph->vertices[i].firstStrand, graph->vertices[i].label);

	}

	for(i = 0; i < graph->nbStrand; ++i){
		
		printf("index : %d, Vert : %d, next : %d, type : %d\n", i, graph->strands[i].vertice, graph->strands[i].nextStrand, graph->strands[i].type);

	}

}