
#ifndef __AQUA__SDL2_SRC_FUNCTIONS_FS_LIST_H
	#define __AQUA__SDL2_SRC_FUNCTIONS_FS_LIST_H
	
	#include <dirent.h>
	#define FS_LIST_FUNCTIONS 1
	
	#define FS_LIST_D_NAME_VALID (strcmp(directory->d_name, "..") != 0 && strcmp(directory->d_name, ".") != 0)
	
	unsigned long long fs_list_count(const char* path) {
		unsigned long long count = 0;
		
		DIR* dp = opendir(path);
		struct dirent* directory;
		
		if (dp) {
			while ((directory = readdir(dp)) != NULL) {
				count += FS_LIST_D_NAME_VALID;
				
			}
			
			closedir(dp);
			return count;
			
		} else {
			printf("WARNING Directory `%s` could not be opened (for list counting)\n", path);
			return -1;
			
		}
		
	}
	
	char** fs_list(const char* path) {
		unsigned long long count   = fs_list_count(path);
		unsigned long long current = 0;
		char** result              = (char**) malloc(count * sizeof(char*));
		
		DIR* dp = opendir(path);
		struct dirent* directory;
		
		if (dp) {
			while ((directory = readdir(dp)) != NULL) {
				if (FS_LIST_D_NAME_VALID) {
					unsigned long long bytes = strlen(directory->d_name) + 1;
					result[current] = (char*) malloc(bytes * sizeof(char));
					memcpy(result[current++], directory->d_name, bytes);
					
				}
				
			}
			
			closedir(dp);
			return result;
			
		} else {
			printf("WARNING Directory `%s` could not be opened (for listing)\n", path);
			return (char**) 0;
			
		}
		
	}
	
	void fs_list_free(char** path) {
		KOS_TODO;
		
	}
	
#endif
