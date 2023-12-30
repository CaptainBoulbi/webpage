#include "config.h"

UserConfig config = {
  .port = 443,
  .addr = "https://github.com/CaptainBoulbi/webpage",
};

void getUserConfig(int argc, char* argv[]){
  for (int i=1; i<argc; i++){
    if (strcmp(argv[i], "-p") == 0){
      if (argc <= (i+1)){
        perror("ERROR: Port not defined by user");
        exit(1);
      }
      config.port = atoi(argv[++i]);
    } else {
      config.addr = argv[i];
    }
  }
}
