CC	= gcc -g
LIB 	=
BIN 	= stop-test
SRC     = stop-test.c 
OBJ	= eprintf.o stop.o

stop-test:	stop-test.o $(OBJ)
	$(CC) -o $@ $@.o $(OBJ)	$(LIB)

all: stop-test

clean: 
	rm stop-test; rm *.o; rm \#*\#; rm *~
