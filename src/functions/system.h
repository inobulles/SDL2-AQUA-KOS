
#ifndef __AQUA__SDL2_SRC_FUNCTIONS_SYSTEM_H
	#define __AQUA__SDL2_SRC_FUNCTIONS_SYSTEM_H
	
	#include "../macros_and_inclusions.h"
	
	unsigned long long get_platform(void) {
		#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
			return PLATFORM_WIN32;
		#elif defined(_WIN64)
			return PLATFORM_WIN64;
		#elif defined(__CYGWIN__) && !defined(_WIN32)
			return PLATFORM_UNIX;
		#elif defined(__APPLE__) && defined(__MACH__)
			printf("WARNING Apple based platform detected, consider changing your OS\n");
			
			#if TARGET_IPHONE_SIMULATOR == 1 || TARGET_OS_IPHONE == 1
				return PLATFORM_IOS;
			#elif TARGET_OS_MAC == 1
				return PLATFORM_MACOS;
			#else
				return PLATFORM_CANCER;
			#endif
		#elif defined(BSD)
			printf("WARNING BSD based system detected. Returning Unix, as BSD is not included in the AQUA system standard\n");
			return PLATFORM_UNIX;
		#elif defined(__linux__)
			return PLATFORM_LINUX;
		#elif defined(__unix__)
			return PLATFORM_UNIX;
		#else
			return PLATFORM_OTHER;
		#endif
		
	}
	
	unsigned long long platform_system(const char* command) {
		#if SYSTEM_ACCESS
			return system(command);
		#else
			printf("WARNING You do not seem to have the privileges (SYSTEM_ACCESS = %d) to run the command `%s`\n", SYSTEM_ACCESS, command);
			return 1;
		#endif
		
	}
	
	void platform(void) {
		KOS_TODO
		
	}
	
	unsigned long long is_device_supported(const char* device) {
		if      (strcmp(device, "texture")  == 0) return DEVICE_TEXTURE;
		else if (strcmp(device, "keyboard") == 0) return DEVICE_KEYBOARD;
		return                                           DEVICE_NULL;
		
	}
	
	unsigned long long* get_device(unsigned long long device, const char* extra) {
		KOS_TODO
		return NULL;
		
	}
	
	void send_device(unsigned long long device, const char* extra, unsigned long long* data) {
		switch (device) {
			case DEVICE_TEXTURE: {
				if (strcmp(extra, "sharp") == 0) {
					SHARP_TEXTURES = *data;
					
				} else {
					printf("WARNING The command you have passed to the texture device (%s) is unrecognized\n");
					
				}
				
				break;
				
			} case DEVICE_NULL: {
				printf("WARNING The device you have selected is DEVICE_NULL\n");
				break;
				
			} default: {
				printf("WARNING Device %lld does not seem to exist or doesn't seem to accept sent commands\n", device);
				break;
				
			}
			
		}
		
	}
	
#endif
