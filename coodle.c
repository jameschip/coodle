#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEYCODE_1 10
#define KEYCODE_2 11

#define BRUSH_TRACE 1
#define BRUSH_BLOCK 2

void init_x(void);
void event_loop(void);
void handle_key_down(unsigned int key_code);
void do_brush(void);

Display *display;
Window window;
XEvent event;
int screen;
Bool drawing = False;
unsigned int brush_type = BRUSH_TRACE;

int main(void) {

    init_x();
    event_loop();
    XCloseDisplay(display);
    return 0;
}

/*
Create the window and display it
*/
void init_x() {
     display = XOpenDisplay(NULL);
    
    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }
 
    screen = DefaultScreen(display);
    window = XCreateSimpleWindow(display, RootWindow(display, screen), 10, 10, 1000, 700, 1,BlackPixel(display, screen), WhitePixel(display, screen));
    XSelectInput(display, window, ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | EnterWindowMask );
    XMapWindow(display, window);
}

void event_loop() {
    while (1) {
        XNextEvent(display, &event);
        switch(event.type) {
                case Expose:
                    break;
                case ButtonPress:
                    drawing = True;
                    do_brush();
                    break;
                case ButtonRelease:
                    drawing = False;
                    break;
                case MotionNotify:
                    if (drawing) {
                        do_brush();
                    } 
                    break;
                case KeyPress:
                    printf("The key code is: %d\n", event.xkey.keycode);
                    handle_key_down(event.xkey.keycode);
                    break;
            }
    }
}

void handle_key_down(unsigned int key_code) {
    switch (key_code) {
        case KEYCODE_1:
            printf("Setting trace brush.\n");
            brush_type = BRUSH_TRACE;
            break;
        case KEYCODE_2:
            printf("Setting block brush.\n");
            brush_type = BRUSH_BLOCK;
            break;
        default:
            printf("unknown key");
            break;
    }
}

void do_brush() {
    switch (brush_type) {
        case BRUSH_TRACE:
            XFillRectangle(display, window, DefaultGC(display, screen), event.xbutton.x, event.xbutton.y, 2, 2);
            break;
        case BRUSH_BLOCK:
            XFillRectangle(display, window, DefaultGC(display, screen), event.xbutton.x, event.xbutton.y, 10, 10);
            break;
    }
}