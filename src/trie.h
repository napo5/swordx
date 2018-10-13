#include <stdbool.h>


typedef struct TrieNode {

	struct TrieNode *children[26];
	char letter;
	bool wordEnd;
	int counter;
} TrieNode;

void insert(TrieNode* root, char* word);
TrieNode *newNode(void);
void printLetters(TrieNode* t);
void printAllWords(TrieNode* root,char * outpath);
bool search(TrieNode *root, char* word);
void wordsToTrie(TrieNode *root, char** word, int counter);
void printSBO(TrieNode * root, char * outpath);
