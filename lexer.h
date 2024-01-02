#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "page.h"

typedef enum TokenType {
  UNDEFINED_TYPE,
  DONT_CARE,
  TEXT,
  BODY, END_BODY,
  HTML, END_HTML,
  A, END_A,
  UL, LI,
  H1, H2, H3, H4, H5, H6,
} TokenType;

typedef struct Token {
  TokenType type;
  char* value;
  int len;
} Token;

Token* nexttoken(void);

void printtoken(Token* token);

#endif // LEXER_H
