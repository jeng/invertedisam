CC	= gcc -g
LIB 	= -lm
BIN 	= stop-test
SRC     = stop-test.c 
OBJ	= eprintf.o stop.o document.o posting.o fileutils.o words.o

stop-test:	stop-test.o $(OBJ)
	$(CC) -o $@ $@.o $(OBJ)	$(LIB)

document-manager-test: document-manager-test.o $(OBJ)
	$(CC) -o $@ $@.o $(OBJ)	$(LIB)

posting-test: posting-test.o $(OBJ)
	$(CC) -o $@ $@.o $(OBJ) $(LIB)

words-test: words-test.o $(OBJ)
	$(CC) -o $@ $@.o $(OBJ) $(LIB)

all: stop-test document-manager-test posting-test words-test	

clean: 
	rm stop-test; rm document-manager-test; 
	rm *.o; rm \#*\#; rm *~
