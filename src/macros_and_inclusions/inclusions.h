
#ifndef __AQUA__SDL2_SRC_MACROS_AND_INCLUSIONS_INCLUSIONS_H
	#define __AQUA__SDL2_SRC_MACROS_AND_INCLUSIONS_INCLUSIONS_H
	
	#include <stdint.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	
	#if KOS_USES_SDL2
		#include <SDL2/SDL.h>
		#include <SDL2/SDL_opengl.h>
	#endif
	
	#if KOS_USES_OPENGL_DESKTOP
		#include <GL/gl.h>
		#include <GL/glu.h>
	#endif
	
	#include "../lib/structs.h"
	#include "../lib/macros.h"
	
#endif
