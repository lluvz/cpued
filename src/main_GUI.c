#include"cpu.c"
#include<X11/Xlib.h>
int main(){
    Display*display=XOpenDisplay(NULL);
    XEvent event;
    if(display==NULL){
        fprintf(stderr,"Unable to connect to the X server.\n");
        return -1;
    }
    int screen=DefaultScreen(display);
    Window window=XCreateSimpleWindow(display,RootWindow(display,screen),300,300,300,300,1,0,0);
    XSelectInput(display,window,ExposureMask|KeyPressMask);
    XMapWindow(display,window);
    XStoreName(display,window,"CPUED GUI");
    for(;;){
        XNextEvent(display,&event);
        if(event.type==KeyPress){
            break;
        }
    }
    XCloseDisplay(display);
    return 0;
}