
#ifndef __AQUA__KOS_FUNCTIONS_REQUESTIONS_H
	#define __AQUA__KOS_FUNCTIONS_REQUESTIONS_H
	
	int asprintf(char **strp, const char *fmt, ...);
	
	#include "../librequests/include/requests.h"
	#include "../librequests/src/requests.c"
	
	static req_t         kos_current_request;
	static CURL*         kos_curl;
	static unsigned char kos_requests_init = 0;
	
	typedef struct {
		unsigned long long code;
		
		#define                MAX_URL_LENGTH_BYTES 4096
		unsigned long long url[MAX_URL_LENGTH_BYTES / sizeof(unsigned long long)];
		
		unsigned long long text_bytes;
		unsigned long long text;
		
	} kos_request_response_t;
	
	static void request_global_init(void) {
		kos_curl = requests_init(&kos_current_request);
		
	}
	
	void kos_requests_get(kos_request_response_t* this, const char* url) {
		if (!kos_requests_init) {
			kos_requests_init++;
			request_global_init();
			
		}
		
		requests_get(kos_curl, &kos_current_request, (char*) url);
		this->code = kos_current_request.code;
		
		unsigned long long url_bytes = strlen(kos_current_request.url);
		if (url_bytes >= MAX_URL_LENGTH_BYTES) {
			printf("WARNING Request URL is too long (%lld bytes, %s)\n", url_bytes, kos_current_request.url);
			strcpy((char*) this->url, "__URL_TOO_LONG__");
			
		} else {
			strcpy((char*) this->url, kos_current_request.url);
			
		}
		
		this->text_bytes = strlen(kos_current_request.text) + 1;
		this->text       = (unsigned long long) malloc(this->text_bytes);
		
		strcpy((char*) this->text, kos_current_request.text);
		requests_close(&kos_current_request);
		
	}
	
	void kos_requests_free(kos_request_response_t* this) {
		if (this->text) {
			free((void*) this->text /* this->text_bytes */);
			
		}
		
	}
	
#endif
