#include "config.h"
#include "page.h"
#include "lexer.h"
#include "eval.h"

int main(int argc, char* argv[]){

  getUserConfig(argc, argv);
  getPage();

  //printPage();

  Token* token = NULL;
  do {
    token = nexttoken();
    evaluate(token);
  } while (token != NULL && token->type != END_HTML);

  printf("\n");

  return 0;
}
