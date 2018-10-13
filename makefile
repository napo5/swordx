

CC = gcc

OBJD = obj

INC = include

CFLAGS = -Wall -I$(INC)

OBJ = $(OBJD)/swordx.o \
	$(OBJD)/trie.o \
	$(OBJD)/linkedlist.o \
	$(OBJD)/filereader.o \
	
vpath %.c src
vpath %.h $(INC)

all: swordx

swordx : $(OBJ)
	$(CC) $^ $(CFLAGS) -o $@
 
# objects are into OBJD and include .c and .h
$(OBJD)/%.o: %.c %.h
	$(CC) $(CFLAGS) -o $@ -c $<


$(OBJD)/trie.o: trie.c trie.h
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJD)/linkedlist.o: linkedlist.c linkedlist.h
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJD)/filereader.o: filereader.c filereader.h
	$(CC) $(CFLAGS) -o $@ -c $<


$(OBJD)/swordx.o: swordx.c trie.h filereader.h
	$(CC) $(CFLAGS) -o $@ -c $<

clean :
	rm swordx obj/*.o
