
#ifndef __AQUA__SDL2_SRC_FUNCTIONS_FS_LIST_H
	#define __AQUA__SDL2_SRC_FUNCTIONS_FS_LIST_H
	
	#include <dirent.h>
	#define FS_LIST_FUNCTIONS 1
	
	unsigned long long fs_list_count(const char* path) {
		unsigned long long count = 0;
		
		DIR* dp = opendir(path);
		struct dirent* directory;
		
		if (dp) {
			while ((directory = readdir(dp)) != NULL) {
				count++;
				
			}
			
			closedir(dp);
			return count;
			
		} else {
			printf("WARNING Directory `%s` could not be opened (for list counting)\n", path);
			return -1;
			
		}
		
	}
	
	char** fs_list(const char* path) {
		KOS_TODO;
		return (char**) 0;
		
	}
	
	void fs_list_free(char** path) {
		KOS_TODO;
		
	}
	
#endif
