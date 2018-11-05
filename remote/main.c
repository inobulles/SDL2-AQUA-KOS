
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../src/socket/socket.h"

#define PORT 0xFACE // PALM

typedef struct {
	char* ip_address;
	socket_t socket;
	
} global_t;

void quit(global_t* this) {
	printf("Freeing IP address ...\n");
	free(this->ip_address);
	
	if (!this->socket.error) {
		printf("Closing socket ...\n");
		socket_close(&this->socket);
		
	}
	
	printf("Done\n");
	
}

#define ERROR \
	quit(this); \
	return 1;

int main(void) {
	printf("Remote client started ...\n");
	
	global_t __this;
	global_t*  this = (global_t*) &__this;
	
	this->ip_address = malloc(64 * sizeof(char));
	
	printf("So, I know I said that before was your last chance, but technically you can still stop the madness here, but yeah. Paste the IP that has been given to you here: ");
	scanf("%s", this->ip_address);
	
	printf("Connecting to %s:%d ...\n",         this->ip_address, PORT);
	socket_client(&this->socket, (ip_address_t) this->ip_address, PORT);
	
	if (this->socket.error) {
		printf("ERROR Failed to connect to server (maybe you have provided an incorrect IP address or the server is down?)\n");
		ERROR
		
	}
	
	printf("Connected to server, saying hi ...\n");
	socket_send(&this->socket, "Howdy", SOCKET_DEFAULT_BUFFER_SIZE);
	
	quit(this);
	return 0;
	
}
