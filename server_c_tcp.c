// the website is:
// http://www.linuxhowtos.org/data/6/server_c_tcp.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{	
	int size = 128;
	// this is the original code
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[size];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
       error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
    int sum = 0;

     
    while (1) {
     	char fail []= "From server: Sorry, cannot compute!\n";
     	// printf("%lu\n",strlen(fail));
	    listen(sockfd,5);
	    clilen = sizeof(cli_addr);
	    newsockfd = accept(sockfd, 
	                 (struct sockaddr *) &cli_addr, 
	                 &clilen);
	    bzero(buffer,size);
	    n = read(newsockfd,buffer,size);

	    // this is the code I added

	    while(1){
	     	int notDigit = 0;

	     	if (strlen(buffer) > size) {
                n = write(newsockfd,fail,strlen(fail));
                break;
            }
            sum = 0; // initialize sum = 0 every iteration
            for (int i=0;i<strlen(buffer)-1;i++) { 
                if (!isdigit((unsigned char)buffer[i])) {
                	// fprint("%d\n",notDigit);
                	notDigit = 1;
                    n = write(newsockfd,fail,strlen(fail));
                    break;
                }
                char k = buffer[i];
                // fprint("%c\n",k);
			   int j = k - '0';
			   sum+=j; 
			   // fprint("%d\n",sum);
            }
            if (notDigit) break;
            bzero(buffer,size); 
            sprintf(buffer, "%d\n", sum);
            n = write(newsockfd,buffer, size);
            if (strlen(buffer) == 2) break;

	     }

	     // if (n < 0) error("ERROR reading from socket");
	     // printf("Here is the message: %s\n",buffer);
	     // n = write(newsockfd,"I got your message",18);
	     // if (n < 0) error("ERROR writing to socket");

     }
     return 0; 
}

