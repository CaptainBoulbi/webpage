#include "lexer.h"

#define HTML_BALISE_LEN 12
#define DA_LEN 64

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

TokenType token_by_name(const char name[HTML_BALISE_LEN]);

void printtoken(Token* token){
  if (token == NULL){
    puts("NULL TOKEN");
    return;
  }

  printf("%d: ", token->type);

  if (token->value == NULL){
    puts("'NO VALUE FOUND'");
    return;
  }

  printf("'%s'\n", token->value);
}

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
    puts("ERROR: cursor got way too back.");
    exit(1);
  }
}

char* getParam(const char* word, int len, char* cursor, int* size){
  char* res = NULL;
  int found = 0;
  len--;

  do {
    cursor = nextchar();
    for (int i=0; i<len; i++){
      if (word[i] != *cursor){
        found = -1;
        break;
      } else {
        found = 1;
        cursor = nextchar();
      }
    }

    if (found == 1){
      while (*cursor != '"'){
        cursor = nextchar();
      }

      int cap = DA_LEN;
      res = malloc(sizeof(char) * cap);
      *size = 0;

      do {
        cursor = nextchar();
        res[*size] = *cursor;
        (*size)++;
        if (*size >= cap){
          cap *= 2;
          res = realloc(res, cap);
        }
      } while (*cursor != '"');

      res[*size-1] = '\0';

      break;
    }
  } while (*cursor != '>');


  return res;
}

Token* create_text_token(Token* token, char* cursor){
  token = malloc(sizeof(Token));
  token->value = malloc(sizeof(char) * DA_LEN);
  int i = 0, cap = DA_LEN;

  go_back();
  do {
    cursor = nextchar();
    token->value[i] = *cursor;
    i++;
    if (i >= cap){
      cap *= 2;
      token->value = realloc(token->value, cap);
    }
  } while (*cursor != '<');
  token->value[i-1] = '\0';
  go_back();

  token->type = TEXT;
  token->len = i-1;

  return token;
}

Token* create_balise_token(Token* token, char* cursor){
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

  if (token->type == A){
    token->value = getParam("href", sizeof("href"), cursor, &token->len);
  } else if (token->type == IMG) {
    int srclen, altlen, totallen;
    char* src = getParam("src", sizeof("src"), cursor, &srclen);
    char* alt = getParam("alt", sizeof("alt"), cursor, &altlen);
    totallen = srclen * altlen + 1;

    token->value = malloc(sizeof(char) * (totallen));
    strncpy(token->value, src, srclen);
    token->value[srclen-1] = ' ';
    strncpy(token->value+srclen, alt, altlen);
    token->value[totallen] = '\0';
    token->len = totallen;

  } else {
    token->value = malloc(sizeof(char) * len);
    strncpy(token->value, balise, len+1);
    token->len = len;
  }

  go_back();
  do {
    cursor = nextchar();
  } while (*cursor != '>');

  return token;
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
    token = create_text_token(token, cursor);
  } else if (*cursor == '<'){
    token = create_balise_token(token, cursor);
  }

  return token;
}

