all: hangman_client hangman_server

hangman_client: hangmanPlayerClient.c
	gcc -o hangmanPlayerClient hangmanPlayerClient.c
hangman_server: hangmanServer.c
	gcc -pthread -o hangmanServer hangmanServer.c

clean:
	$(RM) hangman_client hangman_server