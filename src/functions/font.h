
#ifndef __AQUA__SDL2_SRC_FUNCTIONS_FONT_H
	#define __AQUA__SDL2_SRC_FUNCTIONS_FONT_H
	
	#include "../macros_and_inclusions.h"
	#include <SDL2/SDL_ttf.h>
	
	typedef struct {
		unsigned char used;
		char* text;
		
		TTF_Font* font;
		SDL_Surface* surface;
		
	} kos_font_t;
	
	#ifndef KOS_MAX_FONTS
		#define KOS_MAX_FONTS 4096
	#endif
	
	static kos_font_t kos_fonts[KOS_MAX_FONTS];
	static SDL_Color kos_font_colour;
	
	#ifndef KOS_CHECK_FONT
		#define KOS_CHECK_FONT(return_value) { \
			if (this < 0 && this >= KOS_MAX_FONTS && !kos_fonts[this].used) { \
				printf("WARNING Font %lld does not exist\n", this); \
				return (return_value); \
			} \
		}
	#endif
	
	static void kos_unuse_font(kos_font_t* this) {
		this->used = 0;
		this->text = NULL;
		
		this->font = NULL;
		this->surface = NULL;
		
	}
	
	void kos_init_fonts(void) { /// TO... IMPLEMENT?
		kos_font_colour.r = 0xFF;
		kos_font_colour.g = 0xFF;
		kos_font_colour.b = 0xFF;
		kos_font_colour.a = 0xFF;
		
		unsigned long long i;
		for (i = 0; i < KOS_MAX_FONTS; i++) {
			kos_unuse_font(&kos_fonts[i]);
			
		}
		
		if (TTF_Init() == -1) {
			printf("WARNING TTF could not initialize (%s)\n", TTF_GetError());
			return;
			
		}
		
	}
	
	void kos_destroy_fonts(void) {
		TTF_Quit();
		
	}
	
	font_t new_font(const char* _path, unsigned long long size) {
		unsigned long long i;
		for (i = 0; i < KOS_MAX_FONTS; i++) {
			if (!kos_fonts[i].used) {
				kos_fonts[i].used = 1;
				
				GET_PATH((char*) _path);
				kos_fonts[i].font = TTF_OpenFont(path, size);
				
				if (!kos_fonts[i].font) {
					printf("WARNING Font could not be loaded (possibly an incorrect path? `%s`)\n", path);
					kos_fonts[i].used = 0;
					
					return -1;
					
				}
				
				return i;
				
			}
			
		}
		
		printf("WARNING You have surpassed the maximum font count (KOS_MAX_FONTS = %d)\n", KOS_MAX_FONTS);
		return -1;
		
	}
	
	static void kos_font_create_text(kos_font_t* this, char* text) {
		if (!this->surface || (this->text == NULL || strcmp(text, this->text) != 0)) {
			if (this->surface) {
				SDL_FreeSurface(this->surface);
				this->surface = NULL;
				
			}
			
			this->text = text;
			
			SDL_Surface* temp = TTF_RenderUTF8_Blended(this->font, text, kos_font_colour);
			this->surface = SDL_CreateRGBSurface(0, temp->w, temp->h, 32, 0xFF, 0xFF << 8, 0xFF << 16, 0xFF << 24);
			
			SDL_BlitSurface(temp, NULL, this->surface, NULL);
			SDL_FreeSurface(temp);
			
			if (!this->surface) {
				printf("WARNING Could not create the font surface (SDL `%s`, TTF `%s`)\n", SDL_GetError(), TTF_GetError());
				this->used = 0;
				
				return;
				
			}
			
			SDL_LockSurface(this->surface);
			uint64_t* pixels = (uint64_t*) this->surface->pixels;
			
			unsigned long long i;
			for (i = 0; i < (this->surface->w * this->surface->h) / 2; i++) {
				pixels[i] &= 0xFF000000FF000000;
				pixels[i] += 0x00FFFFFF00FFFFFF;
				
			}
			
		}
		
	}
	
	unsigned long long font_remove(font_t this) {
		KOS_CHECK_FONT(-1)
		
		if (kos_fonts[this].surface) {
			SDL_FreeSurface(kos_fonts[this].surface);
			
		}
		
		TTF_CloseFont(kos_fonts[this].font);
		kos_unuse_font(&kos_fonts[this]);
		
		return 0;
		
	}
	
	texture_t create_texture_from_font(font_t this, char* text) {
		KOS_CHECK_FONT(0)
		
		kos_font_t* font = &kos_fonts[this];
		kos_font_create_text(font, text);
		
		return __texture_create(font->surface->pixels, 32, font->surface->w, font->surface->h, 0);
		
	}
	
	unsigned long long get_font_width(font_t this, char* text) {
		KOS_CHECK_FONT(-1)
		
		kos_font_t* font = &kos_fonts[this];
		kos_font_create_text(font, text);
		
		return font->surface->w;
		
	}
	
	unsigned long long get_font_height(font_t this, char* text) {
		KOS_CHECK_FONT(-1)
		
		kos_font_t* font = &kos_fonts[this];
		kos_font_create_text(font, text);
		
		return font->surface->h;
		
	}
	
#endif
