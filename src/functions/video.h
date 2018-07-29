
#ifndef __AQUA__SDL2_SRC_FUNCTIONS_VIDEO_H
	#define __AQUA__SDL2_SRC_FUNCTIONS_VIDEO_H
	
	#include "../macros_and_inclusions.h"
	
	void video_clear(void) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	}
	
	void video_clear_colour(unsigned long long r, unsigned long long g, unsigned long long b, unsigned long long a) {
		glClearColor((float) r / _UI64_MAX, (float) g / _UI64_MAX, (float) b / _UI64_MAX, (float) a / _UI64_MAX);
		
	}
	
	void video_draw(void) {
		printf("WARNING This function (`video_draw`) is deprecated\n");
		
	}
	
	void video_flip(void) {
		SDL_GL_SwapWindow(current_kos->window);
		
	}
	
	unsigned long long video_width(void) { return current_kos->width; }
	unsigned long long video_height(void) { return current_kos->height; }
	
	static unsigned long long kos_last_time;
	
	unsigned long long video_fps(void) {
		unsigned long long tick_time = SDL_GetTicks();
		float fps = 1000.0f / (float) (tick_time - kos_last_time);
		kos_last_time = tick_time;
		
		return (unsigned long long) fps;
		
	}
	
#endif
