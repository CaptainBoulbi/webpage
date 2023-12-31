#include "lexer.h"
#include "page.h"
#include <string.h>

Cursor cursor = {
  .chunk = 0,
  .offset = 0,
};

char* nextchar(void){
  if (page.chunks[cursor.chunk][cursor.offset+1] == '\0'){
    cursor.chunk++;
    cursor.offset = 0;
  } else {
    cursor.offset++;
  }

  return &page.chunks[cursor.chunk][cursor.offset];
}

Token* nexttoken(void){
  Token* token = malloc(sizeof(Token));
  token->type = NO_TYPE;

  /*
  while (nextchar() == '<');

  char c = '\0';
  do {
    c = nextchar();
    printf("%c", c);;
  } while (c != '>');
  puts("");
  */

  puts("");
  for (int i=0; i<200; i++){
    printf("%c", *nextchar());
  }
  puts("");

  return token;
}

void printtoken(Token* token){
  switch (token->type) {
    case END_BODY:
      printf("END_BODY: ");
      break;
    case NO_TYPE:
      printf("NO_TYPE: ");
      break;
    default:
      printf("OTHER_TOKEN: ");
      break;
  }

  if (token->value == NULL){
    puts("NO VALUE FOUND");
    return;
  }

  printf("%s\n", token->value);
}
