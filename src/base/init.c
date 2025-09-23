#include "init.h"
#include <GL/gl3w.h>
#include <SDL3/SDL.h>
#include <stdio.h>

#include "base.h"

int initSDLAndOpenGL() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  // Create window
  window =
      SDL_CreateWindow("OpenGL Triangle - SDL3 + gl3w + cglm", WINDOW_WIDTH,
                       WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  if (window == NULL) {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  // Create OpenGL context
  glContext = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, glContext);
  if (glContext == NULL) {
    printf("OpenGL context could not be created! SDL_Error: %s\n",
           SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  // Initialize gl3w
  if (gl3wInit() != 0) {
    printf("Failed to initialize gl3w\n");
    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  // Print OpenGL information
  printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
  printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
  printf("Renderer: %s\n", glGetString(GL_RENDERER));

  // Enable VSync
  SDL_GL_SetSwapInterval(1);

  return 0;
}

void onRescaleHook(SDL_Event *event) {
  glViewport(0, 0, event->window.data1, event->window.data2);
  WINDOW_WIDTH = event->window.data1;
  WINDOW_HEIGHT = event->window.data2;
}

int SDLAndOpenGLLoop(void (*draw)(), void (*update)(uint64_t)) {
  SDL_Event event;
  uint64_t lastTime = SDL_GetTicks();

  bool running = true;
  while (running) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_EVENT_QUIT:
        running = false;
      case SDL_EVENT_WINDOW_RESIZED:
        onRescaleHook(&event);
        break;
      }
    }
    uint64_t currentTime = SDL_GetTicks();
    update(currentTime - lastTime);
    lastTime = currentTime;
    draw();
    SDL_GL_SwapWindow(window);
  }
  return 1;
}

int deinitOpenGL() { return 0; }

int deinitSDL() {
  SDL_GL_DestroyContext(glContext);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

int deinitSDLAndOpenGL() {
  deinitOpenGL();
  deinitSDL();
  return 0;
}
