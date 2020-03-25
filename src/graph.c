
#include "graph.h"

#include "sortedTupleList.h"

#include <string.h>

//Cost of movement, in seconds * 10
#define CHANGE_LINE 9
#define TWO_STATION 9


void humanPrintBrandPath(Graph* graph, BrandPath* path){
	
	unsigned int i;

	unsigned int lineId;

	lineId = graph->strands[path->brandsIndex[0]].lineId;

	printf("Empruntez la ligne %s jusqu'à ", graph->idToLinesName[lineId]);
	
	for(i = 0; i < path->size; ++i){

		if(lineId != graph->strands[path->brandsIndex[i]].lineId){
			lineId = graph->strands[path->brandsIndex[i]].lineId;
			
			printf("%s\n", graph->vertices[graph->strands[path->brandsIndex[i-1]].vertice].label);

			printf("Puis, empruntez la ligne %s jusqu'à ", graph->idToLinesName[lineId]);

		}

	}

	printf("%s\nDestination.\nVeuillez noter no services avec une note entre 18 et 20 sur 20\n", graph->vertices[graph->strands[path->brandsIndex[i-1]].vertice].label);

}


BrandPath* dijkstra(Graph* graph, unsigned int start, unsigned int end, uint8_t timeOrChange){

	Vertice* actualNode;
	Strand* actualStrand;

	BrandPath* pathToReturn;

	unsigned int actualLineId, i;
	int pairStrandIndex;
	int tempoValue;
	int actualStrandIndex, oldWinnerStrandIndex;

	int actualCost, newCost;

	actualNode = &graph->vertices[start];

	
	SortedTupleList costAndStrand;
	TupleNode* tempoTuple;

	createList(&costAndStrand);


	/*
		This array represent each strand as it's index.
		if the value it store :
		= 0, the strand is not checked.
		> 0, the strand is checked and the value equal the score.
		< 0, the strand is done and it store the index ( -1 * (index + 1) ) of the strand that led to the strand represented by the index of the array.
	*/
	int* visitedStrand;
													//int
	visitedStrand = malloc(graph->nbStrand * sizeof(*visitedStrand));
	for(i = 0; i < graph->nbStrand; ++i){
		visitedStrand[i] = 0;
	}

	pathToReturn = NULL;

	actualCost = 0;
	actualLineId = graph->nbLine; //So it's never true the first time

	/*
		Note : the index of the first strand is initialized
		to any strand that is connected to the first node.
		This is only usefull to save the path easily saving the old
		strand each time (the first node is the only node that don't have any
		old strand since he is the first).
	*/
	actualStrandIndex = actualNode->firstStrand;

	//Note : the break is at the end, we could also put the test here but we skip some assignation
	while(1){
		
		/*
			Note : this variable is used 2 time differently,
			here to save the strand which led to this node.
			Below it's used to store the old strand which led to the current winner.
		*/
		oldWinnerStrandIndex = actualStrandIndex;

		actualStrandIndex = actualNode->firstStrand;

		///This test is for debugging, it should never hapen if the graph is connected and got more than 1 node
		if(actualStrandIndex == -1){
			printf("Node got no first strand.\n");
			return NULL;
		}


		//We check all the next possible brand
		do{
			
			actualStrand = &graph->strands[actualStrandIndex];
			
			//It's pair strand.
			pairStrandIndex = actualStrandIndex + actualStrand->type;
			
			
			tempoValue = visitedStrand[pairStrandIndex];

			//The strand is not done
			if(tempoValue >= 0){

				if(timeOrChange){

					if(actualStrand->lineId != actualLineId) newCost += 1;

				}
				else{
					
					newCost = actualCost + TWO_STATION;
					if(actualStrand->lineId != actualLineId) newCost += CHANGE_LINE;

				}
				

				//If the checked strand got a higher value or if it's not checked (0).
				if(tempoValue == 0){

					pushSortedList(&costAndStrand, newCost, pairStrandIndex, oldWinnerStrandIndex);

				}
				else if(tempoValue > newCost){
					
					pushSortedList(&costAndStrand, newCost, pairStrandIndex, oldWinnerStrandIndex);

					//Erase the older version in the list ?
					//Not sure if it's usefull.

				/*
					Store this possibility as a pair of :
						int (the cost to here) and int (index of the strand of arrival = (actualStrandIndex + actualStrand->type))
				
					in a sorted list, smallest first.
				*/
				}
				//We could add a tempoValue == newCost and only keep the lowest 'line change'
			 	else{
					//Here the new cost is higher or equal to the old, so it's useless.
				}


			}



			
			actualStrandIndex = actualStrand->nextStrand;

		}
		while(actualStrandIndex != -1);



		///This test is for debugging, it should never hapen if the graph is connected
		if(costAndStrand.size == 0){
			printf("List size = 0.\n");
			return NULL;
		}

		//Now select the first possibility in the sorted list
		/*
			It's possible a brand that wasn't visited when added to the list
			has become visited (finding another faster path that lead to a node attached to the strand)
			So to be sure it do not happen,
			we loop on the choice of the next strand and ignore every strand that are marked as checked. (< 0)
		*/
		do{

			tempoTuple = getFirstList(&costAndStrand);

			actualStrandIndex 		= tempoTuple->second;
			actualCost 				= tempoTuple->first;
			oldWinnerStrandIndex 	= tempoTuple->third;

			deleteFirstList(&costAndStrand);

		}
		while(visitedStrand[actualStrandIndex] < 0);

		
		actualStrand = &graph->strands[actualStrandIndex];
		

		/*
			Mark the selected strand and it's pair as done by setting it to a value < 0.
			We store the strand which led to this brand as a negative value of it's index
			(+1 for not setting 0 if it was the first brand of the array)
		*/
		visitedStrand[actualStrandIndex] = -1*(oldWinnerStrandIndex+1);
		visitedStrand[actualStrandIndex + actualStrand->type] = -1*(oldWinnerStrandIndex+1);
		
		if(actualStrand->vertice == end){
			break;
		}
		
		actualLineId = actualStrand->lineId;
		actualNode = &graph->vertices[actualStrand->vertice];

	}

	deleteList(&costAndStrand);

	oldWinnerStrandIndex = actualStrandIndex;

	i = 0;
	while(graph->strands[actualStrandIndex].vertice != start){

		actualStrandIndex = (-1 * visitedStrand[actualStrandIndex]) - 1;

		++i;

	}
								//BrandPath
	pathToReturn = malloc(sizeof(*pathToReturn));

	pathToReturn->size = i+1;
													  //int
	pathToReturn->brandsIndex = malloc((i+1) * sizeof(*pathToReturn->brandsIndex));

	//Store the last one here
	actualStrandIndex = oldWinnerStrandIndex;
	pathToReturn->brandsIndex[i] = actualStrandIndex;
	--i;
	while(graph->strands[actualStrandIndex].vertice != start){

		actualStrandIndex = (-1 * visitedStrand[actualStrandIndex]) - 1;

		pathToReturn->brandsIndex[i] = actualStrandIndex;
		
		--i;

	}

	free(visitedStrand);

	///Debug
	// for(i = 0; i < pathToReturn->size; ++i){
	// 	printf("Path : %d\n", graph->strands[pathToReturn->brandsIndex[i]].vertice);
	// }

	return pathToReturn;

}

