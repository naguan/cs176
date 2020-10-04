/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
//Error checks of code taken from Oct 12 Section slides
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
int digits(int n) //checks for the number of digits in the string
{
    int num = 0;
    while(n > 0){
        num++;
        n= n/10;
    }
    return num;
}
int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
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
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     printf("String: %s\n",buffer);
     if (n < 0) error("ERROR writing to socket");
     
     
     
     int total = 0;
     for (int i = 0; i < n-1; i++) //adds the inital string
     {
        if(isalpha(buffer[i])){ //checks to see if letter
                n = write(newsockfd,"Sorry, cannot compute!\n", 35);
                close(newsockfd);
                close(sockfd);
                return 0;
            }else{
            total += buffer[i] - '0';//converts char to int
            }
    } 

    //send(sockfd, buffer, 256, 0);
    //recv(sockfd, buffer, 256, 0);
    //n = write(newsockfd, buffer, 256);
    sprintf(buffer,"%d", total);
    while(digits(total) > 1){ //checks to see if singular digit
         char str[digits(total)];
         sprintf(str,"%d", total);
         int holder = total;
         total = 0;
         for (int j = 0; j < digits(holder); j++){ //converts and adds the digits
             total += str[j] - '0';
         }
        sprintf(str,"%d", total);
    //    n= write(newsockfd, str, 256);
     //   send(sockfd, str, 256, 0);
     // recv(sockfd, buffer, 256, 0);
     }
     sprintf(buffer,"%d", total);
     //send(sockfd, buffer, 256, 0);
     //recv(sockfd, buffer, 256, 0);
     lsn = write(newsockfd,buffer,256);
     close(newsockfd);
     close(sockfd);
     return 0; 
}