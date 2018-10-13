

#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

LinkedNode * insertList(LinkedNode * list, char * word, int counter){


	LinkedNode * newNode;
	newNode = malloc(sizeof(LinkedNode));
	newNode->word = word;
	newNode->counter = counter;
	newNode-> next = NULL;


	if (list->word==NULL) {

		return newNode;}

		int listCounter = list->counter;
		char * listWord = list -> word;



	if(counter<listCounter){

		if(list->next!=NULL){
		LinkedNode * nextNode = list->next;
		int nextCounter = nextNode->counter;
 		if(counter > nextCounter){
			newNode->next = nextNode;
			list->next = newNode;
			return list;
		}  else {
			insertList(nextNode,word,counter);
		}

	}if(list->next==NULL && counter == 1){
		LinkedNode * nextNode = list->next;
			newNode->next = nextNode;
			list->next = newNode;

			return list;
		}
		
	return list;
	}

	if(counter>=listCounter){

		newNode->next = list;
		return newNode;
	}

	return list;




}
