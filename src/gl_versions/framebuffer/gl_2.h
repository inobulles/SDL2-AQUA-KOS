
#ifndef __AQUA__SDL2_SRC_KOS_GL_VERSIONS_FRAMEBUFFER_GL_2_H
	#define __AQUA__SDL2_SRC_KOS_GL_VERSIONS_FRAMEBUFFER_GL_2_H
	
	#include "../../macros_and_inclusions.h"
	#include "../../gl_common/texture.h"
	
	framebuffer_t gl2_framebuffer_create(texture_t texture) {
		GLuint framebuffer_id = 0;
		glGenFramebuffers(1, &framebuffer_id);
		
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
		
		GLuint depth_renderbuffer;
		glGenRenderbuffers(1, &depth_renderbuffer);
		
		glBindRenderbuffer   (GL_RENDERBUFFER, depth_renderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, video_width(), video_height());
		
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  GL_RENDERBUFFER, depth_renderbuffer);
		glFramebufferTexture     (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
		
		GLenum draw_buffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, draw_buffers);
		
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			printf("WARNING Failed to create framebuffer\n");
			return (framebuffer_t*) 0;
			
		}
		
		return (framebuffer_t*) framebuffer_id;
		
	}
	
	void gl2_framebuffer_bind(framebuffer_t this) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
	}
	
	void gl2_framebuffer_remove(framebuffer_t this) {
		glDeleteFramebuffers(1, &this);
		
	}
	
#endif
