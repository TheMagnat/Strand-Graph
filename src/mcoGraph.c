
#include "mcoGraph.h"


#include <limits.h>

#include <string.h>

#include "math.h"

#include "sortedQuintupletList.h"

//Cost of movement, in seconds * 10
#define CHANGE_LINE 9
#define TWO_STATION 9


/**
 * Get the first bit set to 1.
 * 
 * @param value The value to check
 * @param maxBit the maximum of bit to test
 * 
 * @return the index of the first bit set at 1 of the value.
*/
unsigned int getFirstBitIndex(unsigned int value, unsigned int maxBit){

	unsigned int tempo, i;

	for(i = 0, tempo = value; i < maxBit; ++i, tempo >>= 1){
		if(tempo & 1)
			return i;
	}

	//Debug
	printf("This print should never happen.\n");

	return 0;

}



void humanPrintNodePath(Matrix* graph, NodePath* path){

	unsigned int lineId, oldLineId, i;

	lineId = path->nodes[1].line;

	printf("E");
	//Starting at i = 2 may not work if the path is only size 1.
	for(i = 1; i < path->size; ++i){

		oldLineId = lineId;
		lineId = lineId & path->nodes[i].line;

		if(lineId == 0){
			lineId = path->nodes[i].line;
			printf("mpruntez la ligne %s jusqu'à %s\n", graph->lineName[getFirstBitIndex(oldLineId, graph->nbLine)], graph->stationName[path->nodes[i-1].node]);
			printf("Puis, e");
		}

	}
	
	printf("mpruntez la ligne %s jusqu'à %s\n", graph->lineName[getFirstBitIndex(oldLineId, graph->nbLine)], graph->stationName[path->nodes[i-1].node]);
	printf("Vous voilà arrivé à destination, n'oubliez pas vos bagages, de vous proteger du coronavirus et de noter ce projet avec une note comprise dans l'intervalle d'entier ]19 - 20].\n");

}


