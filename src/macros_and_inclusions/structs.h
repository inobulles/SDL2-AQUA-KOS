
#ifndef __AQUA__SDL2_SRC_MACROS_AND_INCLUSIONS_STRUCTS_H
	#define __AQUA__SDL2_SRC_MACROS_AND_INCLUSIONS_STRUCTS_H
	
	typedef struct {
		int warning_count;
		
		int width;
		int height;
		
		int bpp;
		
		SDL_Window*   window;
		SDL_GLContext context;
		
		GLuint shader_program;
		GLint  fbo;
		
	} kos_t;
	
#endif
