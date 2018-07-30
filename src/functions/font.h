
#ifndef __AQUA__SDL2_SRC_FUNCTIONS_FONT_H
	#define __AQUA__SDL2_SRC_FUNCTIONS_FONT_H
	
	#include "../macros_and_inclusions.h"
	
	font_t new_font(const char* path, unsigned long long size) { KOS_TODO }
	void font_remove(font_t this) { KOS_TODO }
	
	texture_t create_texture_from_font(font_t* this, const char* text) { KOS_TODO }
	
	unsigned long long get_font_width(font_t* this, const char* text) { KOS_TODO }
	unsigned long long get_font_height(font_t* this, const char* text) { KOS_TODO }
	
#endif