NodePath* dijkstraMatrix(Matrix* graph, unsigned int start, unsigned int end){

	unsigned int actualNode, oldNode, edgeValue;
	unsigned int tempoLine;
	//Strand* actualStrand;

	NodePath* pathToReturn;

	unsigned int actualLineId, newLineId, oldLineId, i, j;
	unsigned int tempoValue;

	unsigned int actualCost, newCost, tempoCost;

	actualNode = start;
	
	//We're going to save : cost, node, line
	SortedQuintupletList costAndStrand;
	QuintupletNode* tempoQuintuplet;

	createQuintList(&costAndStrand);

	/*
		This will store the current state of each node compared to the line id.
	*/
	CostAndDone* saved;
										//CostAndDone
	saved = malloc(graph->width * sizeof(*saved));

	/*
		We initialize all the CostAndDone structure.
		TempoValue is the maximum value.
	*/
	tempoValue = pow(2, graph->nbLine);
	for(i = 0; i < graph->width; ++i){
		//saved[i].line = 0;
		
		saved[i].donePerLine = malloc(tempoValue * sizeof(int));;
		for(j = 0; j < tempoValue; ++j)
			saved[i].donePerLine[j] = 0;
		
		saved[i].costPerLine = malloc(tempoValue * sizeof(unsigned int));
		for(j = 0; j < tempoValue; ++j)
			saved[i].costPerLine[j] = UINT_MAX;
	
	}

	//Fill the starting node to set it done.
	for(i = 0; i < tempoValue; ++i)
		saved[start].donePerLine[i] = 1;

	//Not sur if it's usefull.
	pathToReturn = NULL;

	//The cost start at 0
	actualCost = 0;
	//start the line id with an id that do not exist.
	actualLineId = pow(2, graph->nbLine); 



	//Note : the break is at the end, we could also put the test here but we skip some assignation
	while(1){


		//We check all the next possible brand
		for(i = 0; i < graph->width; ++i){

			edgeValue = graph->data[i + actualNode * graph->width];

			//If the value is 0 it mean there is no edge between actualNode and i.
			if(edgeValue == 0){
				continue;
			}

			/*
				Get the new line id
				Not : If the actualLineId was multiple lines
				(By being a value with multiple bit on like 3 5 6 7...)
				this calcul will return the possible lines between the new node and the old lines we was on.
			*/
			newLineId = actualLineId & edgeValue;

			/*
				Actualize the new cost to the node 'i'
				If the line changed, add even more to the cost.
			*/
			newCost = actualCost + TWO_STATION;

			//If newLineId = 0 it mean the & operation returned 0 so there is no line in common between the old lines and the new node.
			if(newLineId == 0){
				newCost += CHANGE_LINE;
				newLineId = edgeValue;
			}
			
			/*
				If the line is done, continue.
				Note : We could us costPerValue and store in it a negative when it's done
				but when a node is done, we use donePerLine to store the node which led to the node
				and we use costPerLine to store the line that led to the node too.
				So both vector are important.
			*/
			if(saved[i].donePerLine[newLineId] == 1){
				continue;
			}
			
			tempoCost = saved[i].costPerLine[newLineId];

			/*
				If tempoCost is less the UINT_MAX, it mean it has already been explored,
				and if the test it true, the last exploration had a better score so we wontinue.
			*/
			if(newCost >= tempoCost){
				continue;
			}


			//If we arrive here, the actual path is the best to arrive to this node (with the current line).
			saved[i].costPerLine[newLineId] = newCost;

			//We need to save all these value
			pushSortedQuintList(&costAndStrand, newCost, i, newLineId, actualNode, actualLineId);

		}


		///This test is for debugging, it should never hapen if the graph is connected
		if(costAndStrand.size == 0){
			return NULL;
		}

		//Now select the first possibility in the sorted list
		/*
			It's possible a node that wasn't done when added to the list
			has become done (finding another faster path that lead to a node)
			So to be sure it do not happen,
			we loop on the choice of the next node and ignore every node that are done.
		*/		
		do{

			tempoQuintuplet = getFirstQuintList(&costAndStrand);
			
			actualCost		= tempoQuintuplet->first;
			
			actualNode		= tempoQuintuplet->second;
			actualLineId	= tempoQuintuplet->third;
			
			//Save Value to retrace the path
			oldNode			= tempoQuintuplet->fourth;
			oldLineId		= tempoQuintuplet->fifth;

			deleteFirstQuintList(&costAndStrand);

		}
		while(saved[actualNode].donePerLine[actualLineId] != 0);

		//Now the node is done, give it a value that is not equal to 0 and that can help us to get to the node before it.
		saved[actualNode].donePerLine[actualLineId] = 1 + oldNode;
		//Now the node is done, use costPerLine to store the line id which brought here
		saved[actualNode].costPerLine[actualLineId] = oldLineId;
		
		//End if the node is equal to the end node.		
		if(actualNode == end){
			break;
		}
		
	}

	//Free the Quint list.
	deleteQuintList(&costAndStrand);


	oldNode 	= actualNode;
	oldLineId 	= actualLineId;

	/*
		Count the number of node in the path.
	*/
	i = 0;
	while(actualNode != start){
		
		tempoLine 		= saved[actualNode].costPerLine[actualLineId];
		actualNode 		= saved[actualNode].donePerLine[actualLineId]-1;
		
		actualLineId = tempoLine;

		++i;

	}

	
								//NodePath
	pathToReturn = malloc(sizeof(*pathToReturn));

	pathToReturn->size = i+1;
											//NodeAndLine
	pathToReturn->nodes = malloc((i+1) * sizeof(*pathToReturn->nodes));


	//Store the last one here
	actualNode = oldNode;
	actualLineId = oldLineId;

	pathToReturn->nodes[i].node = actualNode;
	pathToReturn->nodes[i].line = actualLineId;

	--i;

	/*
		Then store then in the pathToReturn vector.
	*/
	while(actualNode != start){

		tempoLine 		= saved[actualNode].costPerLine[actualLineId];
		actualNode 		= saved[actualNode].donePerLine[actualLineId]-1;
		
		actualLineId = tempoLine;


		pathToReturn->nodes[i].node = actualNode;
		pathToReturn->nodes[i].line = actualLineId;
		
		--i;

	}


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

}


void fillMatrixGraph(Matrix* toFill, char* filename, SearchingTree* wordTree, uint8_t fillWord){

	int nbVertice, nbEdge, nbSubway, tempo, count;

	FILE *fp;
	char str[MAXCHAR];

    fp = fopen(filename, "r");


	fscanf(fp, "%d %d %d\n", &nbVertice, &nbEdge, &nbSubway);


	toFill->size = nbVertice*nbVertice;
	toFill->width = nbVertice;
												//unsigned int
	toFill->data = malloc(toFill->size * sizeof(*toFill->data));
	for(count = 0; count < nbVertice*nbVertice; ++count){

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

		if(fillWord)
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

	unsigned int i, j;

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
