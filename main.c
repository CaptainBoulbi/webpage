#include "config.h"
#include "page.h"
#include "lexer.h"

int main(int argc, char* argv[]){

  getUserConfig(argc, argv);
  getPage();

  Token* token = NULL;
  do {
    token = nexttoken();
    printtoken(token);
    //evaluate(token);
  } while (token != NULL && token->type != END_BODY);

  return 0;
}
