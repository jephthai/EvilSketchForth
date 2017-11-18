#include "utility.h"

//
// A basic "hexdump" utility.  This is useful for debugging,
// especially when dictionary entries or the compiler are messing up.
// It is also provided in the 'dump' forth word, wrapped up in
// f_dump() in core_words.c
//

void dump(PBYTE b, CELL count) {
  int i;
  char ascii[17];

  printf("\n");
  memset(ascii, 0, 17);

  for(i = 0; i < count; i++, b++) {
    if(i % 8 == 0) putchar(' ');
    if(i % 16 == 0) {
      printf("%s\n%p  ", ascii, b);
      memset(ascii, 0, 17);
    }
    printf("%02x ", *b);
    ascii[i % 16] = isprint(*b) ? *b : '.';
  }

  if(i % 8 == 0) putchar(' ');
  while(i++ % 16) {
    if(!(i % 8)) putchar(' ');
    printf("   ");
  }

  printf("%s\n\n", ascii);
}
