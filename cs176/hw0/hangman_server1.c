#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>  
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <fcntl.h> //open
#include <unistd.h> //write and close
#include <time.h> //rand

#define NUM_THREADS 5

void *pthrdid (void *id);
void guess(char guess, char w[], char trivia[]);
int winCondition(char trivia[]);
char readFileRandWord();

int n_threads;

int main(int argc, char* argv[]){
    int socket_desc , client_sock , c, read_size, portNum;
    struct sockaddr_in serv_addr, cli_addr;
    char client_message[2000];
    int i=0;
    n_threads= 0;
    int id,rid;
    pthread_t thread[NUM_THREADS];
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc < 0){
        printf("Could not create socket\n");
    }
    printf("Socket Created\n");
    portNum = atoi(argv[2]);
    //Prepare the sockaddr_in structure
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(portNum);
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&serv_addr , sizeof(serv_addr)) < 0){
        //print the error message
        printf("Bind failed. Error\n");
        return 1;
    }
    printf("Bind Done\n");
	int count=1;
	while(count>0){
	    //Listen
		listen(socket_desc , 1);
		//Accept incoming connection
		if(n_threads <= 0){
		    printf("Waiting for incoming connections...\n");
		}
		c = sizeof(struct sockaddr_in);
        //accept connection from an incoming client
		if(n_threads <3){
		    client_sock = accept(socket_desc, (struct sockaddr *)&cli_addr, (socklen_t*)&c);
			if (client_sock < 0){
				printf("Accept Failed\n");
				return 0;
			}
			printf("Connection Accepted\n");//
            printf("seg");
            strcpy(client_message,"Ready to start game? (y/n):");
			//Send the message back to client
			write(client_sock , client_message, strlen(client_message));
	     	n_threads++;
			rid = pthread_create(&thread[id], NULL, &pthrdid, (void*)(size_t)client_sock);
			if(rid!=0){
			    printf("pthread_create() failed for Thread # %d",id);
				return 0;
			}
            if(n_threads <= 0){
			    pthread_exit(NULL);
				return 0;
				}
			}else{
				client_sock = accept(socket_desc, (struct sockaddr *)&cli_addr, (socklen_t*)&c);
			    if (client_sock < 0){
				    printf("Accept Failed\n");
				    return 0;
			    }
				printf("Request denied. Server full\n");
				strcpy(client_message,"Server overloaded\n");
					//Send the message back to client
					write(client_sock , client_message, strlen(client_message));
			}
	}
    return 0;
}

void *pthrdid (void *id){
    int client_sock = (int)id;
	int read_size =0;
    char client_msg[2000],  word[2000];
    char trivia[2000], *fromList;
    memset(trivia, '\0', sizeof(trivia));
    memset(client_msg, '\0', sizeof(client_msg));
    fromList = "cards";//readFileRandWord;
    strcpy(word, fromList);
    for(int i=0;i<strlen(word);i++)
    {
        trivia[i] = '_';
    }
    write(client_sock , trivia, strlen(trivia));
    int end = 0;
    printf("Game Start Client %i\n",n_threads);
    while(end == 0){
      	if((read_size = recv(client_sock , client_msg, 2000 , 0))> 0){
            guess(client_msg[0], word, trivia);
            if(strcmp(client_msg,"end")==0){
                write(client_sock , word, strlen(word));
                n_threads--;
                pthread_exit(NULL);
            }
            if(winCondition(trivia)){
                end=1;
            }
            //Send the message back to client
            write(client_sock , trivia, strlen(trivia));
      	}else if(read_size == 0){
      			printf("Client disconnected \n");
      		    }else{
      			perror("Recv Failed\n");
      		    }
      	   }
	n_threads--;
	pthread_exit(NULL);
}

int winCondition(char trivia[]){
    int check=1;
    for(int i=0;i<strlen(trivia);i++){
        if(trivia[i] == '_')
            check=0;
    }
    return check;
}

char readFileRandWord(){
    FILE* fptr; 
    char c[10];   
    srand(time(NULL));
    int randNum = rand() % 15;
    int iterator = 0;
    // Open file 
    fptr = fopen("hangman_text.txt", "r"); 
    if (fptr == NULL) 
    { 
        printf("Cannot open file \n"); 
        exit(0); 
    } 
    // Read contents from file 
    while ( fgets(c, sizeof(c), fptr) )
    {   
        printf("%d\n", iterator);
        printf("%d\n", randNum);
        if(iterator == randNum){
            fclose(fptr); 
            return(*c); 
        }else{
            iterator++;
        }
        
    }    
}

void guess(char guess, char w[], char trivia[]){   
    int check=0;
    for(int i=0;i<strlen(w);i++){
        if(w[i] == guess){
            trivia[i] = w[i];
        }
    }
}