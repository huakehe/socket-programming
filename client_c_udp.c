// the website is:
// http://www.linuxhowtos.org/data/6/client_udp.c
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

void error(const char *);
int main(int argc, char *argv[])
{	
	// this is the original code
	int size = 128;
   int sock, n;
   unsigned int length;
   struct sockaddr_in server, from;
   struct hostent *hp;
   char buffer[size];
   
   if (argc != 3)  error("Usage: server port\n");
   
   sock= socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("socket");

   server.sin_family = AF_INET;
   hp = gethostbyname(argv[1]);
   if (hp==0) error("Unknown host");

   bcopy((char *)hp->h_addr, 
        (char *)&server.sin_addr,
         hp->h_length);
   server.sin_port = htons(atoi(argv[2]));
   length=sizeof(struct sockaddr_in);

   printf("Enter a string: ");
   bzero(buffer,size);
   fgets(buffer,size,stdin);
   n=sendto(sock,buffer,
            strlen(buffer),0,(const struct sockaddr *)&server,length);
   if (n < 0) error("Sendto");


   // this is the code I write
   while (1) {
    bzero(buffer,size); 
    n = recvfrom(sock,buffer,size,0,(struct sockaddr *)&from, &length);
        if(!isdigit(buffer[0])) { 
            printf("%s",buffer);
            break;
        }
        else if  (strlen(buffer) == 2) {
            printf("From server: %s",buffer);
            break;
        }
        else
            printf("From server: %s",buffer);
    }
    close(sock);

   return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}