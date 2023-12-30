#include "config.h"
#include "page.h"

int main(int argc, char* argv[]){

  getUserConfig(argc, argv);
  getPage();

  printPage();

  return 0;
}
