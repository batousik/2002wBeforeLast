all : bin/sp1_p2

clean : 
	rm obj/* bin/*

reset :
	clear
	make clean
	make all

bin/sp1 : obj/sp1.o
	gcc obj/sp1.o -o bin/sp1   -g -std=gnu99

obj/sp1.o : src/sp1.c src/sp1.h
	gcc -c src/sp1.c -o obj/sp1.o   -g -std=gnu99

bin/sp1_p2 : obj/sp1_p2.o
	gcc obj/sp1_p2.o -o bin/sp1_p2   -g -std=gnu99

obj/sp1_p2.o : src/sp1_p2.c src/sp1_p2.h
	gcc -c src/sp1_p2.c -o obj/sp1_p2.o   -g -std=gnu99