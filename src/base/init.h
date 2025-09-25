#ifndef INIT_H
#define INIT_H

#include "SDL3/SDL_events.h"
#include <stdint.h>

int initSDLAndOpenGL();
int SDLAndOpenGLLoop(void (*draw)(), void (*update)(uint64_t),
                     void (*handle_events)(SDL_Event *event));
int deinitSDLAndOpenGL();

#endif