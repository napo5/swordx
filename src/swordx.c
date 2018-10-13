
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>


#include "trie.h"
#include "filereader.h"

void printHelp();
void _pushFile(TrieNode* root, FILE* file, char * fileName, int fileSize, char *delimiter, int minLenght, char ** words,int counter, TrieNode * ignoreRoot, char * logFile);
void pushFile(TrieNode* root, char * path, char *delimiter, int minLenght,TrieNode * ignoreRoot,char *logFile);
int isDirectory(const char *path);
void inputCheck(TrieNode* root,char *path,char*delimiter, int flagM,int flagR,int flagF,int flagS,char * exclude, char * ignoreList, char * logFile);
void pushIgnoreList(TrieNode * ignoreRoot, char * ignoreList);



int main(int argc, char **argv)
{

	int index;
	int c;

	char* delimiter = " ,.:;-\n'!’\t";

	opterr = 0;

	int flagR=0;
	int flagF=0;
	int flagA=0;
	int flagM=0;
	char * min = NULL;
	int flagS=0;
	int flagO=0;

	char* outpath = NULL;
	char* arg2 = "";

	char * exclude =" ";
	char * ignoreList = NULL;

	char * logFile = NULL;


	char *path = NULL;

	const char* const short_options="hrfo:e:am:i:l:s";

	const struct option long_options[] = {
		{"help",     no_argument,       0, 'h'},
        {"recursive",  no_argument,       0, 'r'},
        {"output",  required_argument, 0, 'o'},
        {"follow",  no_argument, 0, 'f'},
        {"exclude",    required_argument, 0, 'e'},
        {"alpha",    no_argument, 0, 'a'},
        {"min",    required_argument, 0, 'm'},
        {"ignore",    required_argument, 0, 'i'},
        {"sortbyoccurency",    no_argument, 0, 's'},
        {"log",      no_argument,    0, 'l'},
        {NULL,0,NULL,0}
        /*requied at array end*/
	};

	while((c = getopt_long(argc, argv ,short_options,long_options,NULL)) != -1){
		switch(c){

			case 'h':
				printHelp();
				exit(0);
				break;

			case 'r':
				flagR=1;
				break;

			case 'f':
				flagF=1;
				break;

			case 'o':
				flagO=1;
			 outpath = malloc(sizeof(char)*(strlen(optarg)+2));

          if (outpath != NULL){

			strcat(outpath,optarg);
			printf("%s",outpath);
			}

				break;

			case 'e':
			exclude = malloc(sizeof(char)*(strlen(optarg+2)));

			if (exclude != NULL){
			strcpy(exclude,optarg);}

				break;

			case 'a':
				delimiter = " \n'’\t1234567890!#$%&(*)+,-./:;<=>?@[]^_`\"{|}~\\";
				printf("[*] delimiter set to only alpha \n");
				 break;

			case 'm':
			if ((*optarg - '0') < 0) {
				printf("[WARN] invalid input error \n");}
			if ((*optarg -'0') >= 0) {

				min = (char*)malloc((strlen(optarg)+1)*sizeof(char));
				strcpy(min,optarg);
				flagM = (int)strtol(min,NULL,10);

					}
				break;

			case 'i':
				ignoreList = malloc(sizeof(char)*(strlen(optarg+2)));

				if (ignoreList != NULL){
					strcpy(ignoreList,optarg);
					}

				break;

			case 's':
				flagS=1;
				break;

			case 'l':
				logFile = malloc(sizeof(char)*(strlen(optarg+2)));

				if (logFile != NULL){
				strcpy(logFile,optarg);}


				break;

			default :

				abort();


			}

		}


		TrieNode* root = newNode();

  for (int index = optind; index < argc; index++){
		path = argv[index];
		char ** ignoreListVector = NULL;
    	inputCheck(root,path,delimiter,flagM,flagR,flagF,flagS,exclude,ignoreList,logFile);
    	printf("[*]... Analysing %s \n",path);
	}


		if(flagS==0){
			printAllWords(root,outpath);

	} 	if(flagS==1){
		printSBO(root,outpath);
	}





	return 0;
}

int isSymLink(char *  path){
	struct stat statbuf;
	int check = lstat(path,&statbuf);
	if(S_ISLNK(statbuf.st_mode)) return 1;
	else return 0;
}



int isDirectory(const char *path){
	struct stat statbuf;
	if(lstat(path, &statbuf) != 0)
		return 0;
	return S_ISDIR(statbuf.st_mode);
}


