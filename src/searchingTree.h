#ifndef SEARCHING_TREE_H
#define SEARCHING_TREE_H


struct TreeNode;
typedef struct TreeNode TreeNode;

struct SearchingTree;
typedef struct SearchingTree SearchingTree;

/**
 * This structure is a node of the SearchingTree.
 * It contain the character, an int that tell
 * if this character is an end of word
 * and a pointer that is is not NULL,
 * contain the next part of the tree.
 * It can also store a value.
*/
struct TreeNode {

	unsigned int value;

	char c;
	int end;

	SearchingTree* nextTree;

};

/**
 * This structe is used to store words as a tree.
 * It allow fast search of present words and fast
 * completion of words from any character string
*/
struct SearchingTree {

	int* charToIndex;
	int charToIndexReserved;

	TreeNode** nextNodes;
	int nextNodesSize;
	int nextNodesReserved;

};

/**
 * Initialize the tree
*/
void initTree(SearchingTree* treeToFill);
/**
 * Initialize a node
*/
void initNode(TreeNode* toFill);

void deleteTree(SearchingTree* toDelete);

/**
 * Reserve more nodes if the allocated memory of nextNodes if full
*/
void reserveMoreNodes(SearchingTree* treeToFill);

/**
 * Reserve again the charToIndex memory to the toWhere size.
*/
void reserveCharIndexTo(SearchingTree* treeToFill, int toWhere);

/**
 * This function add a word to the given tree.
 * 
 * @param treeToFill The tree that we want to fill with the word.
 * @param word The word we want to add to the tree.
 * @param value The value to store at the word end node.
*/
void addWord(SearchingTree* treeToFill, char* word, unsigned int value);

/**
 * For debug purpose.
 * Print the tree in stdin.
*/
void printTree(SearchingTree* toPrint);

/**
 * Fin a word in the tree.
 * Note : if showAutoComplete is not set at 0,
 * 		  it will print the possibles words in stdin.
 * 
 * @param showAutoComplete If not set to 0, also show the auto complete possible of the given word.
 * 
 * @return value of the end node of the word if it's present, -1 otherwise.
*/
int findWord(SearchingTree* tree, char* wordToFind, int showAutoComplete);

/**
 * Only auto complete function.
 * Note : it will print the possibles words in stdin.
*/
void autoCompleteWord(SearchingTree* tree, char* wordToComplete);

/**
 * Print all the words of the tree in stdin.
*/
void recursivePrintWord(SearchingTree* tree, char buffer[1024], int buffSize);

#endif
