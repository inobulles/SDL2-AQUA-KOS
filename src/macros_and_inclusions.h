
#ifndef __AQUA__SDL2_SRC_MACROS_AND_INCLUSIONS_H
	#define __AQUA__SDL2_SRC_MACROS_AND_INCLUSIONS_H
	
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_opengl.h>
	
	#include <GL/gl.h>
	#include <GL/glu.h>
	
	#include "lib/structs.h"
	#include "lib/macros.h"
	
	// structs
	
	typedef struct {
		int warning_count;
		
		int width;
		int height;
		
		SDL_Window*   window;
		SDL_GLContext context;
		
		GLuint shader_program;
		
	} kos_t;
	
	// global variables
	
	static kos_t* current_kos;
	
	static char kos_best_gl_version_major;
	static char kos_best_gl_version_minor;
	
	static const float vertex_matrix[] = {
		0.0f, 2.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		2.0f, 0.0f, 1.0f,
		2.0f, 2.0f, 1.0f,
	};
	
	static const float texture_coords[] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
	};
	
	// macros
	
	#ifndef SYSTEM_ACCESS
		#define SYSTEM_ACCESS 0
	#endif
	
	#ifndef KOS_TEXTURE_WARNING
		#define KOS_TEXTURE_WARNING 0
	#endif
	
	#ifndef SHARP_TEXTURES
		#define SHARP_TEXTURES 0
	#endif
	
	#if !defined(KOS_ORIGINAL_WIDTH) || !defined(KOS_ORIGINAL_HEIGHT)
		#define KOS_ORIGINAL_WIDTH 800
		#define KOS_ORIGINAL_HEIGHT 480
	#endif
	
	#ifndef KOS_EXIT
		#define KOS_ERROR { \
			kos_quit(this); \
			return 1; \
		}
	#endif
	
	#if !defined(KOS_LOWEST_GL_VERSION_MAJOR) || !defined(KOS_LOWEST_GL_VERSION_MINOR)
		#define KOS_LOWEST_GL_VERSION_MAJOR 2
		#define KOS_LOWEST_GL_VERSION_MINOR 1
	#endif
	
	#if !defined(KOS_HIGHEST_GL_VERSION_MAJOR) || !defined(KOS_HIGHEST_GL_VERSION_MINOR)
		#define KOS_HIGHEST_GL_VERSION_MAJOR 2
		#define KOS_HIGHEST_GL_VERSION_MINOR 1
	#endif
	
	#ifndef KOS_BEST_GL_VERSION
		#define KOS_BEST_GL_VERSION { \
			char major = glGetString(GL_VERSION)[0] - '0'; \
			char minor = glGetString(GL_VERSION)[2] - '0'; \
			\
			if (major < KOS_LOWEST_GL_VERSION_MAJOR || (major == KOS_LOWEST_GL_VERSION_MAJOR && minor < KOS_LOWEST_GL_VERSION_MINOR)) { \
				printf("ERROR You have an unsupported GL version (%d.%d, minimum %d.%d)\n", major, minor, KOS_LOWEST_GL_VERSION_MAJOR, KOS_LOWEST_GL_VERSION_MINOR); \
				kos_quit(this); \
				\
			} else { \
				kos_best_gl_version_major = KOS_HIGHEST_GL_VERSION_MAJOR; \
				kos_best_gl_version_minor = KOS_HIGHEST_GL_VERSION_MINOR; \
			} \
		}
	#endif
	
	// warnings
	
	#ifndef KOS_WARN_NO_GL_VERSION
		#define KOS_WARN_NO_GL_VERSION printf("WARNING You seem to have an inexistant OpenGL version (%d.%d)\n", kos_best_gl_version_major, kos_best_gl_version_minor);
	#endif
	
	// prototypes (temporary)
	
	void glShaderSource(GLuint shader, GLsizei count, const GLchar** string, const GLint* length);
	void glCompileShader(GLuint shader);
	void glGetShaderiv(GLuint shader, GLenum pname, GLint* params);
	void glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
	
	GLuint glCreateShader(GLenum shaderType);
	GLuint glCreateProgram(void);
	
	void glAttachShader(GLuint program, GLuint shader);
	void glLinkProgram(GLuint program);
	void glGetProgramiv(GLuint program, GLenum pname, GLint* params);
	void glGetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
	void glDeleteShader(GLuint shader);
	void glUseProgram(GLuint program);
	
#endif
