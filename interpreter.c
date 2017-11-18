#include "interpreter.h"

#include "dictionary.h"
#include "stacks.h"

void do_constant() {
  push(work);
  pip++;
}

void do_variable() {
  pushp((PCELL)work);
  pip++;
}

void do_bif() {
  ((VOIDFUN)work)();
  pip++;
}

void do_colon() {
  rpush(pip);
  pip = (PCELL)work;
}

void inner() {
  while(*pip) {
    entry *e = (entry*)*pip;
    work = e->data;
    e->code();
  }
}  

