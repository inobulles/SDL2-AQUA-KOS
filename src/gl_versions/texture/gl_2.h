
#ifndef __AQUA__SDL2_SRC_KOS_GL_VERSIONS_TEXTURE_GL_2_H
	#define __AQUA__SDL2_SRC_KOS_GL_VERSIONS_TEXTURE_GL_2_H
	
	#include "../../macros_and_inclusions.h"
	
	texture_t gl2_texture_create(unsigned long long* data, unsigned long long bpp, unsigned long long width, unsigned long long height) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		
		texture_t texture_id;
		glGenTextures(1, (GLuint*) &texture_id);
		
		glBindTexture(GL_TEXTURE_2D, (GLuint) texture_id);
		glTexImage2D(GL_TEXTURE_2D, 0, bpp == 24 ? GL_RGB : GL_RGBA, (GLuint) width, (GLuint) height, 0, bpp == 24 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TEXTURE_WRAP_TYPE); // x axis
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TEXTURE_WRAP_TYPE); // y axis
		
		#if KOS_SHARP_TEXTURES
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		#else
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		#endif
		
		return texture_id;
		
	}
	
	void gl2_texture_remove(texture_t this) {
		glDeleteTextures(1, (GLuint*) &this);
		
	}
	
#endif
