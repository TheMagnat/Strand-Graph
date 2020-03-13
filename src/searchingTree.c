
#include "searchingTree.h"


#include <string.h>



#define INIT_SIZE 10


void initTree(SearchingTree* treeToFill){

	int i;

	treeToFill->nextNodesReserved = INIT_SIZE;
	treeToFill->nextNodesSize = 0;

	treeToFill->nextNodes = malloc(INIT_SIZE * sizeof(TreeNode*));

	
	treeToFill->charToIndexReserved = INIT_SIZE;

	treeToFill->charToIndex = malloc(INIT_SIZE * sizeof(int));

	for(i = 0; i < INIT_SIZE; ++i){

		treeToFill->charToIndex[i] = -1;

	}

}

void initNode(TreeNode* toFill){
	
	toFill->end = 0;
	toFill->nextTree = NULL;

}

void deleteTree(SearchingTree* toDelete){

	//delete stuff

	hashmap_free(toDelete->charToIndex);
}

void reserveMoreNodes(SearchingTree* treeToFill){

	treeToFill->nextNodesReserved *= 2;

	treeToFill->nextNodes = realloc(treeToFill->nextNodes, treeToFill->nextNodesReserved * sizeof(TreeNode*) );

}

void reserveCharIndexTo(SearchingTree* treeToFill, int toWhere){

	int old;

	old = treeToFill->charToIndexReserved;

	treeToFill->charToIndexReserved = toWhere+1;

	treeToFill->charToIndex = realloc(treeToFill->charToIndex, treeToFill->charToIndexReserved * sizeof(int) );

	for(; old < treeToFill->charToIndexReserved; ++old){

		treeToFill->charToIndex[old] = -1;

	}

}

void addWord(SearchingTree* treeToFill, char* word){

	size_t stringSize, i;
	int nodeIndex;
	char tempoChar;

	TreeNode* currentNode;
	TreeNode trickNode;

	/*
		I use trick node as a trick to don't have to do
		the first iteration of the for loop differently or
		to add a test (i == stringSize-2) at the end of the loop. 
	*/
	trickNode.nextTree = treeToFill;

	currentNode = &trickNode;


	stringSize = strlen(word);

	for(i = 0; i < stringSize; ++i){

		//If the current node don't have any tree attached to it, create one
		if(currentNode->nextTree == NULL){
			currentNode->nextTree = malloc(sizeof(SearchingTree));
			initTree(currentNode->nextTree);
		}

		treeToFill = currentNode->nextTree;


		tempoChar = word[i];

		if((int)tempoChar > treeToFill->charToIndexReserved){
			reserveCharIndexTo(treeToFill, (int)tempoChar);
		}

		//Node do not exist
		if(treeToFill->charToIndex[(int)tempoChar] == -1){

			//Reserve more if needed
			if(treeToFill->nextNodesSize == treeToFill->nextNodesReserved){
				reserveMoreNodes(treeToFill);
			}

			//Create the new node
			nodeIndex = treeToFill->nextNodesSize;

			treeToFill->nextNodes[nodeIndex] = malloc(sizeof(TreeNode));

			initNode(treeToFill->nextNodes[nodeIndex]);

			treeToFill->nextNodes[nodeIndex]->c = tempoChar;

			++treeToFill->nextNodesSize;

			//save the new node index
			treeToFill->charToIndex[(int)tempoChar] = nodeIndex;

		}
		//Node exist
		else{
			nodeIndex = treeToFill->charToIndex[(int)tempoChar];
		}

		//Now use the node index.
		currentNode = treeToFill->nextNodes[nodeIndex];


	}

	currentNode->end = 1;

}

void printTree(SearchingTree* toPrint){

	int i;

	for(i = 0; i < toPrint->nextNodesSize; ++i){

		printf("Node %c :\n", toPrint->nextNodes[i]->c);
		if(toPrint->nextNodes[i]->end){
			printf("word end.\n");
		}

		if(toPrint->nextNodes[i]->nextTree){
			printTree(toPrint->nextNodes[i]->nextTree);
		}

		printf("end of Node %c\n", toPrint->nextNodes[i]->c);

	}

}