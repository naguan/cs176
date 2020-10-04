//timeout code from http://www.mathcs.emory.edu/~cheung/Courses/455/Syllabus/9-netw-prog/timeout.html
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <ctype.h>
#include <poll.h>
void error(const char *);
int main(int argc, char *argv[])
{
   int sock, n;
   unsigned int length;
   struct sockaddr_in server, from;
   struct hostent *hp;
   int count = 0;
   int sent = 0;
   int received = 0;
   double timeDifferenceMilliSec, max, avg, min, RTT;
   char buffer[2500];
   char tempBuffer[2500];
   clock_t startTime, endTime;
 
   if (argc != 3) { 
      printf("Need host port\n");
      exit(1);
   }
   sock= socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) {
      error("socket");
  }
   server.sin_family = AF_INET;
   hp = gethostbyname(argv[1]);
   if (hp==0) {
      error("Unknown host");
  }
   bcopy((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length);
   server.sin_port = htons(atoi(argv[2]));
   length=sizeof(struct sockaddr_in);
    
   char temp[3];
   max = 0;
   min = 0;
   avg = 0;
   double total = 0;
   for(count = 1; count < 11; count++) { //count starts at 1 such that sequence goes from 1-10
      bzero(buffer, 2500);
      strcat(buffer, "PING");
      bzero(temp, 3);
      sprintf(temp, "%d", count);
      strcat(buffer, temp);
      strcat(buffer, " ");
      bzero(tempBuffer, 2500);
      sprintf(tempBuffer, "%2.13ld", startTime);
      strcat(buffer, tempBuffer);
      startTime = clock(); //set start time
      n = sendto(sock,buffer, strlen(buffer),0,(const struct sockaddr *)&server,length);
      if(n < 0){ //checks to see if message sent, if sent, increment sent +1, start timeout process
        error("Error: sendto");
      }
      sent++;
      struct pollfd fd[1];
      int timeout;
      fd[0].fd = sock;
      fd[0].events = 0;
      fd[0].events |= POLLIN;
      timeout = poll(fd, 1, 1000);
      if(timeout == 0){
        endTime = clock();// sets end time for if timed-out
        RTT = (endTime - startTime);
        timeDifferenceMilliSec = 1 + (RTT/1000.0);
        printf("PING %d timed out: %.3ld ms\n", count, endTime);
      }else{//not timed-out
        n = recvfrom(sock,buffer,2000,0,(struct sockaddr *)&from, &length);
        endTime = clock(); //end time for received
        if(n > 1){ //ping rcvd from server
          RTT = (endTime - startTime);
          timeDifferenceMilliSec = 1 + (RTT/1000.0);
          printf("PING received from %s: seq#=%d time=%.3f ms\n", argv[1], count, timeDifferenceMilliSec);
          received++;
        }
        RTT = timeDifferenceMilliSec;
        avg += RTT; //finds avg RTT
        if(RTT < min || min == 0) //checks for min RTT
            min = RTT;
        if(RTT > max || max == 0) //checks for max RTT
            max = RTT;
        
      }
      total++;
   } 
    avg = avg/received;
    printf("---ping statistics--- %d packets transmitted, %d received, %.3f percent packet lost rtt min/avg/max = %.3f %.3f %.3f ms\n",sent, received, ((total-received)/total)*100, min, avg, max);
    close(sock);
   
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
