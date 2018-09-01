
#ifndef __AQUA__SDL2_SRC_FUNCTIONS_FS_H
	#define __AQUA__SDL2_SRC_FUNCTIONS_FS_H
	
	#include "../macros_and_inclusions.h"
	
	unsigned long long fs_support(void) {
		return 1;
		
	}
	
	void fs_read(const char* path, char** data, unsigned long long* bytes) { KOS_TODO }
	void fs_free(char** data, unsigned long long bytes) { KOS_TODO }
	
	unsigned long long fs_write(const char* path, const char* data, unsigned long long bytes) {
		FILE* file = fopen(path, "wb");
		fwrite((const void*) data, sizeof(char), bytes, file);
		fclose(file);
		return 0;
		
	}
	
#endif
