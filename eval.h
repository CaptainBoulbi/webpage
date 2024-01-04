#ifndef EVAL_H
#define EVAL_H

#include "lexer.h"

typedef struct PageState {
  int nbLink;
  int x;
  int y;
  int beginLine;
  int inList;
} PageState;

void evaluate(Token* token);

#endif // EVAL_H
