#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
   int sock, length, n;
   socklen_t fromlen;
   struct sockaddr_in server;
   struct sockaddr_in from;
   char buf[2000];

   if (argc < 2) {
      fprintf(stderr, "ERROR, no port provided\n");
      exit(0);
   }
   
   sock=socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("Opening socket");
   length = sizeof(server);
   bzero(&server,length);
   server.sin_family=AF_INET;
   server.sin_addr.s_addr=INADDR_ANY;
   server.sin_port=htons(atoi(argv[1]));
   if (bind(sock,(struct sockaddr *)&server,length)<0) 
       error("binding");
   fromlen = sizeof(struct sockaddr_in);
   while (1) {
       bzero(buf,2000);	
       n = recvfrom(sock,buf,2000,0,(struct sockaddr *)&from,&fromlen);
       if (n < 0) error("recvfrom");
       //loop for total sum
       int sum = 0;
       int length = strlen(buf);
       char result[2000];//char array
       for(int i = 0; i < length-1; i++){
         int number = buf[i] - '0';
         if (number < 0 || number > 9) {//check fora non-int type
            char error[23] = "Sorry, cannot compute!"; //create error message array
	    sendto(sock, error, 23, 0,(struct sockaddr *)&from,fromlen);
	    return 0;
            } //number retrieved
         sum += number; //summation of separate numbers
       }
       bzero(result,2000);
       sprintf(result, "%d", sum); //create new string 
       sendto(sock, result, strlen(result),
                  0,(struct sockaddr *)&from,fromlen);
       if (n  < 0) error("sendto");
   }
 }

