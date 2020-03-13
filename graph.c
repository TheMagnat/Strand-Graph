
#include "graph.h"

#include <string.h>

void addSubway(char* str, Graph* toFill){

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
		
		//Same comment as above
		toFill->vertices[to].firstStrand = toFill->nbStrand;
		
		// printf("%d Strand : vert = %d, first = %d, type = %d\n", toFill->nbStrand, toFill->strands[toFill->nbStrand].vertice, toFill->strands[toFill->nbStrand].nextStrand, toFill->strands[toFill->nbStrand].type);

		++toFill->nbStrand;


		from = to;

	}

	token = strtok(NULL, s);

	printf("Metro : %s, added.\n", token);

}


void fillGraph(Graph* toFill, char* filename){

	int nbVertice, nbEdge, nbSubway, tempo, count;

	FILE *fp;
	char str[MAXCHAR];

    fp = fopen(filename, "r");


	fscanf(fp, "%d %d %d\n", &nbVertice, &nbEdge, &nbSubway);


	toFill->nbVertice = nbVertice;
	toFill->vertices = malloc(nbVertice * sizeof(Vertice));

	toFill->nbEdge = 0;
	toFill->edges = malloc(nbEdge * sizeof(Edge));

	toFill->nbStrand = 0;
	toFill->strands = malloc((2 * nbEdge) * sizeof(Strand));


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

	}
	for(count = 0; count < nbVertice; ++count){

		printf("%d = %s\n", count, toFill->vertices[count].label);

	}

	for(count = 0; count < nbSubway; ++count){

		if(!fgets(str, MAXCHAR, fp)){
			printf("Error, can't read line %d\n", count);
		}

		addSubway(str, toFill);

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