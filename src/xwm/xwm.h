
#ifndef __AQUA__SDL2_KOS__XWM_XWM_H
	#define __AQUA__SDL2_KOS__XWM_XWM_H
	
	#include <X11/Xlib.h>
	
	static unsigned char xwm_detected = 0;
	
	static int on_xwm_detected(Display* display, XErrorEvent* event) {
		xwm_detected = 1;
		return 0;
		
	}
	
	static int on_xwm_error(Display* display, XErrorEvent* event) {
		#define    MAX_ERROR_LENGTH 1024
		char error[MAX_ERROR_LENGTH];
		
		XGetErrorText(display, event->error_code, error, MAX_ERROR_LENGTH);
		printf("WARNING Received X error, request = %d, error code = %d - \"%s\", resource id = %ld\n", event->request_code, event->error_code, error, event->resourceid);
		
		return 0;
		
	}
	
	unsigned char open_xwm(void) {
		Display* display = XOpenDisplay(0);
		
		if (!display) {
			printf("WARNING Could not open X display\n");
			return 1;
			
		}
		
		Window root = DefaultRootWindow(display);
		XSetErrorHandler(on_xwm_detected);
		
		XSelectInput(display, root, SubstructureRedirectMask | SubstructureNotifyMask);
		XSync(display, 0);
		
		if (xwm_detected) {
			printf("WARNING Detected another WM on display\n");
			return 1;
			
		}
		
		XSetErrorHandler(on_xwm_error);
		
		return 0;
		
	}
	
#endif

