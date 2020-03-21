
#include "mcoGraph.h"


#include <limits.h>

#include "math.h"

#include "sortedQuintupletList.h"

//Cost of movement, in seconds * 10
#define CHANGE_LINE 9
#define TWO_STATION 9


unsigned int getFirstBitIndex(unsigned int value, unsigned int maxBit){

	unsigned int tempo, i;

	for(i = 0, tempo = value; i < maxBit; ++i, tempo >>= 1){

		if(tempo & 1){
			//printf("i : %d returned.\n", i);
			return i;
		}

	}
	

	printf("This print should never happen.\n");

	return 0;

}


void humanPrintNodePath(Matrix* graph, NodePath* path){

	unsigned int i;

	unsigned int lineId, oldLineId;

	char *save;

	lineId = path->nodes[1].line;

	//printf("Empruntez la ligne %s jusqu'à \n", graph->lineName[getFirstBitIndex(lineId, graph->nbLine)]);
	
	printf("E");
	for(i = 0; i < path->size; ++i){

		//printf("%d %d\n", path->nodes[i].node, path->nodes[i].line);

		oldLineId = lineId;
		lineId = lineId & path->nodes[i].line;

		if(lineId == 0){
			lineId = path->nodes[i].line;
			
			printf("mpruntez la ligne %s jusqu'à %s\n", graph->lineName[getFirstBitIndex(oldLineId, graph->nbLine)], graph->stationName[path->nodes[i-1].node]);
			
			printf("Puis, e");
			//printf("%s\n", graph->stationName[path->nodes[i].node]);

			//printf("Puis, empruntez la ligne %s jusqu'à \n", graph->lineName[getFirstBitIndex(lineId, graph->nbLine)]);

		}

	}
	
	printf("mpruntez la ligne %s jusqu'à %s\n", graph->lineName[getFirstBitIndex(oldLineId, graph->nbLine)], graph->stationName[path->nodes[i-1].node]);
	printf("Vous voilà arrivé à destination, n'oubliez pas vos bagages, de vous proteger du coronavirus et de noter ce projet avec une note comprise dans l'intervalle d'entier ]19 - 20].\n");
	//printf("%s\nDestination.\nVeuillez noter no services avec une note entre 18 et 20 sur 20\n", graph->stationName[path->nodes[i-1].node]);



}

