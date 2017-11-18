#ifndef STACKS_H
#define STACKS_H

#include "global.h"

void  init_stacks();
void  push(CELL c);
void  pushp(PCELL c);
void rpush(PCELL c);
PCELL rpop();
CELL  pop();
PCELL popp();
void  drop();

void print_stack();

#endif
