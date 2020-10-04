#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>

int main(){
    FILE *fptr; 
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
            printf(c);
            break; 
        }else{
            printf(c);
            iterator++;
        }
        
    } 
    fclose(fptr); 
    return 0; 
}