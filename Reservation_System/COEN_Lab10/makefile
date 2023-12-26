coen11_labs_jake: list.o files.o main.o makefile
	gcc -lpthread list.o files.o main.o -o coen11_labs_jake


list.o: list.c global_defs.h makefile
	gcc -c -Wall -Wextra -pedantic list.c

files.o: files.c global_defs.h makefile
	gcc -c -Wall -Wextra -pedantic files.c

main.o: main.c global_defs.h makefile
	gcc -c -Wall -Wextra -pedantic main.c

clean:
	rm coen11_labs_jake *.o
