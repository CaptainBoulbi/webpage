#include "config.h"
#include "page.h"
#include "lexer.h"

int main(int argc, char* argv[]){

  getUserConfig(argc, argv);
  getPage();

  printPage();

  Token* token = NULL;
  do {
    token = nexttoken();
    printtoken(token);
  } while (token != NULL && token->type != UNDEFINED_TYPE);

  //parse(token);

  return 0;
}
