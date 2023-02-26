CC 			= gcc
CFLAGS 		= -g -Wall -I ./include -l pthread 
VPATH 		= ./:./src:./include
PROC_NAME   = SSI
BIN_PTH     = ./bin


SERVER_RELY = \
			ssi.o \

Server : ${SERVER_RELY}
	@${CC} ${CFLAGS} ${SERVER_RELY} -o ${BIN_PTH}/${PROC_NAME}
ssi.o : ssi.c ssi.h 
	@${CC} -c ${CFLAGS} $<


.PHONY: clean

clean : 
	@rm -rf ${BIN_PTH}/${PROC_NAME} ${SERVER_RELY}



