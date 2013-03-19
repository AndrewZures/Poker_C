all:	
	gcc -ansi -pedantic -Wall testcardgame.c -I ../cardgame.c -I ../rank.c -I ../mcadvisor.c -I ../interface.c -I ../betting.c -o -a
clean:
	rm -f a
