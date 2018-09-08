
#ifndef __AQUA__SDL2_SRC_FUNCTIONS_FS_H
	#define __AQUA__SDL2_SRC_FUNCTIONS_FS_H
	
	#include "../macros_and_inclusions.h"
	
	unsigned long long fs_support(void) {
		return 1;
		
	}
	
	#define FS_CHECK_FILE(action) \
		if (!file) { \
			printf("WARNING File `%s` could not be opened (for %s)\n", path, action); \
			return 1; \
		}
	
	unsigned long long fs_read(const char* path, char** data, unsigned long long* bytes) {
		FILE* file = fopen(path, "rb");
		FS_CHECK_FILE("reading")
		
		fseek(file, 0, SEEK_END);
		*bytes = ftell(file);
		rewind(file);
		
		*data = (char*) malloc(*bytes + 1);
		fread(*data, *bytes, sizeof(char), file);
		
		fclose(file);
		return 0;
		
	}
	
	void fs_free(char* data, unsigned long long bytes) {
		free(data);
		
	}
	
	unsigned long long fs_write(const char* path, const char* data, unsigned long long bytes) {
		FILE* file = fopen(path, "wb");
		FS_CHECK_FILE("writing")
		
		fwrite((const void*) data, sizeof(char), bytes, file);
		fclose(file);
		
		return 0;
		
	}
	
#endif
