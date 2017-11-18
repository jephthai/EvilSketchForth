#include "global.h"

CELL  psp;
CELL  rsp;
CELL  work;
PCELL pip;
PBYTE here;
entry *last;
entry *current;
CELL  tibtop;
CELL  tibstart;
CELL  temp[16];
CELL  mode;
FILE *input;

uint8_t tib[128];

PCELL bifs[1024];
