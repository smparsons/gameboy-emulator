#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "src/mmu.h"

int main(int argc, char *argv[])
{
  if (argc != 2) {
    printf("Expected 1 argument, but got %d.\n", argc - 1);
    exit(EXIT_FAILURE);
  }

  Memory memory = initializeMemory();
  loadGame(&memory, argv[1]);

  printf("Printing out first 0x8000 bytes of rom...");

  for (int i = 0; i < 0x8000; i++) {
    printf("%u ", memory.rom[i]);
  }

  return 0;
}