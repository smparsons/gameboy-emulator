#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
  if (argc != 2) {
    printf("Expected 1 argument, but got %d.\n", argc - 1);
    exit(EXIT_FAILURE);
  }

  const int numberOfBytes = 0x8000;
  unsigned char buffer[numberOfBytes] = {0};

  FILE *ptr;
  char *romPath = argv[1];
  ptr = fopen(romPath, "rb");

  if (ptr == NULL) {
    fprintf(stderr, "Error opening file: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  fread(buffer, sizeof(buffer), 1, ptr);

  for (int i = 0; i < numberOfBytes; i++) {
    printf("%u ", buffer[i]);
  }

  fclose(ptr);

  return 0;
}