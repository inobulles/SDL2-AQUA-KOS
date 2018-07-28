
#ifndef __AQUA__SDL2_SRC_KOS_H
	#define __AQUA__SDL2_SRC_KOS_H
	
	#include <SDL2/SDL.h>
	#include "macros_and_inclusions.h"
	
	typedef struct {
		int width;
		int height;
		
		SDL_Window* window;
		SDL_Surface* screen;
		
	} kos_t;
	
	int kos_init(kos_t* this) {
		this->width = KOS_ORIGINAL_WIDTH;
		this->height = KOS_ORIGINAL_HEIGHT;
		
		this->window = NULL;
		this->screen = NULL;
		
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			printf("ERROR SDL2 could not initizalize (%s)\n", SDL_GetError());
			return 1;
			
		}
		
		this->window = SDL_CreateWindow("AQUA 3.X SDL2 KOS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->width, this->height, SDL_WINDOW_SHOWN);
		
		if (this->window == NULL) {
			printf("ERROR SDL2 window could not be created (%s)\n", SDL_GetError());
			return 1;
			
		}
		
		return 0;
		
	}
	
#endif
