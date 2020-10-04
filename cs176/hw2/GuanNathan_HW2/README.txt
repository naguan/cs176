How this program should work is when the user activates both the server and clie
nt, the user will be prompted to enter a string of numbers. If the string contai
ns anything other than numbers, it will end the session. If it is all numbers, t
hen the server will add all the digits together and return it to the client. If
the number is not just a digit, the digits will all be added together and sent t
o the client again. This will keep going until the output is just a singular dig
it.

For my TCP client/server duo, I was able to get the calculation to work but coul
d not get the back and forth communication.

The starter code is from the Oct 12 discussion slides.

Example UDP C Client:
http://www.linuxhowtos.org/data/6/client_udp.c
Example UDP C Server:
http://www.linuxhowtos.org/data/6/server_udp.c
Example TCP C Client:
http://www.linuxhowtos.org/data/6/client.c
Example TCP C Server:
http://www.linuxhowtos.org/data/6/server.c
