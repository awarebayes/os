main.o: main.c
	gcc -c main.c

main.out: main.o
	gcc main.o -o main.out

run: main.out
	./main.out

clear:
	rm -rf ./*.o ./*.out