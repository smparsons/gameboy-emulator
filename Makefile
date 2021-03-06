compile :
	gcc main.c -o main.o -c
	gcc src/mmu.c -o mmu.o -c
	gcc src/cpu.c -o cpu.o -c 
	gcc src/emulator.c -o emulator.o -c
	gcc src/microops.c -o microops.o -c
	gcc src/gpu.c -o gpu.o -c
	gcc src/alu.c -o alu.o -c
	gcc -o gbemu main.o mmu.o cpu.o emulator.o microops.o gpu.o alu.o