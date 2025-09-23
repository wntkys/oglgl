#define _GNU_SOURCE
#include <math.h>

#include <GL/gl3w.h>

#include "base/base.h"
#include "base/debug_draw/debug_draw.h"
#include "base/init.h"

#include <stdbool.h>

#include <SDL3/SDL.h>
#include <cglm/cglm.h>

void drawTriangle() {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  mat4 view;

  glm_lookat((vec3){5.0f, 5.0f, 5.0f}, (vec3){0.0f, 0.0f, 0.0f},
             (vec3){0.0f, 0.0f, 1.0f}, view);

  mat4 projection;
  glm_perspective(1.25f, (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f,
                  projection);

  mat4 viewProjection;
  glm_mul(projection, view, viewProjection);

  processDrawRequests(viewProjection);
}

void update(uint64_t dt) {
  for (int i = 0; i < 10; i++) {
    float angle = (float)i / 9.0f * 2.0f * M_PI;

    vec3 pos = {cosf(angle) * 5, sinf(angle) * 5, 0.0f};
    vec3 pos_end;
    glm_vec3_add(pos,
                 (vec3){0.3f * (float)rand() / RAND_MAX,
                        0.3f * (float)rand() / RAND_MAX,
                        0.3f * (float)rand() / RAND_MAX},
                 pos_end);

    postDrawRequest((DebugDrawCommand){
        .type = CUBE_WIREFRAME,
        .color = {1.0f, 0.0f, 0.0f},
        .start = {pos[0], pos[1], pos[2]},
        .end = {pos_end[0], pos_end[1], pos_end[2]},
    });
  }
}

int main(int argc, char *argv[]) {
  setenv("SDL_VIDEODRIVER", "x11", 1);
  if (initSDLAndOpenGL() != 0) {
    return 1;
  }

  setCurrentDebugContext(initDebugContext());

  SDLAndOpenGLLoop(drawTriangle, update);
  deinitSDLAndOpenGL();

  return 0;
}
