#ifndef SEARCHING_TREE_H
#define SEARCHING_TREE_H


struct TreeNode;
typedef struct TreeNode TreeNode;

struct SearchingTree;
typedef struct SearchingTree SearchingTree;

struct TreeNode {

	char c;
	int end;

	SearchingTree* nextTree;

};

struct SearchingTree {

	int* charToIndex;
	int charToIndexReserved;

	TreeNode** nextNodes;
	int nextNodesSize;
	int nextNodesReserved;

};

void initTree(SearchingTree* treeToFill);
void initNode(TreeNode* toFill);

void reserveMoreNodes(SearchingTree* treeToFill);

void reserveCharIndexTo(SearchingTree* treeToFill, int toWhere);


void addWord(SearchingTree* treeToFill, char* word);


void printTree(SearchingTree* toPrint);

#endif