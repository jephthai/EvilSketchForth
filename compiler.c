#include "compiler.h"

//
// This is the core compiler, implemented using a combination of C and
// Forth primitives.  Some of this isn't the prettiest, because it is
// in a sort of transitional phase, between C and Forth.  I could
// probably do a lot of work here to migrate it more in either
// direction, whether I felt one was more clear or educationally
// useful than the other.  But for now, it works, and is at least
// somewhat readable!
//
//

#define  lit(n)    (CELL)lookup(cslit(n))

#include "interpreter.h"
#include "dictionary.h"
#include "stacks.h"
#include "core_words.h"

void def_variable(char *name) {
  entry *e = create_entry((PBYTE) name, strlen(name));
  e->code = do_variable;
  e->data = (CELL) here;
  comma(0);
}

void def_colon(char *name) {
  entry *e = create_entry((PBYTE) name, strlen(name));
  e->code = do_colon;
  e->data = (CELL) here;
}

void f_compile() {
  mode = INTERPRET;
}

void f_interpret() {
  mode = COMPILE;
}

void f_immediate() {
  current->f1 = 1;
}

void f_bl() {
  while(1) {
    f_key();
    BYTE b = (BYTE) pop();
    switch(b) {
    case '\n':
    case '\r':
      if(input == stdin) {
	printf("\x1b[35;1mok.\x1b[0m "); fflush(stdout);
      }
    case ' ':
    case '\t':
      if(tibtop > tibstart) {
	push((CELL)(&tib[tibstart]));
	push(tibtop - tibstart);
	tibstart = tibtop;
	if(b == 10) {
	  tibstart = 0;
	  tibtop = 0;
	}
	return;
      }
      break;
    default:
      tib[tibtop++] = b;
    }
  }
}

void f_lit() {
  comma((CELL)lookup(cslit("push")));
  comma(pop());
}

void f_create() {
  f_bl();
  f_header();
  current->code = do_variable;
  current->data = (CELL)(current + 1);
}

void f_exec() {
  entry *e = (entry*) popp();
  work = e->data;
  e->code();
  // because do_colon will increment, we have to be careful
  pip--;
}

void outer() {
  while(1) {
    f_bl();
    f_over();
    f_over();
    f_find();
    f_dup();
    if(pop()) {
      switch(mode) {
      case COMPILE:
	f_dup();
	entry *e = (entry*) pop();
	if(e->f1) {
	  rpush(0);
	  temp[0] = (CELL)popp();
	  temp[1] = 0;
	  pip = temp;
	  pop();
	  pop();
	  inner();
	} else {
	  f_comma();
	  pop();
	  pop();
	}
	break;
      case INTERPRET:
	rpush(0);
	temp[0] = (CELL)popp();
	temp[1] = 0;
	pip = temp;
	pop();
	pop();
	inner();
	break;
      }
    } else {
      f_drop();
      CELL a = pop();
      PBYTE b = (PBYTE) pop();
      b[a] = 0;
      if(isdigit(b[0]) || b[0] == '-') {
	push(atol((char*)b));
	if(mode == COMPILE) {
	  pushp((PCELL)lookup(cslit("push")));
	  f_comma();
	  f_comma();
	}
      } else {
	printf("\x1b[31;1mERROR: undefined word '%s'\x1b[0m\n", b);
	psp = 0;
      }
    }
  }
}

void colon() {
  f_bl();
  f_header();
  f_docol();
  mode = COMPILE;
}

void f_pheader() {
  print_entry((entry*)popp());
}

void semicolon() {
  pushp((PCELL)lookup(cslit("ret")));
  f_comma();
  f_reveal();
  mode = INTERPRET;
}

void init_compiler() {
  input    = stdin;
  mode     = INTERPRET;
  tibtop   = 0;
  tibstart = 0;

  memset(temp, 0, sizeof(temp));
  memset(tib,  0, sizeof(tib));

  def_bif(cslit("["),         f_compile);     f_immediate();
  def_bif(cslit("immediate"), f_immediate);   f_immediate();
  def_bif(cslit("lit"),       f_lit);         f_immediate();
  def_bif(cslit(";"),         semicolon);     f_immediate();
  def_bif(cslit("reveal"),    f_reveal);      f_immediate();

  def_bif(cslit("[reveal]"),  f_reveal);
  def_bif(cslit("bl"),        f_bl);
  def_bif(cslit(":"),         colon);
  def_bif(cslit("create"),    f_create);
  def_bif(cslit("execute"),   f_exec);
  def_bif(cslit("]"),         f_interpret); 
  def_bif(cslit(".header"),   f_pheader);

  input = fopen("core.fth", "r");
}
