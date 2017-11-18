#ifndef GLOBAL_H
#define GLOBAL_H

// counted strings vs NULL terminated strings... this makes
// it more convenient to put string literals in function calls
// that expected counted strings.

#define cslit(s)         (PBYTE)s, (CELL)strlen(s)

#define DICTIONARY_SIZE  (1024 * 1024 * 8)
#define PSTACK_SIZE      1024
#define RSTACK_SIZE      1024
#define NAMELEN          16
#define COMPILE         -1
#define INTERPRET        0

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

typedef  uint64_t   CELL;
typedef  uint64_t*  PCELL;
typedef  uint8_t    BYTE;
typedef  uint8_t*   PBYTE;
typedef  PBYTE      CSTRING;

typedef  void(*VOIDFUN)(void);

typedef  CELL(*RETBIF0)(void);
typedef  CELL(*RETBIF1)(CELL);
typedef  CELL(*RETBIF2)(CELL, CELL);
typedef  CELL(*RETBIF3)(CELL, CELL, CELL);
typedef  CELL(*RETBIF4)(CELL, CELL, CELL, CELL);
typedef  CELL(*RETBIF5)(CELL, CELL, CELL, CELL, CELL);
typedef  CELL(*RETBIF6)(CELL, CELL, CELL, CELL, CELL, CELL);

typedef  void(*VOIDBIF0)(void);
typedef  void(*VOIDBIF1)(CELL);
typedef  void(*VOIDBIF2)(CELL, CELL);
typedef  void(*VOIDBIF3)(CELL, CELL, CELL);
typedef  void(*VOIDBIF4)(CELL, CELL, CELL, CELL);
typedef  void(*VOIDBIF5)(CELL, CELL, CELL, CELL, CELL);
typedef  void(*VOIDBIF6)(CELL, CELL, CELL, CELL, CELL, CELL);

struct entry_t {
  struct entry_t *next;
  BYTE name[NAMELEN];
  BYTE len;
  BYTE f1;
  BYTE f2;
  BYTE f3;
  VOIDFUN code;
  CELL data;
};

typedef struct entry_t entry;

extern CELL    psp;
extern CELL    rsp;
extern CELL    work;
extern PCELL   pip;
extern PBYTE   here;
extern entry  *last;
extern entry  *current;
extern CELL    tibtop;
extern CELL    tibstart;
extern uint8_t tib[128];
extern CELL    temp[16];
extern CELL    mode;
extern FILE   *input;
extern PCELL   bifs[1024];
#endif
