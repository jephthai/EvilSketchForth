#include "cstring.h"

// Library for counted strings

int cstrcmp(PBYTE ab, CELL al, PBYTE bb, CELL bl) {
  if(al != bl) return 0;
  return memcmp(ab, bb, al) == 0;
}

void cprint(PBYTE s, CELL len) {
  for(; len; len--, s++) {
    putchar(*s);
  }
}
