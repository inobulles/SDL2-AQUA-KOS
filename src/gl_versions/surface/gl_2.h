
#ifndef __AQUA__SDL2_SRC_KOS_GL_VERSIONS_SURFACE_GL_2_H
	#define __AQUA__SDL2_SRC_KOS_GL_VERSIONS_SURFACE_GL_2_H
	
	#include "../../macros_and_inclusions.h"
	
	int gl2_surface_draw(surface_t* this) {
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		
		if (this->has_texture) {
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			
		}
		
		glVertexPointer(3, GL_FLOAT, 0, this->vertices);
		glColorPointer( 4, GL_FLOAT, 0, this->colours);
		
		if (this->has_texture) {
			glTexCoordPointer(2, GL_FLOAT, 0, this->texture_coords);
			
		}
		
		glBindTexture( GL_TEXTURE_2D, (GLuint) this->texture);
		glDrawElements(GL_TRIANGLES, SURFACE_VERTEX_COUNT, GL_UNSIGNED_BYTE, this->faces);
		
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		
		if (this->has_texture) {
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			
		}
		
		return 0;
		
	}
	
#endif
