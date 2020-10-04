/* Creates a datagram server.  The port 
   number is passed as an argument.  This
   server runs forever */
//Part of code taken from Oct 12 Section slides
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
   char buf[1024];

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
       n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
       if (n < 0) error("recvfrom");
       write(1,"Received a datagram: ",21);
       write(1,buf,n);
       n = sendto(sock,"Got your message\n",17,
                  0,(struct sockaddr *)&from,fromlen);
       if (n  < 0) error("sendto");
   }
   //int total = 0;
   //  for (int i = 0; i < n-1; i++) //adds the inital string
   //  {
   //     if(isalpha(buffer[i])){ //checks to see if letter
   //             n = write(newsockfd,"Sorry, cannot compute!\n", 35);
//            close(newsockfd);
//                close(sockfd);
//                return 0;
//            }else{
//            total += buffer[i] - '0';//converts char to int
//            }
//    } 

    //send(sockfd, buffer, 256, 0);
    //recv(sockfd, buffer, 256, 0);
    //n = write(newsockfd, buffer, 256);
//    sprintf(buffer,"%d", total);
//    while(digits(total) > 1){ //checks to see if singular digit
//         char str[digits(total)];
//         sprintf(str,"%d", total);
//         int holder = total;
//         total = 0;
//         for (int j = 0; j < digits(holder); j++){ //converts and adds the digits
//            total += str[j] - '0';
//         }
//        sprintf(str,"%d", total);
    //    n= write(newsockfd, str, 256);
     //   send(sockfd, str, 256, 0);
     // recv(sockfd, buffer, 256, 0);
//     }
//     sprintf(buffer,"%d", total);
     //send(sockfd, buffer, 256, 0);
     //recv(sockfd, buffer, 256, 0);
//     lsn = write(newsockfd,buffer,256);
//     close(newsockfd);
//     close(sockfd);
//     return 0; 
//   return 0;
 }

