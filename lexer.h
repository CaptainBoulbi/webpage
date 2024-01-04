#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "page.h"

// 46 enum
typedef enum TokenType {
  UNDEFINED_TYPE,
  DONT_CARE,
  TITLE, END_TITLE,
  TEXT, END_HTML,
  BODY, END_BODY,
  A, END_A,
  IMG, END_IMG,
  BLOCKQUOTE, END_BLOCKQUOTE,
  CODE, END_CODE,
  HR, BR, END_P,
  PROGRESS, END_PROGRESS,
  STRONG, B, I, EM,
  END_STRONG, END_B, END_I, END_EM,
  UL, OL, LI, END_UL, END_OL, END_LI,
  TABLE, END_TABLE, THEAD,
  END_THEAD, TBODY, END_TBODY,
  TR, TH, END_TR, END_TH,
  H1, H2, H3, H4, H5, H6,
  END_H1, END_H2, END_H3, END_H4, END_H5, END_H6,
} TokenType;

typedef struct Token {
  TokenType type;
  char* value;
  int len;
} Token;

Token* nexttoken(void);

void printtoken(Token* token);

#endif // LEXER_H
