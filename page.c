#include "config.h"
#include "page.h"

PageHTML page = {
  .chunks = NULL,
  .cap = DEFAULT_PAGE_LEN,
  .len = 0,
};

size_t save_chunk(char* buffer, size_t itemsize, size_t nitems, void* ignore){
  size_t bytes = itemsize * nitems;

  if (page.len >= page.cap){
    page.cap *= 1.5;
    page.chunks = realloc(page.chunks, sizeof(*page.chunks) * page.cap);

    if (page.chunks == NULL){
      perror("ERROR: Buy more ram.\n");
      exit(1);
    }

    memset(&page.chunks[page.len], 0, page.cap - page.len);
  }

  page.chunks[page.len] = malloc(bytes+1);
  page.chunks[page.len][bytes] = '\0';
  strncpy(page.chunks[page.len], buffer, bytes);
  page.len++;

  return bytes;
}

void getPage(void){
  page.chunks = malloc(sizeof(*page.chunks) * DEFAULT_PAGE_LEN);
  if (page.chunks == NULL){
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
  for (int i=0; i<page.len; i++){
    printf("%s", page.chunks[i]);
  }
}
