#include <stdio.h>
#include <stdlib.h>
#include "src/mmu.h"

int main(int argc, char *argv[])
{
  if (argc != 2) {
    printf("Expected 1 argument, but got %d.\n", argc - 1);
    exit(EXIT_FAILURE);
  }

  Memory memory = initializeMemory();
  loadGame(&memory, argv[1]);

  printf("Printing out rom bytes...\n");

  for (int i = 0; i < 0x8000; i++) {
    printf("%u ", readByte(&memory, i));
  }

  printf("Writing byte to address 0x2AB2");
  writeByte(&memory, 0x2AB2, 0x3C);
  printf("Byte stored at address 0x2AB2 is %d", readByte(&memory, 0x2AB2));

  printf("Writing word to address 0x13BD");
  writeWord(&memory, 0x13BD, 0x5BDC);
  printf("Word stored at address 0x13BD is %d", readWord(&memory, 0x13BD));

  return 0;
}