compile :
	gcc main.c -o main.o -c
	gcc src/mmu.c -o mmu.o -c
	gcc -o gbemu main.o mmu.o