#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <assert.h>

#define DEFAULT_PAGE_LEN 16

int pageChunkLen = DEFAULT_PAGE_LEN;
int nbchunck = 0;
char** pageChunk;

typedef struct UserConfig {
  int port;
  char* addr;
} UserConfig;

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

size_t save_chunk(char* buffer, size_t itemsize, size_t nitems, void* ignore){
  size_t bytes = itemsize * nitems;

  if (nbchunck >= pageChunkLen){
    pageChunkLen *= 1.5;
    pageChunk = realloc(pageChunk, sizeof(*pageChunk) * pageChunkLen);

    if (pageChunk == NULL){
      perror("ERROR: Buy more ram.\n");
      exit(1);
    }

    memset(&pageChunk[nbchunck], 0, pageChunkLen - nbchunck);
  }

  pageChunk[nbchunck] = malloc(bytes+1);
  pageChunk[nbchunck][bytes] = '\0';
  strncpy(pageChunk[nbchunck], buffer, bytes);
  nbchunck++;

  return bytes;
}

void getPage(void){
  pageChunk = malloc(sizeof(*pageChunk) * DEFAULT_PAGE_LEN);
  if (pageChunk == NULL){
    perror("ERROR: Buy more ram.\n");
    exit(1);
  }

  CURL* curl = curl_easy_init();
  if (curl == NULL){
    perror("ERROR: curl init failed.\n");
    exit(1);
  }

  curl_easy_setopt(curl, CURLOPT_URL, config.addr);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, save_chunk);

  CURLcode result = curl_easy_perform(curl);
  if (result != CURLE_OK){
    fprintf(stderr, "CURL ERROR: %s\n", curl_easy_strerror(result));
  }

  curl_easy_cleanup(curl);
}

void printPage(void){
  for (int i=0; i<nbchunck; i++){
    printf("%s", pageChunk[i]);
  }
}

int main(int argc, char* argv[]){

  getUserConfig(argc, argv);
  getPage();

  printPage();

  return 0;
}
