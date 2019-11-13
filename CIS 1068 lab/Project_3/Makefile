CC=gcc
CFLAGS=-g -Wall -pthread -std=c99
RM=rm

all: clean server

server: main.c queue.c utils.c logqueue.c spellchecker.h
		$(CC) $(CFLAGS) -o server main.c queue.c utils.c logqueue.c 

clean:
		$(RM) -rf serv *.dSYM
