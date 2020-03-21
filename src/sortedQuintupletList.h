#ifndef SORTED_QUINTUPLET_LIST_H
#define SORTED_QUINTUPLET_LIST_H

/**
 * The node of the sorted pair list.
*/
struct QuintupletNode{

	int first;
	int second;
	int third;
	int fourth;
	int fifth;

	struct QuintupletNode* next;

};
typedef struct QuintupletNode QuintupletNode;

/**
 * A list that contain pair and is sorted (smallest first in first).
*/
struct SortedQuintupletList{

	unsigned int size;

	QuintupletNode* start;
	QuintupletNode* end;

};
typedef struct SortedQuintupletList SortedQuintupletList;

void createQuintList(SortedQuintupletList *l);

void pushSortedQuintList(SortedQuintupletList* l, int firstValue, int secondValue, int thirdValue, int fourthValue, int fifthValue);

QuintupletNode* getFirstQuintList(SortedQuintupletList* l);

void deleteFirstQuintList(SortedQuintupletList* l);

QuintupletNode* getNodeQuintList(SortedQuintupletList* l, unsigned int index);

void clearQuintList(SortedQuintupletList* l);

void printQuintList(SortedQuintupletList* l);

void deleteQuintList(SortedQuintupletList* l);

#endif /* SORTED_QUINTUPLET_LIST_H */
