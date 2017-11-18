#include "global.h"

// ------------------------------------------------------------------------
//
// This is a very simple indirect threaded code (ITC) compiler for a
// Forth.  It's not anywhere near standards compliant.  Instead, it's
// intended as a very straightforward sketch of a Forth environment.
// My purpose in making it is as a reference for translating parts of
// it into assembly. It may also be useful for educational purposes,
// as I've tried to write it in such a way as to be easily understood.
//
// If you're coming to the code from a fresh start, I'd recommend
// reviewing the source files in the following order:
//
// 1 global.h      defines some types and fundamental data structures
// 2 dictionary.c  implements the dictionary, which is core to forth
// 3 interpreter.c defines the mechanics of the inner interpreter
// 4 compiler.c    the outer interpreter and compiler bits
// 5 core_words.c  the underlying implementation of the core library
// 6 core.fth      the post-compiler foundation for the core language
//
// ------------------------------------------------------------------------

#include "stacks.h"
#include "dictionary.h"
#include "interpreter.h"
#include "core.h"
#include "core_words.h"
#include "compiler.h"

void banner() {
  printf("\nThis is \x1b[32;1mEvil \x1b[36;1mSketch\x1b[35;1mForth\x1b[0m, ");
  printf("by Josh Stone (yakovdk@gmail.com)\n\n");
  printf("Starting FORTH environment...\n\n");
}

void init() {
  init_stacks();
  init_dictionary();
  init_core();
  init_compiler();
}

int main(int argc, char **argv) {
  banner();
  init();
  outer();
  return 0;
} 

