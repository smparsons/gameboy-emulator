#include <stdio.h>
#include <stdlib.h>
#include "src/emulator.h"

int main(int argc, char *argv[])
{
  if (argc != 2) {
    printf("Expected 1 argument, but got %d.\n", argc - 1);
    exit(EXIT_FAILURE);
  }

  playGame(argv[1]);

  return 0;
}