
#ifndef __AQUA__KOS_FUNCTIONS_SCREENSHOT_H
	#define __AQUA__KOS_FUNCTIONS_SCREENSHOT_H
	
	#define CREATE_TEXTURE_FROM_SCREENSHOT_FUNCTION 1
	
	texture_t create_texture_from_screenshot(void) {
		unsigned long long bpp = 32;
		
		unsigned char* pixels = (unsigned char*) malloc(video_width() * video_height() * (bpp / 8));
		glReadPixels(0, 0, video_width(), video_height(), GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		
		texture_t texture = texture_create((unsigned long long*) pixels, bpp, video_width(), video_height());
		free(pixels);
		
		if (texture < 0) {
			printf("WARNING Could not create texture from screenshot\n");
			
		}
		
		return texture;
		
	}
	
#endif
