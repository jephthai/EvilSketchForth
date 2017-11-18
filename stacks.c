#include "stacks.h"

PCELL pstack;
PCELL rstack;

void init_stacks() {
  pstack = (PCELL) malloc(PSTACK_SIZE * sizeof(CELL));
  rstack = (PCELL) malloc(RSTACK_SIZE * sizeof(CELL));

  memset(pstack, 0, PSTACK_SIZE * sizeof(CELL));
  memset(rstack, 0, RSTACK_SIZE * sizeof(CELL));
  psp = 0;
  rsp = 0;
}

void push(CELL c) {
  pstack[psp++] = c;
}

void pushp(PCELL c) {
  pstack[psp++] = (CELL)c;
}

void rpush(PCELL c) {
  rstack[rsp++] = (CELL)c;
}

void underflow() {
  printf("\x1b[31;1mERROR: stack underflow!\x1b[0m\n");
  pip = 0;
  psp = 0;
}

PCELL rpop() {
  if(rsp) {
    return (PCELL)rstack[--rsp];
  } else {
    underflow();
    return NULL;
  }
}

CELL pop() {
  if(psp) {
    return pstack[--psp];
  } else {
    underflow();
    return 0;
  }
}

PCELL popp() {
  if(psp) {
    return (PCELL)pstack[--psp];
  } else {
    underflow();
    return NULL;
  }
}

void drop() {
  psp--;
}

void print_stack() {
  printf("[%lu] ", psp);
  for(int i = 0; i < psp; i++) {
    // printf("%lld ", pstack[i]);
    printf("%p ", (void*)pstack[i]);
  }
  printf("\n");
}
