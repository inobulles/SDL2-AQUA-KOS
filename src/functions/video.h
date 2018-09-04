
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
	
	unsigned long long video_width (void) { return current_kos->width;  }
	unsigned long long video_height(void) { return current_kos->height; }
	
	static unsigned long long kos_last_time;
	
	unsigned long long video_fps(void) {
		unsigned long long tick_time = SDL_GetTicks();
		float fps = 1000.0f / (float) (tick_time - kos_last_time);
		kos_last_time = tick_time;
		
		return (unsigned long long) fps;
		
	}
	
	void set_video_visibility(unsigned long long state) {
		KOS_DEPRECATED
		
		if (state == HIDDEN) {
			SDL_MinimizeWindow(current_kos->window);
			
		}
		
	}
	
	static unsigned char kos_is_mouse_pressed = 0;
	static unsigned char kos_has_clicked = 0;
	
	static unsigned long long get_device_keyboard_key;
	static unsigned long long resize_count;
	
	void get_events(event_list_t* this) { // I guess this shouldn't be here but idc tbh
		SDL_Event event;
		
		this->quit = 0;
		this->pointer_click_type = kos_is_mouse_pressed;
		
		SDL_GetMouseState((int*) &this->pointer_x, (int*) &this->pointer_y);
		
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				this->quit = 1;
				break;
				
			} else if (event.type == SDL_WINDOWEVENT) {
				switch (event.window.event) {
					case SDL_WINDOWEVENT_SIZE_CHANGED: {
						resize_count++;
						this->resize = 1;
						
						current_kos->width  = event.window.data1;
						current_kos->height = event.window.data2;
						
						glViewport(0, 0, current_kos->width, current_kos->height);
						
						break;
						
					} default: {
						break;
						
					}
					
				}
				
				break;
				
			} else if (event.type == SDL_MOUSEBUTTONDOWN) {
				kos_has_clicked = 1;
				kos_is_mouse_pressed = 1;
				break;
				
			} else if (event.type == SDL_MOUSEBUTTONUP) {
				kos_is_mouse_pressed = 0;
				break;
				
			} else if (event.type == SDL_KEYDOWN) {
				get_device_keyboard_key = event.key.keysym.scancode;
				break;
				
			}
			
		}
		
	}
	
	void free_events(event_list_t* this) {
		
	}
	
	// predefined textures
	
	static texture_t predefined_background_texture;
	static texture_t predefined_frost_background_texture;
	
	void bmp_load(bitmap_image_t* this, unsigned long long _path);
	void bmp_free(bitmap_image_t* this);
	
	static int kos_setup_predefined_textures(kos_t* this) {
		int warning = 0;
		bitmap_image_t temp_bmp;
		
		bmp_load(&temp_bmp, (unsigned long long) "wallpaper/wallpaper.bmp");
		predefined_background_texture = texture_create(temp_bmp.data, temp_bmp.bpp, temp_bmp.width, temp_bmp.height);
		bmp_free(&temp_bmp);
		warning += predefined_background_texture == -1;
		
		bmp_load(&temp_bmp, (unsigned long long) "wallpaper/frost.bmp");
		predefined_frost_background_texture = texture_create(temp_bmp.data, temp_bmp.bpp, temp_bmp.width, temp_bmp.height);
		bmp_free(&temp_bmp);
		warning += predefined_frost_background_texture == -1;
		
		return warning;
		
	}
	
	void kos_free_predefined_textures(kos_t* this) {
		if (predefined_background_texture       != -1) texture_remove(predefined_background_texture);
		if (predefined_frost_background_texture != -1) texture_remove(predefined_frost_background_texture);
		
	}
	
	texture_t get_predefined_texture(unsigned long long name) {
		switch (name) {
			case TEXTURE_BACKGROUND:         return predefined_background_texture;
			case TEXTURE_FROSTED_BACKGROUND: return predefined_frost_background_texture;
			
			default: {
				printf("WARNING Texture %lld is unknown. Returning -1 ...\n", name);
				return -1;
				
			}
			
		}
		
	}
	
	void update_predefined_texture(unsigned long long name) {
		switch (name) {
			case TEXTURE_BACKGROUND: {
				break;
				
			} case TEXTURE_FROSTED_BACKGROUND: {
				break;
				
			} default: {
				printf("WARNING Texture %lld is unknown\n", name);
				break;
				
			}
			
		}
		
	}
	
#endif
