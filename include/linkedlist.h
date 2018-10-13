 #include <stdio.h>

typedef struct LinkedNode {

	char * word;
	int counter;
	struct LinkedNode * next;
} LinkedNode;

LinkedNode * insertList(LinkedNode * list, char * word, int counter);
