#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <string.h>

char page[1024];

typedef struct UserConfig {
  int port;
  int addr;
} UserConfig;
UserConfig config = {
  .port = 443,
  .addr = 0x08080808,
};

void getPage(void){
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1){
    perror("ERROR: Could not create socket.\n");
    exit(-1);
  }

  struct sockaddr_in addr = {
    .sin_family = AF_INET,
    .sin_port = htons(config.port),
    .sin_addr = htonl(config.addr),
  };

  if (connect(sockfd, (const struct sockaddr*)&addr, sizeof(addr)) == -1){
    perror("ERROR: Could not connect.\n");
    exit(-1);
  }

  SSL_CTX* ctx = SSL_CTX_new(TLS_method());
  if (ctx == NULL){
    perror("ERROR: Could not create SSL_CTX object.\n");
    exit(-1);
  }
  SSL* ssl = SSL_new(ctx);
  if (ssl == NULL){
    perror("ERROR: Could not create SSL structure.\n");
    exit(-1);
  }
  if (SSL_set_fd(ssl, sockfd) != 1){
    perror("ERROR: Could not connect SSL object file descriptor.\n");
    exit(-1);
  }
  if (SSL_connect(ssl) != 1){
    perror("ERROR: Could not initiate TLS/SSL connection.\n");
    exit(-1);
  }

  char* request = "GET /\r\n\r\n";
  if (SSL_write(ssl, request, strlen(request)) <= 0){
    perror("ERROR: Could not send request.\n");
    exit(-1);
  }
  char buffer[1024] = {0};
  int readbyte = SSL_read(ssl, buffer, 1023);
  if (readbyte <= 0){
    perror("ERROR: Could not read from connection.\n");
    exit(-1);
  }

  strcpy(page, buffer);
}

void getUserConfig(int argc, char* argv[]){
  for (int i=1; i<argc; i++){
    if (strcmp(argv[i], "-p") == 0){
      if (argc <= (i+1)){
        perror("ERROR: Port not defined by user");
        exit(1);
      }
      config.port = atoi(argv[++i]);
    } else {
      config.addr = atoi(argv[i]);
    }
  }
}

int main(int argc, char* argv[]){

  getUserConfig(argc, argv);

  printf("conf: port = %d - addr = %d\n", config.port, config.addr);

  getPage();

  puts(page);

  return 0;
}
