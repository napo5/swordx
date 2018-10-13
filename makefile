
SRC = src/{swordx.c,trie.c,filereader.c,linkedlist.c}
default : swordx.o swordx

swordx : swordx trie.o  filereader.o linkedlist.o
	gcc -o swordx swordx.o trie.o filereader.o linkedlist.o

swordx.o :
	gcc -c  $(SRC)

clean :
	rm swordx swordx.o trie.o filereader.o linkedlist.o
