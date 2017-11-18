#include "dictionary.h"

//
// The dictionary is the core data structure that makes Forth work.
// It is basically a mapping of names to pointers, but with a little
// metadata that supports the Forth execution model.  A dictionary
// entry can be several things, but normally they are executable
// words, variables, buffers, etc.  It can be considered similar to a
// heap if you come from C, but with some structure that is made
// available to the programmer.
//
// The 'entry' structure is defined in 'global.h', and is the header
// for a dictionary entry.  Each dictionary entry looks something like
// this:
//
//         +--------------------------------------------+
//         | LINK | NAME... | LEN | FLAGS | CODE | DATA |
//         +--------------------------------------------+
//
// Each of these fields is as follows:
//
//  LINK      A pointer to the next entry in the dictionary.  As
//            such the dictionary is a linked list, of sorts.
//
//  NAME...   A fixed length field denoting the name of the entry.
//            Think of this as a function or variable "name".  The
//            entry can be found with the 'find' word when passed
//            this value.
//
//  LEN       A byte indicating the length of the name in bytes.
//
//  FLAGS     Metadata flags that can be used to denote whether the
//            word is executed at COMPILE time vs INTERPRET time, etc.
//
//  CODE      A function pointer, should point to a function that
//            carries out the action associated with the entry.  E.g.,
//            if the entry is a built-in function, this CODE pointer
//            will be the "do_bif()" inner interpreter function that
//            handles execution of a back-end C function.
//
//  DATA      Some entries benefit from one extra field of metadata.
//            E.g., if the entry is a compiled function (a so-called
//            "colon definition") this points to the location where
//            the compiled sequence of execution tokens begins.  For
//            a variable, this points to the variable's location.
//
// Dictionary entries can be found by looking them up by name, and can
// be enumerated with the 'words' function in the forth environment.
//
// There are three global pointers used to keep track of the dictionary:
//
// CURRENT    The most recent entry created in the dictionary.
//
// LAST       The last entry "revealed" (actually added to the list).
//            This is important in case a new entry needs to refer to
//            a previous entry by the same name.  One case where this
//            subtle distinction between CURRENT and LAST is important
//            is when compiling recursive functions -- they must be
//            revealed (with the 'reveal' function) before the compiler
//            can find the self-named entry in the dictionary!
//
// HERE       Points to the next available byte in the dictionary.
//            Since the dictionary may be used to store things like
//            buffers, this may be advanced arbitrarily after the end
//            of a defined dictionary entry.
//
// Think of the dictionary's memory space as organized something like this:
//
//                                            LAST         CURRENT
//  +-------------------------------------------------------------+
//  | ENTRY | ENTRY | CODE | ENTRY | BUFFER | ENTRY | ... | ENTRY |
//  +-------------------------------------------------------------+
//     ^____/  ^___________/ ^______________/  ^_____ ... _/       HERE
//
//

PBYTE dictionary;

void init_dictionary() {
  dictionary = (PBYTE) malloc(DICTIONARY_SIZE);
  last = NULL;
  current = NULL;
  here = dictionary;
}

entry *lookup(PBYTE name, CELL len) {
  entry *this = last;
  while(this) {
    if(cstrcmp(this->name, this->len, name, len)) {
      break;
    }
    this = this->next;
  }

  return this;
}

PBYTE after_entry(entry *e) {
  return (PBYTE)(e + 1);
}

entry *create_entry(PBYTE name, CELL len) {
  if(len > NAMELEN) {
    printf("ERROR: %lu too long for name, truncating!\n", len);
    len = 16;
  }

  entry *e = (entry*) here;
  memset(e, 0, sizeof(entry));
  memcpy(e->name, name, len);

  e->next = last;
  e->len  = (BYTE) len;
  here    = after_entry(e);
  current = e;

  return e;
}

void comma(CELL c) {
  *(PCELL)here = c;
  here += sizeof(CELL);
}

void move_here(CELL count) {
  here += count;
}

void print_dictionary() {
  entry *this = last;
  int count = 0;

  printf("\n");

  while(this) {
    printf(" %p ", this);
    cprint(this->name, this->len);
    for(int i = NAMELEN-this->len; i; i--) putchar(32);
    printf(" %p %02x %016lx\n", this->code, this->f1, this->data);
    count++;
    this = this->next;
  }

  printf("\n Dictionary contains %d entries, consuming %lu bytes\n\n",
	 count, here - dictionary);
}

void def_bif(PBYTE name, int len, VOIDFUN fn) {
  entry *e = create_entry(name, len);
  e->code = do_bif;
  e->data = (CELL) fn;
  last = current;
}

void print_entry(entry *e) {
  printf("name:    "); 
  cprint(e->name, e->len);
  putchar(10);

  printf("next:    %p\n", e->next);
  printf("len:     %d\n", e->len);
  printf("flags:   %02x%02x%02x\n", e->f1, e->f2, e->f3);
  printf("code:    %p\n", e->code);
  printf("data:    %p\n", (void*)e->data);
  printf("Content: %p\n", (void*)(e + 1));
}
