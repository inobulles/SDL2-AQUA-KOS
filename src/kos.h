
#ifndef __AQUA__SDL2_SRC_KOS_H
	#define __AQUA__SDL2_SRC_KOS_H
	
	#include "macros_and_inclusions.h"
	
	#include "gl_common/surface.h"
	#include "gl_common/shaders.h"
	#include "gl_common/texture.h"
	
	static const char* temp_gl_2_vertex_shader   = "#version 120\n\nvoid main(void) {\n\tgl_Position  = vec4(0, 0, 0, 0);\n\t\n}\n";
	static const char* temp_gl_2_fragment_shader = "#version 120\n\nvoid main(void) {\n\tgl_FragColor = vec4(1, 0, 0, 1);\n\t\n}\n";
	
	void kos_quit(kos_t* this) {
		SDL_GL_DeleteContext(this->context);
		SDL_DestroyWindow(this->window);
		SDL_Quit();
		
	}
	
	int kos_init(kos_t* this) {
		current_kos  = this;
		this->window = NULL;
		
		this->width  = KOS_ORIGINAL_WIDTH;
		this->height = KOS_ORIGINAL_HEIGHT;
		
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			printf("ERROR SDL2 could not initizalize (%s)\n", SDL_GetError());
			KOS_ERROR
			
		}
		
		this->window = SDL_CreateWindow("AQUA 3.X SDL2 KOS", \
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, \
			this->width, this->height, \
			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
		
		if (this->window == NULL) {
			printf("ERROR SDL2 window could not be created (%s)\n", SDL_GetError());
			KOS_ERROR
			
		}
		
		this->context = SDL_GL_CreateContext(this->window);
		const char* error_message = SDL_GetError();
		
		if (*error_message != '\0') {
			printf("ERROR SDL2 GL context could not be created (%s)\n", error_message);
			KOS_ERROR
			
		}
		
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE,      5);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,    6);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,     5);
		
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,   16);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,  1);
		
		printf("OpenGL info\n");
		printf("\tVendor:                   %s\n", glGetString(GL_VENDOR));
		printf("\tRenderer:                 %s\n", glGetString(GL_RENDERER));
		printf("\tVersion:                  %s\n", glGetString(GL_VERSION));
		printf("\tShading language version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
		
		KOS_BEST_GL_VERSION
		printf("Using OpenGL version %d.%d\n", kos_best_gl_version_major, kos_best_gl_version_minor);
		
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, kos_best_gl_version_major);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, kos_best_gl_version_minor);
		
		if (SDL_GL_SetSwapInterval(1) < 0) {
			printf("WARNING Failed to enable VSync (this may cause problems down the line)\n");
			this->warning_count++;
			
		}
		
		if (gl_load_shaders(&this->shader_program, (char*) temp_gl_2_vertex_shader, (char*) temp_gl_2_fragment_shader)) {
			printf("ERROR Failed to create GL shader program\n");
			KOS_ERROR
			
		}
		
		//~ glUseProgram(this->shader_program);
		
		// testing stuff
		
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		surface_t surface;
		surface_new(&surface, 0, 0, _UI64_MAX_MARGIN / 5, _UI64_MAX_MARGIN / 5);
		
		surface_draw(&surface);
		SDL_GL_SwapWindow(this->window);
		
		SDL_Delay(5000);
		
		return 0;
		
	}
	
	#include "functions/video.h"
	#include "functions/system.h"
	#include "functions/fs.h"
	
#endif
