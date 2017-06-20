#Makefile

EXEC = myshell
# Add source code
SRCS = myshell.c mycommand/pwd_command.c mycommand/help_command.c mycommand/whoami_command.c mycommand/cd_command.c

OBJECTS = $(SRCS:.c=.o)
CC = gcc
CFLAG = -Wall

myshell: ${OBJECTS}
	${CC} -o ${EXEC} ${OBJECTS} ${CFLAG}

%.o : %.c Makefile
	${CC} -c $< -o $@ ${CFLAG}

clean : 
	rm ${OBJECTS} ${EXEC}

