#Makefile

CC = gcc

all: myshell

myshell: myshell.c
	gcc -o myshell myshell.c