void pushFolder(TrieNode* root,char * path, char * delimiter, int minLenght,int flagR,int flagF,char * exclude,TrieNode * ignoreRoot, char * logFile){
	DIR *d;
  struct dirent *dir;
  d = opendir(path);
  if (d) {
    while ((dir = readdir(d)) != NULL) {


     char * fullpath = NULL;
     char entry[_POSIX_PATH_MAX] = {0};

     strcat (entry, path);
     strcat (entry, dir->d_name);


     if (isDirectory(entry)==1) {
		 printf("[*]Directories cannot be read \n");
	}

	if (isSymLink(entry)==1){
		printf("[*] %s is a symbolic link \n",entry);

		if(flagF==1){
			printf("[*] --follow option active, analizing file \n");
			pushFile(root,entry,delimiter,minLenght,ignoreRoot,logFile);
		} else { printf("[*] --follow option not selected ignoring file \n");}

	}
	  if (isDirectory(entry)==0 && isSymLink(entry)==0) {

		if(strcmp(exclude,entry)!=0){
		 pushFile(root,entry,delimiter,minLenght,ignoreRoot,logFile);
		} else {
			printf("[*] File %s excluded from the analysis (-e) \n",entry);
		}
	}


}
    closedir(d);
  }
}


void inputCheck(TrieNode* root,char *path,char*delimiter, int flagM,int flagR,int flagF,int flagS,char * exclude,char * ignoreList, char * logFile){

	TrieNode* ignoreRoot = newNode();
	if(flagF==1){
		printf("[*] Following Links \n");
	}

	if(ignoreList==NULL){
		printf("[*] Ignore option not activated \n");
	} else {

		pushIgnoreList(ignoreRoot,ignoreList);
		printf("[*] Ignore list pushed into blacklist trie \n");
	}

	if(flagM>0){
		printf("[*] Shortest word can be %i char long \n",flagM);
			}


	if(isDirectory(path)==0){

		pushFile(root,path,delimiter,flagM,ignoreRoot,logFile);

	}

	if(isDirectory(path)==1){



		pushFolder(root,path,delimiter,flagM,flagR,flagF,exclude,ignoreRoot,logFile);
	}
}



void printHelp(){
	printf("%s\n","Usage: swordx [OPTION]... [INPUT]...");
	printf("\n");
	printf("%s\n"," 'swordx' reads a file/folder,counts its occourencies and it saves it in a file.");
	printf("\n");
	printf(" [INPUT]		file or directory to be processed\n");
	printf(" [OPTION]  :\n");
	printf("\n");
	printf("  -h, --help                     print this help\n");
	printf("  --output [FILENAME]           write output in [FILE]\n");
	printf("                                (default output file is  /swordx.out)  \n");
	printf("  -r, --recursive                processes directories recursively     \n");
	printf("  -f, --follow                    follow links\n");
	printf("  -e [FILE], --explude [FILE]    [FILE] not processed                \n");
	printf("                                           (only works with -r)\n");
	printf("  -a, --alpha                    only process alphanumeric characters\n");
	printf("  -m[NUM], --min [NUM]           process words with more than [NUM] characters           \n");
	printf("  -i[FILE], --ignore[FILE]       [FILE] is a list of words to ignore      \n");
	printf("                                               (one word per line)      \n");
	printf("  -s, --sortbyoccurency           output file ordered by occurencies   \n");
	printf("  -l [FILE], --log [FILE]        create a log file with timing and stored/ignored words \n");
	printf("                                                                       \n");



}

void pushFile(TrieNode* root,char * path, char * delimiter, int minLenght,TrieNode *ignoreRoot,char *logFile){

	FILE*myfile = open_file(path);
	int length = fileLenght(myfile);
	int counter = 0; // serve ?????????
	char ** words = NULL;

	words =malloc( fileLenght(myfile)* sizeof(char));

	_pushFile(root,myfile,path,length,delimiter,minLenght,words,counter,ignoreRoot,logFile);

}

void _pushFile(TrieNode* root, FILE* file, char * path, int fileSize, char * delimiter, int minLenght, char ** words,int counter, TrieNode * ignoreRoot, char * logFile){

		int newcounter;
		int ignoredCounter = 0;

        char* token=NULL;


        for(token=strtok(stringSaver(file,fileSize),delimiter);
        token!=NULL;
        token=strtok(stringSaver(file,fileSize),delimiter)){


				if(strlen(token)>=minLenght){


						if(search(ignoreRoot,token)){
							printf("%s ignored \n",token);
							ignoredCounter++;
							}
							else {

									insert(root,token);
									newcounter++;
								}
						}

			}



			if(logFile!=NULL){

				FILE * outputFile =create_file(logFile);
				fprintf(outputFile,"%s pushed words [%i] , ignored words [%i] \n", path,newcounter,ignoredCounter);

				fclose(outputFile);

		}
}





void pushIgnoreList(TrieNode * ignoreRoot,char * ignoreList){

	FILE* ignoreFile = open_file(ignoreList);
	int length = fileLenght(ignoreFile);

	char* token=NULL;

	for(token=strtok(stringSaver(ignoreFile,length)," ");
		token!=NULL;
        token=strtok(stringSaver(ignoreFile,length)," ")){

			insert(ignoreRoot,token);

		}

	}