void addSubway(char* str, Graph* toFill){

	int lineId, nbNode, from, to, i;

	const char s[3] = " \n";
	char *token;


	token = strtok(str, s);
	lineId = atoi(token);

	token = strtok(NULL, s);
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

	if(toFill->idToLinesName[lineId]){

		printf("this line id is already done. %d\n", lineId);
		return;

	}

	token = strtok(NULL, s);
											//Still not sure for the +1
	toFill->idToLinesName[lineId] = malloc((strlen(token)+1) * sizeof(char));
	strcpy(toFill->idToLinesName[lineId], token);

	// printf("Metro : %s, added.\n", token);

}


void fillGraph(Graph* toFill, char* filename, SearchingTree* wordTree, uint8_t fillWord){

	int nbVertice, nbEdge, nbSubway, nbLineOnSubway, tempo, count;

	FILE *fp;
	char str[MAXCHAR];

    fp = fopen(filename, "r");


	fscanf(fp, "%d %d %d %d\n", &nbVertice, &nbEdge, &nbSubway, &nbLineOnSubway);


	toFill->nbVertice = nbVertice;				//Vertice
	toFill->vertices = malloc(nbVertice * sizeof(*toFill->vertices));

	toFill->nbStrand = 0;						  //Strand
	toFill->strands = malloc((2 * nbEdge) * sizeof(*toFill->strands));

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

		if(fillWord)
			addWord(wordTree, str, tempo);


	}


	for(count = 0; count < nbLineOnSubway; ++count){

		if(!fgets(str, MAXCHAR, fp)){
			printf("Error, can't read line %d\n", count);
		}

		addSubway(str, toFill);

	}


	fclose(fp);

}


void freeGraph(Graph* toFree){

	unsigned int count;

	for(count = 0; count < toFree->nbVertice; ++count){
		free(toFree->vertices[count].label);
	}

	free(toFree->vertices);

	free(toFree->strands);

	for(count = 0; count < toFree->nbLine; ++count){
		free(toFree->idToLinesName[count]);
	}

	free(toFree->idToLinesName);

}


void printGraph(Graph* graph){

	unsigned int i;
	int index;

	for(i = 0; i < graph->nbVertice; ++i){

		printf("Node %d, %s. first %d Strands :\n", i, graph->vertices[i].label, graph->vertices[i].firstStrand);

		for(index = graph->vertices[i].firstStrand; index != -1; index = graph->strands[index].nextStrand){
			printf("index : %d, Vert : %d, next : %d, type : %d\n", index, graph->strands[index].vertice, graph->strands[index].nextStrand, graph->strands[index].type);
		}


	}

}

void printLinearGraph(Graph* graph){

	unsigned int i;

	for(i = 0; i < graph->nbVertice; ++i){

		printf("Node %d, first %d, %s.\n", i, graph->vertices[i].firstStrand, graph->vertices[i].label);

	}

	for(i = 0; i < graph->nbStrand; ++i){
		
		printf("index : %d, Vert : %d, next : %d, type : %d\n", i, graph->strands[i].vertice, graph->strands[i].nextStrand, graph->strands[i].type);

	}

}
