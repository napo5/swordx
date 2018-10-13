#include <stdlib.h>
#include <ctype.h>
#include "filereader.h"

FILE* open_file(char* path){

	FILE* pf = fopen(path, "rb");

	return pf;
}

FILE* create_file(char* path){
	FILE *pf = fopen(path, "ab+");
	return pf;
}



char *stringSaver(FILE *file, int fileSize){

	char *string = malloc(sizeof(char)* fileSize);
		int i = 0;
		int readChar = fgetc(file);






	if ((int)readChar == -1) { return NULL;}


	while ((int)readChar != -1 && (int)readChar >= 0 && (int)readChar <= 255){


		string[i] = (char)tolower((char)readChar);

		readChar = fgetc(file);
		i++;


	}
	
	return string;
}


int fileLenght(FILE* fp){

	fseek(fp,0,SEEK_END);

	int length = ftell(fp);

	rewind(fp);

	return length;
}
