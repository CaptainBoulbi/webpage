#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>

typedef enum TokenType {
  NO_TYPE,
  TEXT,
  BODY,
  END_BODY,
  UL, LI,
  H1, H2, H3, H4, H5, H6,
} TokenType;

typedef struct Token {
  TokenType type;
  char* value;
} Token;

typedef struct Cursor {
  int chunk;
  int offset;
} Cursor;

Token* nexttoken(void);

void printtoken(Token* token);

#endif // LEXER_H
