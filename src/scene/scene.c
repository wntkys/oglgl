#include "scene.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../base/base.h"
#include <GL/gl3w.h>
#include <SDL3/SDL.h>

void SceneManager_init(SceneManager *manager) {
  manager->current_scene = NULL;
  manager->scenes = malloc(sizeof(Scene) * 16);
  manager->scene_count = 0;
  manager->scene_capacity = 16;
}

void SceneManager_deinit(SceneManager *manager) {
  for (size_t i = 0; i < manager->scene_count; i++) {
    manager->scenes[i].deinit(manager->scenes[i].data);
  }
  free(manager->scenes);
  manager->scenes = NULL;
  manager->scene_count = 0;
}

Scene *SceneManager_findByName(SceneManager *manager, const char *scene_name) {
  for (size_t i = 0; i < manager->scene_count; i++) {
    if (strcmp(manager->scenes[i].name, scene_name) == 0) {
      return &manager->scenes[i];
    }
  }
  return NULL;
}

void SceneManager_addScene(SceneManager *manager, Scene *scene) {
  if (manager->scene_count == manager->scene_capacity) {
    // here problem is that realloc can change pointer and we have to update
    // current_scene pointer

    // we have to first check what scene is currently loaded
    char current_scene_name[64];
    if (manager->current_scene != NULL) {
      strcpy(current_scene_name, manager->current_scene->name);
    }

    manager->scenes =
        realloc(manager->scenes, sizeof(Scene) * manager->scene_capacity * 2);
    manager->scene_capacity *= 2;

    if (manager->current_scene >= &manager->scenes[manager->scene_count]) {
      SceneManager_setCurrentScene(manager, current_scene_name);
    }
  }
  manager->scenes[manager->scene_count] = *scene;
  manager->scene_count++;
}

void SceneManager_setCurrentScene(SceneManager *manager,
                                  const char *scene_name) {
  manager->current_scene = SceneManager_findByName(manager, scene_name);
  if (manager->current_scene != NULL &&
      manager->current_scene->loaded == false) {
    manager->current_scene->init(manager->current_scene);
    manager->current_scene->loaded = true;
  }
}

void onRescaleHook(SDL_Event *event) {
  glViewport(0, 0, event->window.data1, event->window.data2);
  WINDOW_WIDTH = event->window.data1;
  WINDOW_HEIGHT = event->window.data2;
}

void SceneManager_mainLoop(SceneManager *manager) {
  SDL_Event event;
  uint64_t lastTime = SDL_GetTicks();

  bool running = true;
  while (running) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_EVENT_QUIT:
        running = false;
        break;
      case SDL_EVENT_WINDOW_RESIZED:
        onRescaleHook(&event);
        goto endFrame;
        break;
      default:
        manager->current_scene->handle_events(manager->current_scene, &event);
        break;
      }
    }
  endFrame:
    uint64_t currentTime = SDL_GetTicks();
    manager->current_scene->update(manager->current_scene,
                                   currentTime - lastTime);
    lastTime = currentTime;
    manager->current_scene->draw(manager->current_scene);
    SDL_GL_SwapWindow(window);
  }
}