TokenType token_by_name(const char name[HTML_BALISE_LEN]){
  if (name == NULL){
    return UNDEFINED_TYPE;
  } else if (strncmp(name, "body", HTML_BALISE_LEN) == 0){
    return BODY;
  } else if (strncmp(name, "/body", HTML_BALISE_LEN) == 0){
    return END_BODY;
  } else if (strncmp(name, "/html", HTML_BALISE_LEN) == 0){
    return END_HTML;
  } else if (strncmp(name, "title", HTML_BALISE_LEN) == 0){
    return TITLE;
  } else if (strncmp(name, "/title", HTML_BALISE_LEN) == 0){
    return END_TITLE;
  } else if (strncmp(name, "em", HTML_BALISE_LEN) == 0){
    return EM;
  } else if (strncmp(name, "/em", HTML_BALISE_LEN) == 0){
    return END_EM;
  } else if (strncmp(name, "i", HTML_BALISE_LEN) == 0){
    return I;
  } else if (strncmp(name, "/i", HTML_BALISE_LEN) == 0){
    return END_I;
  } else if (strncmp(name, "b", HTML_BALISE_LEN) == 0){
    return B;
  } else if (strncmp(name, "/b", HTML_BALISE_LEN) == 0){
    return END_B;
  } else if (strncmp(name, "strong", HTML_BALISE_LEN) == 0){
    return STRONG;
  } else if (strncmp(name, "/strong", HTML_BALISE_LEN) == 0){
    return END_STRONG;
  } else if (strncmp(name, "hr", HTML_BALISE_LEN) == 0){
    return HR;
  } else if (strncmp(name, "br", HTML_BALISE_LEN) == 0){
    return BR;
  } else if (strncmp(name, "p", HTML_BALISE_LEN) == 0){
    return P;
  } else if (strncmp(name, "/p", HTML_BALISE_LEN) == 0){
    return END_P;
  } else if (strncmp(name, "a", HTML_BALISE_LEN) == 0){
    return A;
  } else if (strncmp(name, "/a", HTML_BALISE_LEN) == 0){
    return END_A;
  } else if (strncmp(name, "ol", HTML_BALISE_LEN) == 0){
    return OL;
  } else if (strncmp(name, "/ol", HTML_BALISE_LEN) == 0){
    return END_OL;
  } else if (strncmp(name, "ul", HTML_BALISE_LEN) == 0){
    return UL;
  } else if (strncmp(name, "/ul", HTML_BALISE_LEN) == 0){
    return END_UL;
  } else if (strncmp(name, "li", HTML_BALISE_LEN) == 0){
    return LI;
  } else if (strncmp(name, "/li", HTML_BALISE_LEN) == 0){
    return END_LI;
  } else if (strncmp(name, "img", HTML_BALISE_LEN) == 0){
    return IMG;
  } else if (strncmp(name, "/img", HTML_BALISE_LEN) == 0){
    return END_IMG;
  } else if (strncmp(name, "blockquote", HTML_BALISE_LEN) == 0){
    return BLOCKQUOTE;
  } else if (strncmp(name, "/blockquote", HTML_BALISE_LEN) == 0){
    return END_BLOCKQUOTE;
  } else if (strncmp(name, "code", HTML_BALISE_LEN) == 0){
    return CODE;
  } else if (strncmp(name, "/code", HTML_BALISE_LEN) == 0){
    return END_CODE;
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
  } else if (strncmp(name, "/h1", HTML_BALISE_LEN) == 0){
    return END_H1;
  } else if (strncmp(name, "/h2", HTML_BALISE_LEN) == 0){
    return END_H2;
  } else if (strncmp(name, "/h3", HTML_BALISE_LEN) == 0){
    return END_H3;
  } else if (strncmp(name, "/h4", HTML_BALISE_LEN) == 0){
    return END_H4;
  } else if (strncmp(name, "/h5", HTML_BALISE_LEN) == 0){
    return END_H5;
  } else if (strncmp(name, "/h6", HTML_BALISE_LEN) == 0){
    return H6;
  } else if (strncmp(name, "table", HTML_BALISE_LEN) == 0){
    return TABLE;
  } else if (strncmp(name, "/table", HTML_BALISE_LEN) == 0){
    return END_TABLE;
  } else if (strncmp(name, "thead", HTML_BALISE_LEN) == 0){
    return THEAD;
  } else if (strncmp(name, "/thead", HTML_BALISE_LEN) == 0){
    return END_THEAD;
  } else if (strncmp(name, "tbody", HTML_BALISE_LEN) == 0){
    return TBODY;
  } else if (strncmp(name, "/tbody", HTML_BALISE_LEN) == 0){
    return END_TBODY;
  } else if (strncmp(name, "tr", HTML_BALISE_LEN) == 0){
    return TR;
  } else if (strncmp(name, "/tr", HTML_BALISE_LEN) == 0){
    return END_TR;
  } else if (strncmp(name, "th", HTML_BALISE_LEN) == 0){
    return TH;
  } else if (strncmp(name, "/th", HTML_BALISE_LEN) == 0){
    return END_TH;
  } else if (strncmp(name, "progress", HTML_BALISE_LEN) == 0){
    return PROGRESS;
  } else if (strncmp(name, "/progress", HTML_BALISE_LEN) == 0){
    return END_PROGRESS;
  }

  return DONT_CARE;
}
