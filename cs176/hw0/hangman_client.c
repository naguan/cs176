
#include <stdio.h>
#include <string.h>  
#include <sys/socket.>
#include <arpa/inet.h> 
#include <stdlib>
#include <fcntl.h>
#include <unistd.h> 

int checkWin(char uinput[]){
  	int check=1;
  	for(int i=0;i<strlen(uinput);i++){
    	if(uinput[i] == '_')
			check=0;
  	}
    return check;
}
int guess(char g, char uinput[]){
  int check=0;
  for(int i=0;i<strlen(uinput);i++){
    if(uinput[i] == g)
      check=1;
  	}
    return check;
}

int main(int argc, char* argv[]){
    int sock;
    int wrong = 0;
    struct sockaddr_in serv_addr;
    char message[1000] , servReply[2000], incorrect[50], userGuess[50], uinput [50] ={'\0'};
    memset(incorrect, '\0', sizeof(incorrect));
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock < 0){
        printf("Could not create socket\n");
    }
    printf("Socket created\n");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    //Connect to remote server
    if (connect(sock, (struct sockaddr *)&serv_addr , sizeof(serv_addr)) < 0){
        printf("Connect failed. Error\n");
        return 1;
    }
    if(recv(sock , servReply , 2000 , 0) < 0){
        puts("recv failed\n");
	}
	printf("%s\n",servReply);
    if(strcmp(servReply,"Server is full\n") == 0){
        close(sock);
        return 0;
    }
    fgets(servReply, sizeof(servReply), stdin);
    if (servReply[strlen(servReply) - 1] == '\n'){
        servReply[strlen(servReply) - 1] = '\0';
    }	
    if(strcmp(servReply,"y") == 0){
    	int count = 7;
  		while(count > 0){
    		if( recv(sock , uinput , 2000 , 0) < 0){
        		puts("recv failed\n");
    	}
    	if(checkWin(uinput))
        	break;
		if(!guess(userGuess[0],uinput)){
    		strcat(incorrect,userGuess);
			count--;
		}
      	printf("%s\n",uinput);
  		printf("Incorrect Guesses: ");
      	printf("%s\n\n",incorrect);
      	printf("Letter to Guess: ");
  		fgets(userGuess, sizeof(servReply), stdin);
      	if(userGuess[strlen(userGuess) - 1] == '\n'){
        	userGuess[strlen(userGuess) - 1] = '\0';
      	}
      	while(wrong == 0){
        	if(strlen(userGuess)==1 && ((userGuess[0]<=90 && userGuess[0]>=65 ) || (userGuess[0]<=122 && userGuess[0]>=97)) ){
      			if( send(sock , userGuess , strlen(userGuess) , 0) < 0){
          			printf("Send failed\n");
          			return 1;
      			}
        	    wrong = 1;
    	    }else{
        	    printf("Please guess one letter.\n");
        	    printf("Letter to Guess: ");
        	    fgets(userGuess, sizeof(userGuess), stdin);
        	    if(userGuess[strlen(userGuess) - 1] == '\n'){
                    userGuess[strlen(userGuess) - 1] = '\0';
        		}
    		}
    	}
		if(checkWin(uinput)){
    	    printf("The Word: %s\n",uinput);
    	    printf("You Win!\nGame Over!\n");
    	}else{//==0
        	printf("You Lose :(\nGame Over!\n");
        	strcpy(userGuess,"end");
        	if( send(sock , userGuess , strlen(userGuess) , 0) < 0){
        		printf("Send failed\n");
            	return 1;
        	}
        	if( recv(sock , userGuess , 2000 , 0) < 0){
            	puts("recv failed\n");
            }
        }    
        printf("Correct Word: %s",userGuess);
        }
	}
	close(sock);
    return 0;
}