NodePath* dijkstraMatrix(Matrix* graph, unsigned int start, unsigned int end){

	unsigned int actualNode, oldNode, edgeValue;
	unsigned int tempoCost, tempoLine;
	//Strand* actualStrand;

	NodePath* pathToReturn;

	unsigned int actualLineId, newLineId, oldLineId, i, j;
	int pairStrandIndex;
	unsigned int tempoValue;
	int actualStrandIndex, oldWinnerStrandIndex;

	int actualCost, newCost;

	//actualNode = &graph->vertices[start];
	actualNode = start;
	
	//We're going to save : cost, node, line
	SortedQuintupletList costAndStrand;
	QuintupletNode* tempoQuintuplet;

	createQuintList(&costAndStrand);


	/*
		This array represent each strand as it's index.
		if the value it store :
		= 0, the strand is not checked.
		> 0, the strand is checked and the value equal the score.
		< 0, the strand is done and it store the index ( -1 * (index + 1) ) of the strand that led to the strand represented by the index of the array.
	*/
	costAndLine* visitedStrand;
	
	visitedStrand = malloc(graph->width * sizeof(costAndLine));

	tempoValue = pow(2, graph->nbLine);
	for(i = 0; i < graph->width; ++i){
		visitedStrand[i].line = 0;
		
		visitedStrand[i].donePerLine = malloc(tempoValue * sizeof(int));;
		for(j = 0; j < tempoValue; ++j)
			visitedStrand[i].donePerLine[j] = 0;
		
		visitedStrand[i].costPerLine = malloc(tempoValue * sizeof(unsigned int));
		for(j = 0; j < tempoValue; ++j)
			visitedStrand[i].costPerLine[j] = UINT_MAX;
	}

	for(i = 0; i < tempoValue; ++i)
		visitedStrand[start].donePerLine[i] = 1;

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
	// actualStrandIndex = actualNode->firstStrand;

	//Note : the break is at the end, we could also put the test here but we skip some assignation
	while(1){
		
		/*
			Note : this variable is used 2 time differently,
			here to save the strand which led to this node.
			Below it's used to store the old strand which led to the current winner.
		*/
		// oldWinnerStrandIndex = actualStrandIndex;

		// actualStrandIndex = actualNode->firstStrand;

		///This test is for debugging, it should never hapen if the graph is connected and got more than 1 node
		// if(actualStrandIndex == -1){
		// 	printf("Node got no first strand.\n");
		// 	return NULL;
		// }


		//We check all the next possible brand
		for(i = 0; i < graph->width; ++i){
			
			edgeValue = graph->data[i + actualNode * graph->width];

			if(edgeValue == 0){
				continue;
			}

			if(visitedStrand[i].donePerLine[newLineId] == 1){
				//printf("%d done.\n", i);
				continue;
			}
			

			newCost = actualCost + TWO_STATION;

			newLineId = actualLineId & edgeValue;
			if(newLineId == 0){
				newCost += CHANGE_LINE;
				newLineId = edgeValue;
			}
			//printf("Doing %d for %d. Old line : %d, new Line : %d\n", i, actualNode, actualLineId, newLineId);
			
			tempoLine = visitedStrand[i].line;
			tempoCost = visitedStrand[i].costPerLine[newLineId];


			//The strand is not done
			if((tempoLine & newLineId) != newLineId){

				visitedStrand[i].line = tempoLine | newLineId;

			}
			else if(newCost >= tempoCost){
				continue;
			}

			visitedStrand[i].costPerLine[newLineId] = newCost;


			pushSortedQuintList(&costAndStrand, newCost, i, newLineId, actualNode, actualLineId);


		}


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

		//printf("select\n");
		// printf("done : %d, list :\n", actualNode);
		// printQuintList(&costAndStrand);
		
		do{

			//printf("list size : %d\n", costAndStrand.size);
			tempoQuintuplet = getFirstQuintList(&costAndStrand);
			
			actualCost		= tempoQuintuplet->first;
			
			actualNode		= tempoQuintuplet->second;
			actualLineId	= tempoQuintuplet->third;
			
			//Save Value to retrace the path
			oldNode			= tempoQuintuplet->fourth;
			oldLineId		= tempoQuintuplet->fifth;
			//printf("selected in list : %d\n", actualNode);

			deleteFirstQuintList(&costAndStrand);

		}
		while(visitedStrand[actualNode].donePerLine[actualLineId] != 0);

		// printf("selected\n");
		
		visitedStrand[actualNode].donePerLine[actualLineId] = 1 + oldNode;
		//Now the node is done, use costPerLine to store the line id which brought here
		visitedStrand[actualNode].costPerLine[actualLineId] = oldLineId;


		//visitedStrand[actualNode].line = oldNode;

		//actualStrand = &graph->strands[actualStrandIndex];
		

		/*
			Mark the selected strand and it's pair as done by setting it to a value < 0.
			We store the strand which led to this brand as a negative value of it's index
			(+1 for not setting 0 if it was the first brand of the array)
		*/
		// visitedStrand[actualStrandIndex] = -1*(oldWinnerStrandIndex+1);
		// visitedStrand[actualStrandIndex + actualStrand->type] = -1*(oldWinnerStrandIndex+1);
		
		if(actualNode == end){
			break;
		}
		
		// actualLineId = actualStrand->lineId;
		// actualNode = &graph->vertices[actualStrand->vertice];

	}

	deleteQuintList(&costAndStrand);


	oldNode 	= actualNode;
	oldLineId 	= actualLineId;


	i = 0;
	while(actualNode != start){
		
		tempoLine 		= visitedStrand[actualNode].costPerLine[actualLineId];
		actualNode 		= visitedStrand[actualNode].donePerLine[actualLineId]-1;
		
		actualLineId = tempoLine;

		++i;

	}

	pathToReturn = malloc(sizeof(NodePath));

	pathToReturn->size = i+1;
	pathToReturn->nodes = malloc(i * sizeof(NodeAndLine));

	//Store the last one here
	actualNode = oldNode;
	actualLineId = oldLineId;

	pathToReturn->nodes[i].node = actualNode;
	pathToReturn->nodes[i].line = actualLineId;

	--i;

	while(actualNode != start){

		tempoLine 		= visitedStrand[actualNode].costPerLine[actualLineId];
		actualNode 		= visitedStrand[actualNode].donePerLine[actualLineId]-1;
		
		actualLineId = tempoLine;


		pathToReturn->nodes[i].node = actualNode;
		pathToReturn->nodes[i].line = actualLineId;
		
		--i;

	}

	///Debug
	// for(i = 0; i < pathToReturn->size; ++i){
	// 	printf("Path : %d\n", graph->strands[pathToReturn->brandsIndex[i]].vertice);
	// }

	humanPrintNodePath(graph, pathToReturn);

	return pathToReturn;

}


