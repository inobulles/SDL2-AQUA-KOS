
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
		else if (strcmp(device, "fbo")      == 0) return DEVICE_FBO;
		else if (strcmp(device, "shader")   == 0) return DEVICE_SHADER;
		else if (strcmp(device, "gl")       == 0) return DEVICE_GL;
		
		// compute
		
		else if (strcmp(device, "nvcc") == 0 && !system("command -v nvcc")) return DEVICE_COMPUTE_CUDA_COMPILER;
		else if (strcmp(device, "cuda") == 0)                               return DEVICE_COMPUTE_CUDA_EXECUTOR;
		
		// extensions
		
		#ifdef __HAS_CURL
			else if (strcmp(device, "requests") == 0) return DEVICE_REQUESTS;
		#endif
		#ifdef __HAS_DISCORD
			else if (strcmp(device, "discord") == 0) return DEVICE_DISCORD;
		#endif
		
		else return DEVICE_NULL;
		
	}
	
	static unsigned long long get_device_keyboard_key     = 0;
	static unsigned long long get_device_keyboard_keycode = 0;
	
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
	
	static struct tm* kos_tm_struct = (struct tm*) 0;
	static time_t     kos_time      = 0;
	
	typedef struct {
		char signature[sizeof(uint64_t)];
		uint64_t x;
		
	} math_device_generic_t;
	
	typedef struct {
		time_device_t previous_time_device;
		
		unsigned long long previous_fbo_device_create_result;
		unsigned long long previous_shader_device_create_result;
		
		unsigned long long previous_math_device_sqrt_result;
		unsigned long long previous_math_device_sin_result;
		
		unsigned long long get_device_keyboard_key_packet;
		unsigned long long get_device_keyboard_keycode_packet;
		
	} kos_bda_extension_t;

	#define KOS_BDA_EXTENSION
	kos_bda_extension_t kos_bda_implementation;

	#define FLOAT_ONE 1000000
	
	#include <math.h>
	#include <time.h>
	
	#include "compute/cuda.h"
	
	#ifdef __HAS_CURL
		#include "requests.h"
	#endif
	#ifdef __HAS_DISCORD
		#include "discord.h"
	#endif
	
	unsigned long long* get_device(unsigned long long device, const char* extra) {
		unsigned long long* result = (unsigned long long*) 0;
		
		switch (device) {
			case DEVICE_MATH: {
				if (strcmp(extra, "sqrt") == 0) {
					math_device_generic_t* data = (math_device_generic_t*) extra;
					kos_bda_implementation.previous_math_device_sqrt_result = (unsigned long long) (sqrt((double) data->x / FLOAT_ONE) * FLOAT_ONE);
					result = &kos_bda_implementation.previous_math_device_sqrt_result;
					
				} else if (strcmp(extra, "sin") == 0) {
					math_device_generic_t* data = (math_device_generic_t*) extra;
					kos_bda_implementation.previous_math_device_sin_result = (unsigned long long) (sin((double) data->x / FLOAT_ONE) * FLOAT_ONE);
					result = &kos_bda_implementation.previous_math_device_sin_result;
					
				} else {
					KOS_DEVICE_COMMAND_WARNING("math")
					
				}
				
				break;
				
			} case DEVICE_KEYBOARD: {
				if (strcmp(extra, "press scancode") == 0) {
					kos_bda_implementation.get_device_keyboard_key_packet =                    get_device_keyboard_key;
					get_device_keyboard_key                          = 0;
					result                                           = &kos_bda_implementation.get_device_keyboard_key_packet;
					
				} else if (strcmp(extra, "press key") == 0) {
					kos_bda_implementation.get_device_keyboard_keycode_packet =                    get_device_keyboard_keycode;
					get_device_keyboard_keycode                          = 0;
					result                                               = &kos_bda_implementation.get_device_keyboard_keycode_packet;
					
				} else {
					KOS_DEVICE_COMMAND_WARNING("keyboard")
					
				}
				
				break;
				
			} case DEVICE_CLOCK: {
				kos_time = time(NULL);
				kos_tm_struct = localtime(&kos_time);
				
				if (strcmp(extra, "current") == 0) {
					kos_bda_implementation.previous_time_device.hour     = (uint64_t) kos_tm_struct->tm_hour;
					kos_bda_implementation.previous_time_device.minute   = (uint64_t) kos_tm_struct->tm_min;
					kos_bda_implementation.previous_time_device.second   = (uint64_t) kos_tm_struct->tm_sec;
					
					kos_bda_implementation.previous_time_device.day      = (uint64_t) kos_tm_struct->tm_mday;
					kos_bda_implementation.previous_time_device.month    = (uint64_t) kos_tm_struct->tm_mon;
					kos_bda_implementation.previous_time_device.year     = (uint64_t) kos_tm_struct->tm_year;
					
					kos_bda_implementation.previous_time_device.week_day = (uint64_t) kos_tm_struct->tm_wday;
					kos_bda_implementation.previous_time_device.year_day = (uint64_t) kos_tm_struct->tm_yday;
					
					result = (unsigned long long*) &kos_bda_implementation.previous_time_device;
					
				} else {
					KOS_DEVICE_COMMAND_WARNING("clock")
					
				}
				
				break;
				
			}
			
			case DEVICE_COMPUTE_CUDA_COMPILER: cuda_compile_bytecode(&result, extra); break;
			case DEVICE_COMPUTE_CUDA_EXECUTOR: cuda_execute_bytecode(&result, extra); break;
			
			case DEVICE_FBO: {
				const unsigned long long* fbo_command = (const unsigned long long*) extra;
				
				if (fbo_command[0] == 'c') { // create
					kos_bda_implementation.previous_fbo_device_create_result = framebuffer_create(fbo_command[1]);
					result = (unsigned long long*) &kos_bda_implementation.previous_fbo_device_create_result;
					
				} else if (fbo_command[0] == 'b') {
					framebuffer_bind(fbo_command[1], fbo_command[4], fbo_command[5], fbo_command[2], fbo_command[3]);
					result = (unsigned long long*) 0;
					
				} else if (fbo_command[0] == 'r') { // remove
					framebuffer_remove(fbo_command[1]);
					result = (unsigned long long*) 0;
					
				} else {
					KOS_DEVICE_COMMAND_WARNING("fbo");
					
				}
				
				break;
				
			} case DEVICE_SHADER: {
				const unsigned long long* shader_command = (const unsigned long long*) extra;
				
				if (shader_command[0] == 'c') { // create
					kos_bda_implementation.previous_shader_device_create_result = gl_load_shaders((GLuint*) shader_command[1], (char*) shader_command[3], (char*) shader_command[4]);
					result = (unsigned long long*) &kos_bda_implementation.previous_shader_device_create_result;
					
				} else if (shader_command[0] == 'u') { // use
					gl_use_shader_program((GLuint*) shader_command[1]);
					
				} else if (shader_command[0] == 'd') { // dispose
					gl_delete_shader_program((GLuint*) shader_command[1]);
					
				} else {
					KOS_DEVICE_COMMAND_WARNING("shader");
					
				}
				
				break;
				
			} case DEVICE_GL: {
				const signed long long* gl_command = (const signed long long*) extra;
				
				if (gl_command[0] == 'f') { // glFrustum
					glMatrixMode(GL_PROJECTION);
					glLoadIdentity();
					
					glFrustum( \
						(double) gl_command[1] / FLOAT_ONE, \
						(double) gl_command[2] / FLOAT_ONE, \
						(double) gl_command[3] / FLOAT_ONE, \
						(double) gl_command[4] / FLOAT_ONE, \
						(double) gl_command[5] / FLOAT_ONE, \
						(double) gl_command[6] / FLOAT_ONE  \
					);
					
					glTranslatef(0.0f, 0.0f, -3.0f);
					
					glMatrixMode(GL_MODELVIEW);
					glLoadIdentity();
					
				} else if (gl_command[0] == 'o') { // glOrtho
					glMatrixMode(GL_PROJECTION);
					glLoadIdentity();
					
					glEnable(GL_CULL_FACE);
					glCullFace(GL_BACK);
					glFrontFace(GL_CCW);
					
					glOrtho( \
						(double) gl_command[1] / FLOAT_ONE, \
						(double) gl_command[2] / FLOAT_ONE, \
						(double) gl_command[3] / FLOAT_ONE, \
						(double) gl_command[4] / FLOAT_ONE, \
						(double) gl_command[5] / FLOAT_ONE, \
						(double) gl_command[6] / FLOAT_ONE  \
					);
					
					glTranslatef(0.0f, 0.0f, -100.0f);
					
					glMatrixMode(GL_MODELVIEW);
					glLoadIdentity();
					
				} else if (gl_command[0] == 'v') { // draw vertices only
					typedef struct {
						signed long long x;
						signed long long y;
						signed long long z;
						
					} vertex_t;
					
					surface_t test;
					surface_new(&test, -_UI64_MAX_MARGIN, -_UI64_MAX_MARGIN, 0, 0);
					surface_set_texture(&test, 0);
					surface_draw(&test);
					
					glColor3f(1.0f, 0.0f, 0.0f);
					glPointSize(2.0f);
					glBegin(GL_POINTS);
					
					unsigned long long i;
					for (i = 0; i < gl_command[12]; i++) {
						vertex_t* vertices = (vertex_t*) gl_command[11];
						glVertex3f((double) vertices[i].x / FLOAT_ONE, (double) vertices[i].y / FLOAT_ONE, (double) vertices[i].z / FLOAT_ONE);
						
					}
					
					glEnd();
					
					
				} else {
					KOS_DEVICE_COMMAND_WARNING("gl");
					
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
	
	#ifdef __HAS_CURL
		typedef struct {
			kos_request_response_t request_response;
			unsigned long long     pointer_to_const_url;
			
		} request_device_struct_t;
	#endif
	#ifdef __HAS_DISCORD
		typedef struct {
			kos_discord_rpc_t discord_rpc;
			
		} discord_device_struct_t;
	#endif
	
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
				
			}
			#ifdef __HAS_CURL
				case DEVICE_REQUESTS: {
					request_device_struct_t* request_device_struct = (request_device_struct_t*) data;
					
					if      (strcmp(extra, "get")  == 0) kos_requests_get (&request_device_struct->request_response, (const char*) request_device_struct->pointer_to_const_url);
					else if (strcmp(extra, "free") == 0) kos_requests_free(&request_device_struct->request_response);
					else KOS_DEVICE_COMMAND_WARNING("requests")
					
					break;
					
				}
			#endif
			#ifdef __HAS_DISCORD
				case DEVICE_DISCORD: {
					discord_device_struct_t* discord_device_struct = (discord_device_struct_t*) data;
					
					if      (strcmp(extra, "rpc init")    == 0) init_discord_rpc  ((unsigned long long) data);
					else if (strcmp(extra, "rpc loop")    == 0) loop_discord_rpc  (&discord_device_struct->discord_rpc);
					else if (strcmp(extra, "rpc update")  == 0) update_discord_rpc(&discord_device_struct->discord_rpc);
					else if (strcmp(extra, "rpc dispose") == 0) dispose_discord_rpc();
					else KOS_DEVICE_COMMAND_WARNING("requests")
					
					break;
					
				}
			#endif
			case DEVICE_NULL: {
				printf("WARNING The device you have selected is DEVICE_NULL\n");
				break;
				
			} default: {
				printf("WARNING Device %lld does not seem to exist or doesn't accept `send` commands\n", device);
				break;
				
			}
			
		}
		
	}
	
#endif
