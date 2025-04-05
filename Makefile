CC := /usr/bin/gcc

all: clean | task1 task2

task1:
	$(CC) -o server.o src/server.c
task2:
	$(CC) -o client.o src/client.c

clean: 
	rm -rf *.o
