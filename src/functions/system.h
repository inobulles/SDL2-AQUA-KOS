
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
		else if (strcmp(device, "wm")       == 0) return DEVICE_WM;
		else if (strcmp(device, "math")     == 0) return DEVICE_MATH;
		else if (strcmp(device, "clock")    == 0) return DEVICE_CLOCK;
		else                                      return DEVICE_NULL;
		
	}
	
	static unsigned long long get_device_keyboard_key = 0;
	static unsigned long long get_device_keyboard_key_packet;
	
	#define KOS_DEVICE_COMMAND_WARNING(device_name) printf("WARNING The command you have passed to the " device_name " device (%s) is unrecognized\n", extra);
	
	typedef struct {
		uint64_t hour;
		uint64_t minute;
		uint64_t second;
		
		uint64_t day;
		uint64_t month;
		uint64_t year;
		
		uint64_t week_day;
		uint64_t year_day;
		
	} time_device_t;
	
	static struct tm*    kos_tm_struct = (struct tm*) 0;
	static time_t        kos_time      = 0;
	static time_device_t previous_time_device;
	
	typedef struct {
		char signature[sizeof(uint64_t)];
		uint64_t x;
		
	} math_device_sqrt_t;
	
	static unsigned long long previous_math_device_sqrt_result;
	#define FLOAT_ONE 1000000
	
	#include <math.h>
	#include <time.h>
	
	unsigned long long* get_device(unsigned long long device, const char* extra) {
		unsigned long long* result = (unsigned long long*) 0;
		
		switch (device) {
			case DEVICE_MATH: {
				if (strcmp(extra, "sqrt") == 0) {
					math_device_sqrt_t* data = (math_device_sqrt_t*) extra;
					previous_math_device_sqrt_result = (unsigned long long) (sqrt((double) data->x / FLOAT_ONE) * FLOAT_ONE);
					result = &previous_math_device_sqrt_result;
					
				} else {
					KOS_DEVICE_COMMAND_WARNING("math")
					
				}
				
				break;
				
			} case DEVICE_KEYBOARD: {
				if (strcmp(extra, "press scancode") == 0) {
					get_device_keyboard_key_packet =  get_device_keyboard_key;
					get_device_keyboard_key        = 0;
					result                         = &get_device_keyboard_key_packet;
					
				} else {
					KOS_DEVICE_COMMAND_WARNING("keyboard")
					
				}
				
				break;
				
			} case DEVICE_CLOCK: {
				if (!kos_tm_struct) {
					printf("KOS initializing the tm struct ...\n");
					kos_time = time(NULL);
					kos_tm_struct = localtime(&kos_time);
					
				}
				
				if (strcmp(extra, "current") == 0) {
					previous_time_device.hour     = (uint64_t) kos_tm_struct->tm_hour;
					previous_time_device.minute   = (uint64_t) kos_tm_struct->tm_min;
					previous_time_device.second   = (uint64_t) kos_tm_struct->tm_sec;
					
					previous_time_device.day      = (uint64_t) kos_tm_struct->tm_mday;
					previous_time_device.month    = (uint64_t) kos_tm_struct->tm_mon;
					previous_time_device.year     = (uint64_t) kos_tm_struct->tm_year;
					
					previous_time_device.week_day = (uint64_t) kos_tm_struct->tm_wday;
					previous_time_device.year_day = (uint64_t) kos_tm_struct->tm_yday;
					
					result = (unsigned long long*) &previous_time_device;
					
				} else {
					KOS_DEVICE_COMMAND_WARNING("clock")
					
				}
				
				break;
				
			} case DEVICE_NULL: {
				printf("WARNING The device you have selected is DEVICE_NULL\n");
				break;
				
			} default: {
				printf("WARNING Device %lld does not seem to exist or doesn't accept `get` commands\n", device);
				break;
				
			}
			
		}
		
		return result;
		
	}
	
	void send_device(unsigned long long device, const char* extra, unsigned long long* data) {
		switch (device) {
			case DEVICE_TEXTURE: {
				if (strcmp(extra, "sharp") == 0) SHARP_TEXTURES = *data;
				else KOS_DEVICE_COMMAND_WARNING("texture")
				
				break;
				
			} case DEVICE_WM: {
				if (strcmp(extra, "visible") == 0 && *data == HIDDEN) SDL_MinimizeWindow(current_kos->window);
				else KOS_DEVICE_COMMAND_WARNING("wm")
				
				break;
				
			} case DEVICE_NULL: {
				printf("WARNING The device you have selected is DEVICE_NULL\n");
				break;
				
			} default: {
				printf("WARNING Device %lld does not seem to exist or doesn't accept `send` commands\n", device);
				break;
				
			}
			
		}
		
	}
	
#endif
