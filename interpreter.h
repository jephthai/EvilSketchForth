#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "global.h"

void do_constant();
void do_variable();
void do_bif();
void do_colon();
void inner();

#endif
