#ifndef SORTED_PAIR_LIST_H
#define SORTED_PAIR_LIST_H

/**
 * The node of the sorted pair list.
*/
struct TupleNode{

	int first;
	int second;
	int third;

	struct TupleNode* next;

};
typedef struct TupleNode TupleNode;

/**
 * A list that contain pair and is sorted (smallest first in first).
*/
struct SortedTupleList{

	unsigned int size;

	TupleNode* start;
	TupleNode* end;

};
typedef struct SortedTupleList SortedTupleList;

void createList(SortedTupleList *l);

void pushSortedList(SortedTupleList* l, int firstValue, int secondValue, int thirdValue);

TupleNode* getFirstList(SortedTupleList* l);

void deleteFirstList(SortedTupleList* l);

TupleNode* getNodeList(SortedTupleList* l, unsigned int index);

void clearList(SortedTupleList* l);

void printList(SortedTupleList* l);

void deleteList(SortedTupleList* l);

#endif /* SORTED_PAIR_LIST_H */
