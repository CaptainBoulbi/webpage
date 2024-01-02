#include "lexer.h"

#define HTML_BALISE_LEN 12

typedef struct Cursor {
  int chunk;
  int offset;
} Cursor;

Cursor curr = {
  .chunk = 0,
  .offset = -1,
};

Cursor prev = {
  .chunk = 0,
  .offset = -2,
};

int increment_cursor(Cursor* cursor){
  if (cursor->offset+1 < 0){
    cursor->offset++;
    return 0;
  }

  if (page.chunks[cursor->chunk][cursor->offset+1] == '\0'){
    cursor->chunk++;
    cursor->offset = 0;
  } else {
    cursor->offset++;
  }

  if (cursor->chunk >= page.len){
    return -1;
  }

  return 0;
}

char* nextchar(void){
  if (increment_cursor(&curr) < 0 || increment_cursor(&prev) < 0){
    return NULL;
  }

  if (curr.chunk >= page.len){
    return NULL;
  }

  return &page.chunks[curr.chunk][curr.offset];
}

void go_back(void){
  curr.chunk = prev.chunk;
  curr.offset = prev.offset;
  prev.offset--;

  if (curr.chunk < 0 || curr.offset < 0){
    puts("ERROR: go way too back.");
    exit(1);
  }
}

TokenType token_by_name(const char name[HTML_BALISE_LEN]){
  if (name == NULL){
    return UNDEFINED_TYPE;
  } else if (strncmp(name, "body", HTML_BALISE_LEN) == 0){
    return BODY;
  } else if (strncmp(name, "/body", HTML_BALISE_LEN) == 0){
    return END_BODY;
  } else if (strncmp(name, "html", HTML_BALISE_LEN) == 0){
    return HTML;
  } else if (strncmp(name, "/html", HTML_BALISE_LEN) == 0){
    return END_HTML;
  } else if (strncmp(name, "a", HTML_BALISE_LEN) == 0){
    return A;
  } else if (strncmp(name, "/a", HTML_BALISE_LEN) == 0){
    return END_A;
  } else if (strncmp(name, "ul", HTML_BALISE_LEN) == 0){
    return UL;
  } else if (strncmp(name, "li", HTML_BALISE_LEN) == 0){
    return LI;
  } else if (strncmp(name, "h1", HTML_BALISE_LEN) == 0){
    return H1;
  } else if (strncmp(name, "h2", HTML_BALISE_LEN) == 0){
    return H2;
  } else if (strncmp(name, "h3", HTML_BALISE_LEN) == 0){
    return H3;
  } else if (strncmp(name, "h4", HTML_BALISE_LEN) == 0){
    return H4;
  } else if (strncmp(name, "h5", HTML_BALISE_LEN) == 0){
    return H5;
  } else if (strncmp(name, "h6", HTML_BALISE_LEN) == 0){
    return H6;
  }

  return DONT_CARE;
}

void create_text_token(Token* token, char* cursor){
  int i = 0;

  do {
    cursor = nextchar();
    i++;
  } while (*cursor != '<');
  go_back();

  token = malloc(sizeof(Token));
  token->type = TEXT;
  token->value = "TODO";
  token->len = i;
}

void create_balise_token(Token* token, char* cursor){
  char balise[HTML_BALISE_LEN] = {0};
  int len = 0;

  cursor = nextchar();
  while (*cursor != '>' && *cursor != ' ' && *cursor != '\n' && *cursor != '\t' && len < HTML_BALISE_LEN){
    balise[len] = *cursor;
    len++;
    cursor = nextchar();
  }

  token = malloc(sizeof(Token));
  token->type = token_by_name(balise);
  token->value = malloc(sizeof(char) * len);
  strncpy(token->value, balise, len+1);
  token->len = len;

  go_back();
  do {
    cursor = nextchar();
  } while (*cursor != '>');
}

Token* nexttoken(void){
  Token* token = NULL;
  static char* cursor = NULL;
  cursor = nextchar();

  if (cursor == NULL) return NULL;

  while (*cursor == '\0' || *cursor == ' ' || *cursor == '\n' || *cursor == '\t'){
    cursor = nextchar();
  }

  if (*cursor != '<'){
    create_text_token(token, cursor);
  } else if (*cursor == '<'){
    create_balise_token(token, cursor);
  }

  return token;
}

void printtoken(Token* token){
  if (token == NULL){
    puts("NULL TOKEN");
    return;
  }

  switch (token->type) {
    case UNDEFINED_TYPE:
      printf("UNDEFINED_TYPE: ");
      break;
    case DONT_CARE:
      printf("DONT_CARE: ");
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
    case HTML:
      printf("HTML: ");
      break;
    case END_HTML:
      printf("END_HTML: ");
      break;
    case A:
      printf("A: ");
      break;
    case END_A:
      printf("END_A: ");
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
      printf("ERROR: UNKNOWN TOKEN: ");
      break;
  }

  if (token->value == NULL){
    puts("'NO VALUE FOUND'");
    return;
  }

  printf("'%s'\n", token->value);
}
