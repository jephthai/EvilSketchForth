#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "global.h"
#include "cstring.h"
#include "utility.h"
#include "interpreter.h"

void init_dictionary();
entry *lookup(PBYTE name, CELL len);
PBYTE after_entry(entry *e);
entry *create_entry(PBYTE name, CELL len);
void comma(CELL c);
void move_here(CELL count);
void print_dictionary();
void def_bif(PBYTE name, int len, VOIDFUN fn);
void print_entry(entry *e);

#endif
