#include "config.h"
#include "page.h"
#include "lexer.h"

int main(int argc, char* argv[]){

  getUserConfig(argc, argv);
  getPage();

  printPage();

  Token* token;
  do{
    token = nexttoken();
    printtoken(token);
    //parse(token);
  } while (token->type != NO_TYPE);

  return 0;
}
