#include <GL/gl3w.h>
#include <cglm/cglm.h>

#include "debug_draw.h"

#include "../shader.h"
#include "cglm/affine.h"
#include "cglm/mat4.h"
#include "debug_draw_objects.h"
#include <string.h>

#include "debug_3d_frag.h"
#include "debug_3d_vert.h"

// Global context
DebugDrawContext *currentContext = NULL;

void initDebugDrawObjects();

DebugDrawContext *initDebugContext() {
  DebugDrawContext *context = malloc(sizeof(DebugDrawContext));
  context->commands = malloc(sizeof(DebugDrawCommand) * 1024);
  context->commandCount = 0;
  context->commandCapacity = 1024;
  context->shaderProgram =
      createShaderProgram(debug_3d_vert_glsl, debug_3d_frag_glsl);
  setCurrentDebugContext(context);
  initDebugDrawObjects();
  return context;
}

void deinitDebugContext(DebugDrawContext *context) {
  free(context->commands);
  for (size_t i = 0; i < 8; i++) {
    if (context->objects[i].vbo != UINT32_MAX) {
      glDeleteBuffers(1, &context->objects[i].vbo);
    }
    if (context->objects[i].ebo != UINT32_MAX) {
      glDeleteBuffers(1, &context->objects[i].ebo);
    }
    if (context->objects[i].vao != UINT32_MAX) {
      glDeleteVertexArrays(1, &context->objects[i].vao);
    }
  }
  free(context);
}

void setCurrentDebugContext(DebugDrawContext *context) {
  currentContext = context;
}

void postDrawRequest(DebugDrawCommand command) {
  if (!currentContext) {
    fprintf(stderr, "No debug context set. You probably forgot to call "
                    "setCurrentDebugContext.\n");
    return;
  }
  if (currentContext->commandCount >= currentContext->commandCapacity) {
    currentContext->commandCapacity *= 2;
    currentContext->commands =
        realloc(currentContext->commands,
                sizeof(DebugDrawCommand) * currentContext->commandCapacity);
  }
  currentContext->commands[currentContext->commandCount++] = command;
}

void initDebugDrawObjects() {
  for (size_t i = 0; i < 8; i++) {
    if (DebugDrawObjects[i] == NULL) {
      currentContext->objects[i].vbo = UINT32_MAX;
      currentContext->objects[i].ebo = UINT32_MAX;
      currentContext->objects[i].vao = UINT32_MAX;
      currentContext->objects[i].indexCount = 0;
      continue;
    }
    const DebugDrawObjectVertices *object = DebugDrawObjects[i];

    glGenVertexArrays(1, &currentContext->objects[i].vao);
    glGenBuffers(1, &currentContext->objects[i].vbo);
    glGenBuffers(1, &currentContext->objects[i].ebo);
    currentContext->objects[i].indexCount = object->indexCount;
    currentContext->objects[i].primitiveType = object->primitiveType;

    glBindVertexArray(currentContext->objects[i].vao);
    glBindBuffer(GL_ARRAY_BUFFER, currentContext->objects[i].vbo);
    glBufferData(GL_ARRAY_BUFFER, object->vertexCount * sizeof(vec3),
                 object->vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void *)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentContext->objects[i].ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, object->indexCount * sizeof(uint8_t),
                 object->indices, GL_STATIC_DRAW);
    glBindVertexArray(0);
  }
}

void processDrawRequests(mat4 viewProjection) {
  if (!currentContext) {
    fprintf(stderr, "No debug context set. You probably forgot to call "
                    "setCurrentDebugContext.\n");
    return;
  }

  glUseProgram(currentContext->shaderProgram);
  glUniformMatrix4fv(
      glGetUniformLocation(currentContext->shaderProgram, "viewProjection"), 1,
      GL_FALSE, viewProjection[0]);
  for (size_t i = 0; i < currentContext->commandCount; i++) {
    DebugDrawCommand command = currentContext->commands[i];

    // let's construct model matrix
    vec3 pos;
    glm_vec3_copy(command.start, pos);

    vec3 size;
    glm_vec3_sub(command.end, command.start, size);

    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_translate(model, pos);
    glm_scale(model, size);
    glUniformMatrix4fv(
        glGetUniformLocation(currentContext->shaderProgram, "view_projection"),
        1, GL_FALSE, viewProjection[0]);
    glUniform3fv(glGetUniformLocation(currentContext->shaderProgram, "color"),
                 1, command.color);
    glUniformMatrix4fv(
        glGetUniformLocation(currentContext->shaderProgram, "model"), 1,
        GL_FALSE, model[0]);

    switch (command.type) {
    case LINE:
      break;
    case ARROW:
      break;
    case CYLINDER:
      break;
    case CYLINDER_WIREFRAME:
      break;
    case SPHERE:
      break;
    case SPHERE_WIREFRAME:
      break;
    case CUBE:
      glBindVertexArray(currentContext->objects[CUBE].vao);
      glDrawElements(currentContext->objects[CUBE].primitiveType,
                     currentContext->objects[CUBE].indexCount, GL_UNSIGNED_BYTE,
                     0);
      glBindVertexArray(0);
      break;
    case CUBE_WIREFRAME:
      glBindVertexArray(currentContext->objects[CUBE_WIREFRAME].vao);
      glDrawElements(currentContext->objects[CUBE_WIREFRAME].primitiveType,
                     currentContext->objects[CUBE_WIREFRAME].indexCount,
                     GL_UNSIGNED_BYTE, 0);
      glBindVertexArray(0);
      break;
    }
  }
  currentContext->commandCount = 0;
  int error = glGetError();
  if (error != GL_NO_ERROR) {
    fprintf(stderr, "OpenGL error: %d\n", error);
  }
}
