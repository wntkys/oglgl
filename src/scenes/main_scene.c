#define _GNU_SOURCE
#include <math.h>

#include <stdint.h>

#include "../base/base.h"
#include "../base/debug_draw/debug_draw.h"
#include "GL/gl3w.h"
#include "main_scene.h"
#include <SDL3/SDL.h>
#include <cglm/cglm.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
  uint64_t currentTime;
  DebugDrawContext *debugContext;
} MainSceneData;

void MainScene_init(Scene *self) {
  self->data = malloc(sizeof(MainSceneData));
  MainSceneData *data = self->data;
  data->debugContext = initDebugContext();
  setCurrentDebugContext(data->debugContext);
}
void MainScene_deinit(Scene *self) {
  MainSceneData *data = self->data;
  deinitDebugContext(data->debugContext);
  free(data);
  self->data = NULL;
  self->loaded = false;
}

void MainScene_handle_events(Scene *self, SDL_Event *event) {
  MainSceneData *data = self->data;
  (void)event;
  (void)data;
}

void MainScene_draw(Scene *self) {
  MainSceneData *data = self->data;
  (void)data;
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
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

void MainScene_update(Scene *self, uint64_t dt) {
  MainSceneData *data = self->data;
  data->currentTime += dt;

  float time = (float)data->currentTime / 1000.0f;

  for (int i = 0; i < 100; i++) {
    float angle = (float)i / 100.0f * 2.0f * M_PI;

    vec3 pos = {cosf(angle) * 5, sinf(angle) * 5, 0.0f};
    vec3 pos_end;
    glm_vec3_add(pos,
                 (vec3){0.3f * sinf(time + i * M_PI / 100 * 2 * 5) + 0.4f,
                        0.3f * sinf(time + i * M_PI / 100 * 2 * 5) + 0.4f,
                        0.3f * sinf(time + i * M_PI / 100 * 2 * 5) + 0.4f},
                 pos_end);

    postDrawRequest((DebugDrawCommand){
        .type = CUBE_WIREFRAME,
        .color = {1.0f, 0.0f, 0.0f},
        .start = {pos[0], pos[1], pos[2]},
        .end = {pos_end[0], pos_end[1], pos_end[2]},
    });
  }
}

void MainScene_get(Scene *scene) {
  scene->init = MainScene_init;
  scene->deinit = MainScene_deinit;
  scene->update = MainScene_update;
  scene->draw = MainScene_draw;
  scene->handle_events = MainScene_handle_events;
  strcpy(scene->name, "MainScene");
}