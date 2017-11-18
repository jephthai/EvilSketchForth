#include "core_words.h"

//
// core_words.c defines the implementation of the core words that need
// to exist before we can build the compiler.  This probably isn't the
// perfectly reduced set, but is at least somewhat svelte.  The idea
// is that once we build up the to the compiler, then the rest of the
// language can be built in forth, and we can divest ourselves from
// building implementations in C.
//
// This compiler is an educational exercise, so no effort is made to
// optimize these AT ALL.  This means some of the definitions are
// painstakingly inefficient.  Consider the implementation of "dup",
// for example, where I pop a value from the stack, and then push it
// twice.  The obvious optimization is simply to assign the next value
// in the stack array.  But then, optimizing these makes them somewhat
// more arcane, and detracts from the pedagogical value of avoiding
// complexity so the functionality is perfectly clear.
//
// These are defined in this file, but are actually added to the
// dictionary at runtime in the file "core.c".
//
//

#include "dictionary.h"
#include "stacks.h"

// Stack manipulators
void f_push() {
  CELL num = *(++pip);
  push(num);
}

void f_dup() {
  CELL a = pop();
  push(a);
  push(a);
}

 void f_over() {
   CELL a = pop();
   CELL b = pop();
   push(b);
   push(a);
   push(b);
 }

void f_drop() {
  drop();
}

void f_swap() {
  CELL a = pop();
  CELL b = pop();
  push(a);
  push(b);
}

void f_tor() {
  rpush(popp());
}

void f_rfrom() {
  pushp(rpop());
}

// Basic math
void f_plus() {
  CELL a = pop();
  CELL b = pop();
  push(b + a);
}

void f_minus() {
  CELL a = pop();
  CELL b = pop();
  push(b - a);
}

void f_mul() {
  CELL a = pop();
  CELL b = pop();
  push(b * a);
}

void f_div() {
  CELL a = pop();
  CELL b = pop();
  push(b / a);
}

void f_mod() {
  CELL a = pop();
  CELL b = pop();
  push(b % a);
  push(b / a);
}


// Tests and comparisons
void f_equal() {
  CELL a = pop();
  CELL b = pop();
  push(b == a ? -1 : 0);
}

void f_less() {
  CELL a = pop();
  CELL b = pop();
  push(b < a ? -1 : 0);
}

void f_more() {
  CELL a = pop();
  CELL b = pop();
  push(b > a ? -1 : 0);
}


// bit operations
void f_or() {
  CELL a = pop();
  CELL b = pop();
  push(a | b);
}

void f_and() {
  CELL a = pop();
  CELL b = pop();
  push(a & b);
}

void f_xor() {
  CELL a = pop();
  CELL b = pop();
  push(a ^ b);
}


// Control flow
void f_goto() {
  PCELL p = (PCELL) *(++pip);
  pip = p;
}

void f_branch() {
  CELL a = pop();
  PCELL p = (PCELL) *(++pip);
  if(!a) {
    pip = p;
  }
}


// Memory interaction
void f_fetch() {
  PCELL p = popp();
  push(*p);
}

void f_store() {
  PCELL p = popp();
  CELL  a = pop();
  *p = a;
}

void f_cfetch() {
  PCELL p = popp();
  push((*p) & 0xff);
}

void f_cstore() {
  PBYTE p = (PBYTE) popp();
  BYTE  b = pop() & 0xff;
  *p = b;
}

void f_comma() {
  *((PCELL)here) = pop();
  here += sizeof(CELL);
}

void f_ccomma() {
  *(here++) = (BYTE)(pop() & 0xff);
}

void f_here() {
  push((CELL)here);
}

void f_dump() {
  CELL  len = pop();
  PBYTE ptr = (PBYTE) pop();
  dump(ptr, len);
}

// Dictionary entries
void f_header() {
  CELL len = pop();
  PBYTE name = (PBYTE) pop();
  create_entry(name, len);
}

void f_docol() {
  current->code = do_colon;
  current->data = (CELL)(current + 1);
}

void f_docon() {
  current->code = do_constant;
  current->data = (CELL)(current + 1);
}

void f_dobif() {
  current->code = do_bif;
  current->data = (CELL)(current + 1);
}

void f_dovar() {
  current->code = do_variable;
  current->data = (CELL)(current + 1);
}

void f_find() {
  CELL len = pop();
  PBYTE name = (PBYTE) pop();
  push((CELL)lookup(name, len));
}

void f_last() {
  pushp((PCELL)last);
}

void f_words() {
  print_dictionary();
}

void f_databang() {
  current->data = (CELL)pop();
}

// I/O
void f_emit() {
  BYTE b = (BYTE)(pop() & 0xff);
  putchar(b);
  fflush(stdout);
}

void f_key() {
  uint8_t b = fgetc(input) & 0xff;
  if(feof(input)) {
    if(input == stdin) {
      f_bye();
    } else {
      input = stdin;
      printf("\n\x1b[35;1mok. \x1b[0m");
      fflush(stdout);
      f_key();
    }
  } else {
    push((CELL)(b));
  }
}

void f_dots() {
  print_stack();
}

void f_dot() {
  printf("%ld ", pop());
}

// Interpreter words
void f_ret() {
  pip = rpop();
}
  
void f_bye() {
  exit(0);
}

void f_mode() {
  push(mode);
}

void f_reveal() {
  last = current;
}

// BIF Functions
void f_cbif() {
  int i;
  CELL argnum = pop();
  CELL retq   = pop();
  CELL fn     = pop();
  CELL args[16];

  if(argnum > 0) {
    for(i = argnum; i; i--) {
      args[i - 1] = pop();
    }
  }
  
  switch(argnum) {
  case 0:
    if(retq) {
      push(((RETBIF0)fn)());
    } else {
      ((VOIDBIF0)fn)();
    }
    break;
  case 1:
    if(retq) {
      push(((RETBIF1)fn)(args[0]));
    } else {
      ((VOIDBIF1)fn)(args[0]);
    }
    break;
  case 2:
    if(retq) {
      push(((RETBIF2)fn)(args[0], args[1]));
    } else {
      ((VOIDBIF2)fn)(args[0], args[1]);
    }
    break;
  case 3:
    if(retq) {
      push(((RETBIF3)fn)(args[0], args[1], args[2]));
    } else {
      ((VOIDBIF3)fn)(args[0], args[1], args[2]);
    }
    break;
  case 4:
    if(retq) {
      push(((RETBIF4)fn)(args[0], args[1], args[2], args[3]));
    } else {
      ((VOIDBIF4)fn)(args[0], args[1], args[2], args[3]);
    }
    break;
  case 5:
    if(retq) {
      push(((RETBIF5)fn)(args[0], args[1], args[2], args[3], args[4]));
    } else {
      ((VOIDBIF5)fn)(args[0], args[1], args[2], args[3], args[4]);
    }
    break;
  case 6:
    if(retq) {
      push(((RETBIF6)fn)(args[0], args[1], args[2], args[3], args[4], args[5]));
    } else {
      ((VOIDBIF6)fn)(args[0], args[1], args[2], args[3], args[4], args[5]);
    }
    break;
  }
}

void f_cbifs() {
  pushp((PCELL)bifs);
}
