CC = gcc
ARGS = -Wall

all: udp_ping_client


client_c_udp: udp_ping_client.c
	$(CC) $(ARGS) -o udp_ping_client udp_ping_client.c


clean:
	rm -f *.o udp_ping_client *~
