
#include <stdio.h> 
#include <string.h>   
#include <sys/socket.h>    
#include <arpa/inet.h> 
#include <stdlib.h> 
#include <fcntl.h> 
#include <unistd.h> 

int guess(char g, char trivia[]);
int checkWin(char trivia[]);

int main(int argc, char* argv[]){
    int sock;
    struct sockaddr_in serv_addr;
    char message[1000] , server_reply[2000], incorrect_guesses[50], userGuess[50], trivia [50] ={'\0'};
    memset(incorrect_guesses, '\0', sizeof(incorrect_guesses));
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock < 0){
        printf("Could not create socket\n");
    }
    printf("Socket created\n");
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    if (connect(sock, (struct sockaddr *)&serv_addr , sizeof(serv_addr)) < 0){ //Connect to server
        printf("Connect failed. Error\n");
        return 1;
    }
    if(recv(sock , server_reply , 2000 , 0) < 0){
        puts("recv failed\n");
	}
	printf("%s\n",server_reply);
    if(strcmp(server_reply,"Server is full\n") == 0){
        close(sock);
        return 0;
    }
    fgets(server_reply, sizeof(server_reply), stdin);
    if (server_reply[strlen(server_reply) - 1] == '\n'){
        server_reply[strlen(server_reply) - 1] = '\0';
    }	
    if(strcmp(server_reply,"y") == 0){
    	int count = 7;
  		while(count > 0){
    		if( recv(sock , trivia , 2000 , 0) < 0){
        		puts("recv failed\n");
    	}
    	if(checkWin(trivia))
        	break;
		if(!guess(userGuess[0],trivia)){
    		strcat(incorrect_guesses,userGuess);
			count--;
		}
      	printf("%s\n",trivia);
  		printf("Incorrect Guesses: ");
      	printf("%s\n\n",incorrect_guesses);
      	printf("Letter to Guess: ");
  		fgets(userGuess, sizeof(server_reply), stdin);
      	if(userGuess[strlen(userGuess) - 1] == '\n'){
        	userGuess[strlen(userGuess) - 1] = '\0';
      	}
      	int wrong = 0;
      	while(wrong == 0){
        	if(strlen(userGuess)==1 && ((userGuess[0]>=65 && userGuess[0]<=90) || (userGuess[0]>=97 && userGuess[0]<=122)) ){
      			if( send(sock , userGuess , strlen(userGuess) , 0) < 0){
          			printf("Send failed\n");
          			return 1;
      			}
        	    wrong = 1;
    	    }else{
        	    printf("Error! Please guess one letter.\n");
        	    printf("Letter to Guess: ");
        	    fgets(userGuess, sizeof(userGuess), stdin);
        	    if(userGuess[strlen(userGuess) - 1] == '\n'){
                    userGuess[strlen(userGuess) - 1] = '\0';
        		}
    		}
    	}
		if(checkWin(trivia)){
    	    printf("The Word: %s\n",trivia);
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
        printf("Correct Word was: %s",userGuess);
        }
	}
	close(sock);
    return 0;
}

int guess(char g, char trivia[]){
  int check=0;
  for(int i=0;i<strlen(trivia);i++){
    if(trivia[i] == g)
      check=1;
  	}
    return check;
}

int checkWin(char trivia[]){
  	int check=1;
  	for(int i=0;i<strlen(trivia);i++){
    	if(trivia[i] == '_')
			check=0;
  	}
    return check;
}