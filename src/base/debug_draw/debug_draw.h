#ifndef DEBUG_DRAW_H
#define DEBUG_DRAW_H

#include <cglm/cglm.h>

typedef struct {
  enum {
    LINE,
    ARROW,
    CYLINDER,
    CYLINDER_WIREFRAME,
    SPHERE,
    SPHERE_WIREFRAME,
    CUBE,
    CUBE_WIREFRAME,
  } type;
  vec3 color;
  vec3 start;
  vec3 end;
} DebugDrawCommand;

typedef struct DebugDrawObject {
  uint32_t vbo;
  uint32_t ebo;
  uint32_t vao;
  uint32_t indexCount;
  uint32_t primitiveType;
} DebugDrawObject;

typedef struct {
  DebugDrawCommand *commands;
  size_t commandCount;
  size_t commandCapacity;
  DebugDrawObject objects[8];
  uint32_t shaderProgram;
} DebugDrawContext;

DebugDrawContext *initDebugContext();
void deinitDebugContext(DebugDrawContext *context);
void setCurrentDebugContext(DebugDrawContext *context);

void postDrawRequest(DebugDrawCommand command);
void processDrawRequests(mat4 viewProjection);

#endif