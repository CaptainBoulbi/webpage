#ifndef PAGE_H
#define PAGE_H

#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>


#define DEFAULT_PAGE_LEN 16

typedef struct PageHTML {
  char** chunks;
  int cap;
  int len;
} PageHTML;

extern PageHTML page;

size_t save_chunk(char* buffer, size_t itemsize, size_t nitems, void* ignore);

void getPage(void);

void printPage(void);

#endif // PAGE_H
