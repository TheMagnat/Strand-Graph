#include "sortedQuintupletList.h"
#include <stdlib.h>
#include <stdio.h>

void createQuintList(SortedQuintupletList *l){

	l->size = 0;

	l->start = malloc(sizeof(QuintupletNode));
	l->end = malloc(sizeof(QuintupletNode));


	l->start->next = l->end;

}

void pushSortedQuintList(SortedQuintupletList *l, int firstValue, int secondValue, int thirdValue, int fourthValue, int fifthValue){

	QuintupletNode* newNode;
	QuintupletNode *it, *previous;
	
	newNode = malloc(sizeof(QuintupletNode));
	newNode->first = firstValue;
	newNode->second = secondValue;
	newNode->third = thirdValue;
	newNode->fourth = fourthValue;
	newNode->fifth = fifthValue;



	for(it = l->start->next, previous = l->start; it != l->end; previous = it, it = it->next){

		if(it->first >= firstValue){
			break;
		}

	}

	newNode->next = it;
	//newNode->next = previous->next; //Also work, it's the same
	previous->next = newNode;

	++l->size;

}

// void pushFront(SortedQuintupletList *l, int val){

// 	QuintupletNode* newNode;


// 	newNode = malloc(sizeof(QuintupletNode));
// 	newNode->value = val;


// 	newNode->next = l->start->next;
// 	l->start->next = newNode;

// 	++l->size;
// }

void deleteFirstQuintList(SortedQuintupletList* l){

	QuintupletNode* oldNode;

	oldNode = l->start->next;
	l->start->next = l->start->next->next;

	free(oldNode);

	--l->size;

}


QuintupletNode* getFirstQuintList(SortedQuintupletList* l){
	return l->start->next;
}



QuintupletNode* getNodeQuintList(SortedQuintupletList* l, unsigned int index){

	unsigned int count;
	QuintupletNode *it;

	for(it = l->start->next, count = 0; count < index; it = it->next, ++count);


	return it;


}

void clearQuintList(SortedQuintupletList* l){

	QuintupletNode *it, *tempo;

	for(it = l->start->next; it != l->end; ){

		tempo = it;
		it = it->next;

		free(tempo);

	}

	l->start->next = l->end;

	l->size = 0;

}

void deleteQuintList(SortedQuintupletList* l){

	clearQuintList(l);

	free(l->start);
	free(l->end);

}

void printQuintList(SortedQuintupletList* l){

	QuintupletNode* it;

	for(it = l->start->next; it != l->end; it = it->next){

		printf("Value : %d and %d and %d and %d and %d\n", it->first, it->second, it->third, it->fourth, it->fifth);

	}

}






