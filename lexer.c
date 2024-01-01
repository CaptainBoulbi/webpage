#include "lexer.h"

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

  if (cursor.chunk >= page.len){
    return NULL;
  }

  return &page.chunks[cursor.chunk][cursor.offset];
}

char* HTMLbalise(void){
  return NULL;
}

Token* nexttoken(void){
  Token* token = malloc(sizeof(Token));
  token->value = NULL;

  char* word = HTMLbalise();
  if (word == NULL){
    token->type = NO_TYPE;
  } else if (strcmp(word, "body") == 0){
    token->type = BODY;
  } else if (strcmp(word, "/body") == 0){
    token->type = END_BODY;
  } else if (strcmp(word, "ul") == 0){
    token->type = UL;
  } else if (strcmp(word, "li") == 0){
    token->type = LI;
  } else if (strcmp(word, "h1") == 0){
    token->type = H1;
  } else if (strcmp(word, "h2") == 0){
    token->type = H2;
  } else if (strcmp(word, "h3") == 0){
    token->type = H3;
  } else if (strcmp(word, "h4") == 0){
    token->type = H4;
  } else if (strcmp(word, "h5") == 0){
    token->type = H5;
  } else if (strcmp(word, "h6") == 0){
    token->type = H6;
  } else {
    token->type = NO_TYPE;
  }

  return token;
}

void printtoken(Token* token){
  switch (token->type) {
    case NO_TYPE:
      printf("NO_TYPE: ");
      break;
    case TEXT:
      printf("TEXT: ");
      break;
    case BODY:
      printf("BODY: ");
      break;
    case END_BODY:
      printf("END_BODY: ");
      break;
    case UL:
      printf("UL: ");
      break;
    case LI:
      printf("LI: ");
      break;
    case H1:
      printf("H1: ");
      break;
    case H2:
      printf("H2: ");
      break;
    case H3:
      printf("H3: ");
      break;
    case H4:
      printf("H4: ");
      break;
    case H5:
      printf("H5: ");
      break;
    case H6:
      printf("H6: ");
      break;
    default:
      printf("UNDEFINED TOKEN: ");
      break;
  }

  if (token->value == NULL){
    puts("NO VALUE FOUND");
    return;
  }

  printf("%s\n", token->value);
}
