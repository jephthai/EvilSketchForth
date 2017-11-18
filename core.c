#include "core.h"

//
// Define the dictionary entries for the core language.  These need to
// exist so that 'core.fth' can build up the rest of the core language
// with definitions compiled by the compiler.  I've done some work to
// pare this down to a degree of minimalism, but I'm sure there are
// some more steps that could be taken to simplify this.  I've read
// that Forth can be defined from the ground up with only eight or
// nine core definitions!
//
//

#include "dictionary.h"
#include "core_words.h"
#include "interpreter.h"

void hello() {
  printf("\x1b[31;1mHELLO!\x1b[0m\n");
}

CELL dbl(CELL x) {
  return x * 2;
}

void init_core() {
  def_bif(cslit("and"),    f_and);
  def_bif(cslit("branch"), f_branch);
  def_bif(cslit("bye"),    f_bye);
  def_bif(cslit("c,"),     f_ccomma);
  def_bif(cslit("c@"),     f_cfetch);
  def_bif(cslit("c@"),     f_cfetch);
  def_bif(cslit("c!"),     f_cstore);
  def_bif(cslit("c!"),     f_cstore);
  def_bif(cslit("cbif"),   f_cbif);
  def_bif(cslit("cbifs"),  f_cbifs);
  def_bif(cslit("data,"),  f_databang);
  def_bif(cslit("dobif"),  f_dobif);
  def_bif(cslit("docol"),  f_docol);
  def_bif(cslit("docon"),  f_docon);
  def_bif(cslit("dovar"),  f_dovar);
  def_bif(cslit("drop"),   f_drop);
  def_bif(cslit("dump"),   f_dump);
  def_bif(cslit("dup"),    f_dup);
  def_bif(cslit("emit"),   f_emit);
  def_bif(cslit(","),      f_comma);
  def_bif(cslit("."),      f_dot);
  def_bif(cslit("="),      f_equal);
  def_bif(cslit("@"),      f_fetch);
  def_bif(cslit("find"),   f_find);
  def_bif(cslit("<"),      f_less);
  def_bif(cslit("-"),      f_minus);
  def_bif(cslit(">"),      f_more);
  def_bif(cslit("/"),      f_div);
  def_bif(cslit("*"),      f_mul);
  def_bif(cslit("+"),      f_plus);
  def_bif(cslit("!"),      f_store);
  def_bif(cslit("goto"),   f_goto);
  def_bif(cslit("header"), f_header);
  def_bif(cslit("here"),   f_here);
  def_bif(cslit("key"),    f_key);
  def_bif(cslit("last"),   f_last);
  def_bif(cslit("mode"),   f_mode);
  def_bif(cslit("or"),     f_or);
  def_bif(cslit("push"),   f_push);
  def_bif(cslit("ret"),    f_ret);
  def_bif(cslit("r>"),     f_rfrom);
  def_bif(cslit(">r"),     f_tor);
  def_bif(cslit(".s"),     f_dots);
  def_bif(cslit("swap"),   f_swap);
  def_bif(cslit("words"),  f_words);
  def_bif(cslit("xor"),    f_xor);

  bifs[0] = (PCELL)printf;
  bifs[1] = (PCELL)malloc;
  bifs[2] = (PCELL)free;
}
