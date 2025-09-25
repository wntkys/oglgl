#include "scene/scene.h"
#include "scenes/main_scene.h"
#include "stdlib.h"
#define _GNU_SOURCE

#include <GL/gl3w.h>

#include "base/debug_draw/debug_draw.h"
#include "base/init.h"

#include <stdbool.h>

#include <SDL3/SDL.h>
#include <cglm/cglm.h>

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  if (initSDLAndOpenGL() != 0) {
    return 1;
  }

  setCurrentDebugContext(initDebugContext());

  SceneManager *manager = malloc(sizeof(SceneManager));
  SceneManager_init(manager);

  Scene *scene = malloc(sizeof(Scene));
  MainScene_get(scene);
  SceneManager_addScene(manager, scene);
  SceneManager_setCurrentScene(manager, "MainScene");
  SceneManager_mainLoop(manager);

  deinitSDLAndOpenGL();

  return 0;
}
