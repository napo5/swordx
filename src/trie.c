#include "trie.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "filereader.h"
#include "linkedlist.h"

#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])

// Alphabet size (# of symbols)
#define ALPHABET_SIZE (26)

// Converts key current character into index
// use only 'a' through 'z' and lower case
#define CHAR_TO_INDEX(c) ((int)c - (int)'a')






TrieNode *newNode(void){

	TrieNode *myNode = NULL;
	myNode = (TrieNode*) malloc(sizeof(TrieNode));

	if(myNode){

		int i;

		myNode ->wordEnd = false;

		for (i = 0; i < ALPHABET_SIZE; i++)
            myNode->children[i] = NULL;
            myNode->counter = 0;
      }
      return myNode;
}

void insert(TrieNode* root, char* word){

	TrieNode *pCrawl = root;



	int length = strlen(word);
	int level;
	int index;



	for (level = 0; level < length; level++){



			index = CHAR_TO_INDEX(word[level]);


			if (!pCrawl->children[index]){
            pCrawl->children[index] = newNode();
            pCrawl->children[index]->letter = word[level];
		}

        pCrawl = pCrawl->children[index];
	}

	pCrawl-> wordEnd = true;
	pCrawl-> counter++;



}




bool search ( TrieNode *root, char* word ){
	int level;
    int length = strlen(word);
    int index;
    TrieNode *pCrawl = root;

    for (level = 0; level < length; level++)
    {
        index = CHAR_TO_INDEX(word[level]);

        if (!pCrawl->children[index])
            return false;

        pCrawl = pCrawl->children[index];
    }

    return (pCrawl != NULL && pCrawl->wordEnd);
}


void printWord(char* wordArray, int n, int counter,char * outpath){

	char * word = malloc(n+1 * sizeof(char));
		strncpy(word,wordArray,n);
		word[n] = '\0';


	if(outpath==NULL){

		printf("[*]%s - occurs %i times \n",word,counter);
	}

	else{
		FILE * outputFile =create_file(outpath);
		fprintf(outputFile,"%s - occurs %i times \n",word,counter);
		fclose(outputFile);
	}
}



void _printAllWords(TrieNode* root, char * wordArray, int pos, char * outpath) {



	if (root == NULL) {

		printf("Trie is empty.");
		return;
	}

	if(root->wordEnd){

		printWord(wordArray,pos,root->counter,outpath);
	 }

	for( int i= 0; i<ALPHABET_SIZE; i++){

		if(root->children[i] != NULL){

			wordArray[pos] = root->children[i]->letter;
			_printAllWords(root->children[i], wordArray, pos+1,outpath);
		}
	}
}

void printAllWords(TrieNode * root,char * outpath){
	char wordArray[100];
	_printAllWords(root,wordArray,0,outpath);
}


LinkedNode * pushSBO(TrieNode * root,char *wordArray,int pos,LinkedNode * list) {




	if (root == NULL) {

		printf("Trie is empty.");
		return list;
	}

	if(root->wordEnd){
		char * word = malloc(pos+1 * sizeof(char));
		strncpy(word,wordArray,pos);
		word[pos] = '\0';

		list = insertList(list,word,root->counter);

	}
		for( int i= 0; i<ALPHABET_SIZE; i++){

		if(root->children[i] != NULL){

			wordArray[pos] = root->children[i]->letter;
			list = pushSBO(root->children[i], wordArray, pos+1,list);
		}
	}

	return list;
}

void printAllList(LinkedNode * list, char * outpath){


	FILE * outputFile =create_file(outpath);


	if(list!=NULL){

		if (outpath!=NULL) {

			fprintf(outputFile,"[*] %s - occurs %i times \n",list->word,list->counter);
			if(list->next!=NULL){
					printAllList(list->next,outpath);
					}
				fclose(outputFile);


		} else {

				printf("[*] %s occurs %i times \n",list->word,list->counter);
				if(list->next!=NULL){
					printAllList(list->next,outpath);
					}
				}
	}

}


void printSBO(TrieNode * root, char * outpath) {

	LinkedNode * list = NULL;
	list = malloc(sizeof(LinkedNode));

	char array[100];
	list = pushSBO(root,array,0,list);



	printAllList(list,outpath);
	if(outpath!=NULL) {printf("Output file is ready. \n");}

}
