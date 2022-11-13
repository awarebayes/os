
%.out: %.o
	gcc $< -o $@

%.o: %.c
	gcc -c $<

3.out: 3_1.out 3_2.out

run: 5.out
	./5.out

all: 1.out 2.out 3.out 4.out 5.out

clear:
	rm -rf ./*.o ./*.out
