all:
	clear
	gcc -g -Werror -pedantic -Wall -lm -pthread -o main main.c prudes.c heathens.c common.c

run: all
	./main

