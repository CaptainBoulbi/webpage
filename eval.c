#include "eval.h"
#include "format.h"
#include "lexer.h"
#include "form.h"

#define PAGE_WIDTH 80

#define EXPAND_LIT(x) x, sizeof(x)

PageState state = {0};

void print_text(const char* text, int len){
  for (int i=0; i<len; i++){
    if (state.beginLine) printf("\n");

    printf("%c", text[i]);
    state.y++;
    state.beginLine = 0;

    if (state.y >= PAGE_WIDTH || text[i] == '\n'){
      state.y = 0;
      state.x++;
      state.beginLine = 1;
    }
  }

  state.beginLine = state.beginLine || text[len] == '\n';
}

void break_line(void){
  state.y = 0;
  state.x++;
  state.beginLine = 1;
}

void evaluate(Token* token){
  if (token->type == TITLE)
  {
    printf(FC_BOLD);
    print_text(EXPAND_LIT("--- "));
  }
  else if (token->type == END_TITLE)
  {
    print_text(EXPAND_LIT(" ---"));
    printf(FC_nBOLD);
    break_line();
  }
  else if (token->type == LI)
  {
    print_text(EXPAND_LIT("- "));
  }
  else if (token->type == END_LI)
  {
    //print_text(EXPAND_LIT("\n"));
  }
  else if (token->type == STRONG || token->type == B)
  {
    printf(FC_BOLD);
  }
  else if (token->type == END_STRONG || token->type == END_B)
  {
    printf(FC_nBOLD);
    print_text(EXPAND_LIT(" "));
  }
  else if (token->type == EM || token->type == I)
  {
    printf(FC_ITA);
  }
  else if (token->type == END_EM || token->type == END_I)
  {
    printf(FC_nITA);
  }
  else if (token->type == IMG)
  {
    print_text(EXPAND_LIT("["));
    print_text(EXPAND_LIT("IMG"));
    print_text(EXPAND_LIT("]"));
  }
  else if (token->type == A)
  {
    print_text(EXPAND_LIT("["));
    char* s = malloc(sizeof(int)*2);
    sprintf(s, "%d", state.nbLink);
    print_text(s, strlen(s));
    state.nbLink++;
    free(s);
    print_text(EXPAND_LIT("]"));
    printf(FC_UDL);
  }
  else if (token->type == END_A)
  {
    printf(FC_nUDL);
    print_text(EXPAND_LIT(" "));
  }
  else if (token->type == END_P)
  {
    break_line();
  }
  else if (token->type == TEXT)
  {
    print_text(token->value, token->len);
  }
  else if (token->type == H1 || token->type == H2 || token->type == H3 || token->type == H4 || token->type == H5 || token->type == H6)
  {
    break_line();
    printf(FC_BOLD);
  }
  else if (token->type == END_H1 || token->type == END_H2 || token->type == END_H3 || token->type == END_H4 || token->type == END_H5 || token->type == END_H6)
  {
    printf(FC_nBOLD);
  }
  else if (token->type == HR)
  {
    printf(FC_nUDL);
    break_line();
    print_text(EXPAND_LIT("-------------------------------------------------------------------------------"));
  }
}
