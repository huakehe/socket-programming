// the website is:
// http://www.linuxhowtos.org/data/6/server_udp.c


#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <ctype.h>


void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{	
	int size = 128;
    // this is the original code
    if (argc < 2)
        error("ERROR, no port provided\n");
    
    int sock, length, n;
    socklen_t fromlen; 
    struct sockaddr_in server;
    struct sockaddr_in from;
    char buffer[size];
    

    sock=socket(AF_INET, SOCK_DGRAM, 0); 
    if (sock < 0) error("Opening socket");
    length = sizeof(server);
    bzero(&server,length); 
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port=htons(atoi(argv[1])); 
    
 
    if (bind(sock,(struct sockaddr *)&server,length) < 0)
        error("binding");
    fromlen = sizeof(struct sockaddr_in);
 
    //this is the code I write

    while (1) { 
    	int sum = 0;
    	int notDigit = 0;
        bzero(buffer,size);
        n = recvfrom(sock,buffer,size,0,(struct sockaddr *)&from,&fromlen);
        char fail []= "From server: Sorry, cannot compute!\n";
        while(1) {
            if (strlen(buffer) > 129){
                n = sendto(sock,fail,strlen(fail),0, (struct sockaddr *)&from,fromlen);
                break;
            }
           sum = 0; // initialize sum = 0 every iteration
           for (int i=0;i<strlen(buffer)-1;i++){ 
               if (isdigit(buffer[i])==0){
               		notDigit = 1;
                   n = sendto(sock,fail,strlen(fail),0, (struct sockaddr *)&from,fromlen);
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
           n = sendto(sock,buffer, strlen(buffer),0, (struct sockaddr *)&from,fromlen);
           if (strlen(buffer) == 2) break;
        }
      }
    
    return 0;
}