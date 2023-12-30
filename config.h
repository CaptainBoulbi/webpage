#ifndef CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct UserConfig {
  int port;
  char* addr;
} UserConfig;

extern UserConfig config;

void getUserConfig(int argc, char* argv[]);

#endif // CONFIG_H
