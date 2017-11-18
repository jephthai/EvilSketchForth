#ifndef CORE_WORDS_H
#define CORE_WORDS_H

#include "global.h"

// Stack manipulators
void f_push();
void f_pop();
void f_dup();
void f_2dup();
void f_nip();
void f_tuck();
void f_over();
void f_drop();
void f_swap();
void f_rot();
void f_tor();
void f_rfrom();

// Basic math
void f_plus();
void f_minus();
void f_mul();
void f_div();
void f_mod();

// Tests and comparisons
void f_equal();
void f_less();
void f_more();

// bit operations
void f_or();
void f_and();
void f_xor();

// Control flow
void f_goto();
void f_branch();

// Memory interaction
void f_fetch();
void f_store();
void f_cfetch();
void f_cstore();
void f_comma();
void f_ccomma();
void f_here();
void f_incr();
void f_decr();
void f_dump();

// Dictionary entries
void f_header();
void f_docol();
void f_dovar();
void f_dobif();
void f_docon();
void f_find();
void f_last();
void f_words();
void f_databang();

// I/O
void f_emit();
void f_key();
void f_dots();
void f_dot();

// Interpreter words
void f_ret();
void f_bye();
void f_mode();
void f_reveal();

// BIF Functions
void f_cbif();
void f_cbifs();

#endif
