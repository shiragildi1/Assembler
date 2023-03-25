run: assembler.o macro.o utils.o firstpass.o improve.o secondpass.o openentry.o openextern.o validate.o output.o
	gcc -g -ansi -pedantic -Wall assembler.o macro.o utils.o firstpass.o improve.o secondpass.o openentry.o openextern.o  validate.o output.o -o run

assembler.o: assembler.c macro.h utils.h firstpass.o
	gcc -c -ansi -pedantic -Wall -g assembler.c -o assembler.o

macro.o: macro.c macro.h utils.h
	gcc -c -ansi -pedantic -Wall -g macro.c -o macro.o

firstpass.o: firstpass.c firstpass.h utils.h 
	gcc -c -ansi -pedantic -Wall -g firstpass.c -o firstpass.o

improve.o: improve.c improve.h utils.h 
	gcc -c -ansi -pedantic -Wall -g improve.c -o improve.o

secondpass.o: secondpass.c secondpass.h utils.h 
	gcc -c -ansi -pedantic -Wall -g secondpass.c -o secondpass.o

openentry.o: openentry.c openentry.h utils.h 
	gcc -c -ansi -pedantic -Wall -g openentry.c -o openentry.o

openextern.o: openextern.c openextern.h utils.h
	gcc -c -ansi -pedantic -Wall -g openextern.c -o openextern.o

validate.o: validate.c validate.h utils.h
	gcc -c -ansi -pedantic -Wall -g validate.c -o validate.o

output.o: output.c output.h utils.h
	gcc -c -ansi -pedantic -Wall -g output.c -o output.o

utils.o: utils.c utils.h globals.h
	gcc -c -ansi -pedantic -Wall -g utils.c -o utils.o

