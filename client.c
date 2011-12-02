#include "csapp.h"

#define VERSION " HTTP/1.1\r\n"

char *allcaps(char *str);
void host2header(char *host, char *header);

int main(int argc, char **argv)
{
  int clientfd, port;
  char *host;
  char buffer[1024];

  char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], requesthdr[MAXLINE];
  char version[MAXLINE];
  //int bufsize;
  rio_t rio;

  FILE *file;

  if(argc!=3){
    fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
    exit(0);
  }

  host=argv[1];
  port=atoi(argv[2]);

  /* generate the request header for this host */
  host2header(host,requesthdr);
 
  clientfd = Open_clientfd(host, port);
  Rio_readinitb(&rio, clientfd);
  
  while(Fgets(buf, MAXLINE, stdin)!=NULL){

    sscanf(buf ,"%s %s %s", method, uri, version);
    if(!strcasecmp(method, "GET")){

      printf("<%s>\n",buf);
      /* create request string */
      strcpy(buf,"GET");
      if (uri[strlen(uri)-1] == '/')
	strcat(uri, "home.html");
      strcat(buf," ");
      strcat(buf, uri);
      /* add header */
      strcat(buf, VERSION);
      strcat(buf, requesthdr);
      printf("%s",buf);

      file = Fopen(uri+1,"w");
      printf("opened file: %s\n",uri);

      /* send a get command to the server */
      Rio_writen(clientfd, buf, strlen(buf));
      while(Rio_readlineb(&rio,buf,MAXLINE))
	Fputs(buf, file)
	  ;
	
      printf("wrote to file: %s\n",uri);
      Fclose(file);
    }
  }
  /*
  while(Fgets(buf, MAXLINE, stdin)!=NULL){
    Rio_writen(clientfd, buf, strlen(buf));
    Rio_readlineb(&rio, buf, MAXLINE);
    Fputs(buf, stdout);
    }*/
  Close(clientfd);
  exit(0);
}

/* allcaps - returns str with all letters capitalized */
char *allcaps(char *str)
{
  int i=0;
  while(str[i]){
    putchar(toupper(str[i]));
    i++;
  }
  return str;
}

/* host2header - compiles a HTTP request header from host name */
void host2header(char *host,char *header){
  strcpy(header,"Host: ");
  strcat(header,host);
  strcat(header,"\r\n\r\n");
}

//void genrequest(char *
