
#include "searchingTree.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


//the size with which the arrays will be initialized the first time
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

void recursiveDelete(SearchingTree* toDelete, int depth){
	
	int i;
	for(i = 0; i < toDelete->nextNodesSize; ++i){

		if(toDelete->nextNodes[i]->nextTree){
			recursiveDelete(toDelete->nextNodes[i]->nextTree, depth+1);
		}

		free(toDelete->nextNodes[i]);

	}

	free(toDelete->charToIndex);
	free(toDelete->nextNodes);

	/*
		Si on ne fais pas ce test, le programme crash.
		Pourquoi ? j'ai une théorie mais je ne suis vraiment pas sur.
	*/
	if(depth != 0)
		free(toDelete);

}

void deleteTree(SearchingTree* toDelete){

	recursiveDelete(toDelete, 0);

}

void reserveMoreNodes(SearchingTree* treeToFill){

	treeToFill->nextNodesReserved *= 2;

	treeToFill->nextNodes = realloc(treeToFill->nextNodes, treeToFill->nextNodesReserved * sizeof(TreeNode*) );

}

void reserveCharIndexTo(SearchingTree* treeToFill, int toWhere){

	int old;

	old = treeToFill->charToIndexReserved;

	treeToFill->charToIndexReserved = toWhere;

	treeToFill->charToIndex = realloc(treeToFill->charToIndex, treeToFill->charToIndexReserved * sizeof(int) );

	for(; old < treeToFill->charToIndexReserved; ++old){

		treeToFill->charToIndex[old] = -1;

	}

}

void addWord(SearchingTree* treeToFill, char* word, unsigned int value){

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

		if((int)tempoChar >= treeToFill->charToIndexReserved){
			/* 	+ 1 because we want to access to tempoChar as an index.
				(and index start at 0 so we need to reserve 1 above)	*/
			reserveCharIndexTo(treeToFill, (int)tempoChar + 1);
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
	currentNode->value = value;

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

	// for(i=0; i < toPrint->charToIndexReserved; ++i){

	// 	printf("%d (%c) value: %d\n",i, (char)i, toPrint->charToIndex[i]);

	// }

}

int findWord(SearchingTree* tree, char* wordToFind, int showAutoComplete){

	size_t stringSize, i;
	int nodeIndex;
	char tempoChar;
	char buffer[1024];

	TreeNode* currentNode;
	TreeNode trickNode;

	/*
		I use trick node as a trick to don't have to do
		the first iteration of the for loop differently or
		to add a test (i == stringSize-2) at the end of the loop. 
	*/
	trickNode.nextTree = tree;

	currentNode = &trickNode;


	stringSize = strlen(wordToFind);


	for(i = 0; i < stringSize; ++i){

		//If the current node don't have any tree attached to it, create one
		if(currentNode->nextTree == NULL){
			return -1;
		}

		tree = currentNode->nextTree;


		tempoChar = wordToFind[i];

		//If tempoChar exced the reserbed size, the node does not exist
		if((int)tempoChar >= tree->charToIndexReserved){
			return -1;
		}

		//Node do not exist
		if(tree->charToIndex[(int)tempoChar] == -1){
			return -1;
		}
		//Node exist
		else{
			nodeIndex = tree->charToIndex[(int)tempoChar];
		}

		//Now use the node index.
		currentNode = tree->nextNodes[nodeIndex];
		buffer[i] = currentNode->c;

	}

	if(!currentNode->end){
		if(showAutoComplete){
			recursivePrintWord(currentNode->nextTree, buffer, i);
		}
		return -1;
	}


	return (int)currentNode->value;

}

void autoCompleteWord(SearchingTree* tree, char* wordToComplete){
	
	size_t stringSize, i;
	int nodeIndex;
	char tempoChar;
	char buffer[1024];

	TreeNode* currentNode;
	TreeNode trickNode;

	/*
		I use trick node as a trick to don't have to do
		the first iteration of the for loop differently or
		to add a test (i == stringSize-2) at the end of the loop. 
	*/
	trickNode.nextTree = tree;

	currentNode = &trickNode;


	stringSize = strlen(wordToComplete);


	for(i = 0; i < stringSize; ++i){

		//If the current node don't have any tree attached to it, create one
		if(currentNode->nextTree == NULL){
			return;
		}

		tree = currentNode->nextTree;


		tempoChar = wordToComplete[i];

		//If tempoChar exced the reserbed size, the node does not exist
		if((int)tempoChar >= tree->charToIndexReserved){
			return;
		}

		//Node do not exist
		if(tree->charToIndex[(int)tempoChar] == -1){
			return;
		}
		//Node exist
		else{
			nodeIndex = tree->charToIndex[(int)tempoChar];
		}

		//Now use the node index.
		currentNode = tree->nextNodes[nodeIndex];
		buffer[i] = currentNode->c;

	}


	recursivePrintWord(currentNode->nextTree, buffer, i);

}

void recursivePrintWord(SearchingTree* tree, char buffer[1024], int buffSize){
	
	int i;

	/*
		We could add a verification to check if buffSize get bigger than 1024,
		but it should never happen, if we found one day that we could use word
		that exced 1024 characters, we could use a dynamic buffer instead.
	*/

	for(i = 0; i < tree->nextNodesSize; ++i){

		buffer[buffSize] = tree->nextNodes[i]->c;

		if(tree->nextNodes[i]->end){
			buffer[buffSize+1] = '\0';
			printf("%s\n", buffer);
		}

		if(tree->nextNodes[i]->nextTree){
			recursivePrintWord(tree->nextNodes[i]->nextTree, buffer, buffSize+1);
		}

	}

}