void addSubwayMatrix(char* str, Matrix* toFill, unsigned int lineId){

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

		//y = from, x = to
		toFill->data[to + from * toFill->width] += (unsigned int)pow(2, lineId);
		toFill->data[from + to * toFill->width] += (unsigned int)pow(2, lineId);

		from = to;

	}

	token = strtok(NULL, s);
											//Still not sure for the +1
	toFill->lineName[lineId] = malloc((strlen(token)+1) * sizeof(char));
	strcpy(toFill->lineName[lineId], token);

	// printf("Metro : %s, added.\n", token);

}


void fillMatrixGraph(Matrix* toFill, SearchingTree* wordTree, char* filename){

	int nbVertice, nbEdge, nbSubway, tempo, count;

	FILE *fp;
	char str[MAXCHAR];

    fp = fopen(filename, "r");


	fscanf(fp, "%d %d %d\n", &nbVertice, &nbEdge, &nbSubway);


	toFill->size = nbVertice*nbVertice;
	toFill->width = nbVertice;

	toFill->data = malloc(toFill->size * sizeof(unsigned int));
	for(count = 0; count < toFill->size; ++count){

		toFill->data[count] = 0;

	}

	toFill->stationName = malloc(toFill->width * sizeof(char*));

	toFill->nbLine = nbSubway;
	toFill->lineName = malloc(nbSubway * sizeof(char*));

	if (fp == NULL){
		printf("Could not open file %s",filename);
		exit(1);
	}

	for(count = 0; count < nbVertice; ++count){
		
		fscanf(fp, "%d %s\n", &tempo, str);

			
												//Not sure for the +1
		toFill->stationName[tempo] = malloc((strlen(str)+1) * sizeof(char));
		strcpy(toFill->stationName[tempo], str);

		addWord(wordTree, str, tempo);


	}


	for(count = 0; count < nbSubway; ++count){

		if(!fgets(str, MAXCHAR, fp)){
			printf("Error, can't read line %d\n", count);
		}

		addSubwayMatrix(str, toFill, count);

	}


	fclose(fp);

}


void printMatrixGraph(Matrix* toPrint){

	int i, j;

	for(i = 0; i < toPrint->width; ++i){
		for(j = 0; j < toPrint->width; ++j){
		
			printf("%d ", toPrint->data[j + i * toPrint->width]);
		
		}
		printf("\n");
	}

}


void freeMatrixGraph(Matrix* toFree){

	unsigned int i;

	free(toFree->data);

	for(i = 0; i < toFree->width; ++i){
		free(toFree->stationName[i]);
	}
	free(toFree->stationName);
	
	for(i = 0; i < toFree->nbLine; ++i){
		free(toFree->lineName[i]);
	}
	free(toFree->lineName);

}
