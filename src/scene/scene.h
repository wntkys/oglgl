#pragma once

#include "SDL3/SDL_events.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct SceneManager_s SceneManager;
typedef struct Scene_s Scene;

typedef struct Scene_s {
  void (*init)(Scene *self);
  void (*deinit)(Scene *self);
  void (*update)(Scene *self, uint64_t dt);
  void (*draw)(Scene *self);
  void (*handle_events)(Scene *self, SDL_Event *event);
  void *data;
  bool loaded;
  char name[64];
  SceneManager *manager;
} Scene;

typedef struct SceneManager_s {
  Scene *current_scene;
  Scene *scenes;
  size_t scene_count;
  size_t scene_capacity;
} SceneManager;

void SceneManager_init(SceneManager *manager);
void SceneManager_deinit(SceneManager *manager);
void SceneManager_addScene(SceneManager *manager, Scene *scene);
void SceneManager_setCurrentScene(SceneManager *manager,
                                  const char *scene_name);
void SceneManager_mainLoop(SceneManager *manager);