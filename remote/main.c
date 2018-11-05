
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
	
	this->ip_address = (char*) malloc(64 * sizeof(char));
	
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
	
	#define LENGTH 32768
	char* command_output = (char*) malloc(LENGTH * sizeof(char));
	
	while (1) {
		char* command = socket_receive(&this->socket, LENGTH);
		printf("Executing \"%s\" ...\n", command);
		
		if (strcmp(command, "exit") == 0) {
			break;
			
		}
		
		FILE* file = popen(command, "r");
		
		if (file == NULL) {
			const char* warning_message = "WARNING Failed to execute command\n";
			
			printf              ("%s", warning_message);
			socket_send(&this->socket, warning_message, LENGTH);
			
		} else {
			fseek(file, 0, SEEK_END);
			unsigned long long bytes = ftell(file);
			rewind(file);
			
			printf("Sending %llu bytes of command output ...\n", bytes);
			
			fread(command_output, sizeof(char), bytes, file);
			printf("%s", command_output);
			
			socket_send(&this->socket, command_output, LENGTH);
			pclose(file);
			
		}
		
	}
	
	printf("Freeing command output buffer ...\n");
	free(command_output);
	
	quit(this);
	return 0;
	
}
