#include "sortedTupleList.h"
#include <stdlib.h>
#include <stdio.h>

void createList(SortedTupleList *l){

	l->size = 0;

	l->start = malloc(sizeof(TupleNode));
	l->end = malloc(sizeof(TupleNode));


	l->start->next = l->end;

}

void pushSortedList(SortedTupleList *l, int firstValue, int secondValue, int thirdValue){

	TupleNode* newNode;
	TupleNode *it, *previous;
	
	newNode = malloc(sizeof(TupleNode));
	newNode->first = firstValue;
	newNode->second = secondValue;
	newNode->third = thirdValue;



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

// void pushFront(SortedTupleList *l, int val){

// 	TupleNode* newNode;


// 	newNode = malloc(sizeof(TupleNode));
// 	newNode->value = val;


// 	newNode->next = l->start->next;
// 	l->start->next = newNode;

// 	++l->size;
// }

void deleteFirstList(SortedTupleList* l){

	TupleNode* oldNode;

	oldNode = l->start->next;
	l->start->next = l->start->next->next;

	free(oldNode);

	--l->size;

}


TupleNode* getFirstList(SortedTupleList* l){
	return l->start->next;
}



TupleNode* getNodeList(SortedTupleList* l, unsigned int index){

	unsigned int count;
	TupleNode *it;

	for(it = l->start->next, count = 0; count < index; it = it->next, ++count);


	return it;


}

void clearList(SortedTupleList* l){

	TupleNode *it, *tempo;

	for(it = l->start->next; it != l->end; ){

		tempo = it;
		it = it->next;

		free(tempo);

	}

	l->start->next = l->end;

	l->size = 0;

}

void deleteList(SortedTupleList* l){

	clearList(l);

	free(l->start);
	free(l->end);

}

void printList(SortedTupleList* l){

	TupleNode* it;

	for(it = l->start->next; it != l->end; it = it->next){

		printf("Value : %d and %d and %d\n", it->first, it->second, it->third);

	}

}






