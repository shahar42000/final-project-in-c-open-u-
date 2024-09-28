assembler: assembler.o error.o macro.o check.o
	gcc -g -ansi -Wall -pedantic assembler.o error.o macro.o check.o -o assembler
	rm -f *.o

error.o: error.c 
	gcc -g -ansi -Wall -pedantic -c error.c

macro.o: macro.c 
	gcc -g -ansi -Wall -pedantic -c macro.c

check.o: check.c 
	gcc -g -ansi -Wall -pedantic -c check.c

assembler.o: assembler.c
	gcc -g -ansi -Wall -pedantic -c assembler.c

clean:
	rm -f assembler *.o