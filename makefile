#	03/11/2014
#
#	Programacion Imperativa
#	Trabajo Practico Especial
#
#	Grupo 7
#	Debrouvier, Ariel
#	Matorras Brastche, Tobias
#	Perazzo, Matias


COMPILER=gcc
OUTPUT_FILE=nInLine.out
FILES=nInLineFront.c nInLineBack.c nInLineBack.h rand.a getnum2.c getnum.h
PARAMETERS=-o

all:
#	make getnum
	make rand
	$(COMPILER) $(PARAMETERS) $(OUTPUT_FILE) $(FILES)

getnum:
	$(COMPILER) -c getnum2.c
	ar r getnum2.a getnum2.o
	ranlib getnum2.a

rand:	
	$(COMPILER) -c rand.c
	ar r rand.a rand.o
	ranlib rand.a

clean:
	rm $(OUTPUT_FILE)	
