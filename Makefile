all : compilar

compilar : definicoes.o lexico.o
	gcc -g sintatico.c -g lexico.c -o compilar -lm -Wall

lexico.o : lexico.c lexico.h
	gcc lexico.c -c -lm -Wall

definicoes.o : definicoes.h
	gcc definicoes.h -c -lm -Wall

.PHONY:clean

clean:
	rm -f *.o