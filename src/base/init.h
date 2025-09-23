#ifndef INIT_H
#define INIT_H

#include <stdint.h>

int initSDLAndOpenGL();
int SDLAndOpenGLLoop(void (*draw)(), void (*update)(uint64_t));
int deinitSDLAndOpenGL();

#endif