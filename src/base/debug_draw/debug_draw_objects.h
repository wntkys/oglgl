#include "SDL3/SDL_opengl.h"
#include <cglm/cglm.h>

typedef struct {
  vec3 *vertices;
  uint32_t vertexCount;
  uint8_t *indices;
  uint32_t indexCount;
  uint32_t primitiveType;
} DebugDrawObjectVertices;

DebugDrawObjectVertices Cube_wireframe_data = {
    .vertices =
        (vec3[]){
            {-1.0, -1.0, -1.0},
            {1.0, -1.0, -1.0},
            {1.0, 1.0, -1.0},
            {-1.0, 1.0, -1.0},
            {-1.0, -1.0, 1.0},
            {1.0, -1.0, 1.0},
            {1.0, 1.0, 1.0},
            {-1.0, 1.0, 1.0},
        },
    .vertexCount = 8,
    .indices =
        (uint8_t[]){
            0, 1, 1, 2, 2, 3, 3, 0, // back face edges
            4, 5, 5, 6, 6, 7, 7, 4, // front face edges
            0, 4, 1, 5, 2, 6, 3, 7  // vertical edges
        },
    .indexCount = 24,
    .primitiveType = GL_LINES,
};

DebugDrawObjectVertices Cube_data = {
    .vertices =
        (vec3[]){
            {-1.0, -1.0, -1.0},
            {1.0, -1.0, -1.0},
            {1.0, 1.0, -1.0},
            {-1.0, 1.0, -1.0},
            {-1.0, -1.0, 1.0},
            {1.0, -1.0, 1.0},
            {1.0, 1.0, 1.0},
            {-1.0, 1.0, 1.0},
        },
    .vertexCount = 8,
    .indices =
        (uint8_t[]){
            0, 1, 2, 3, // задняя грань
            4, 7, 6, 5, // передняя грань
            0, 4, 5, 1, // нижняя грань
            2, 6, 7, 3, // верхняя грань
            0, 3, 7, 4, // левая грань
            1, 5, 6, 2  // правая грань
        },
    .indexCount = 24,
    .primitiveType = GL_LINE_LOOP,
};

DebugDrawObjectVertices const *const DebugDrawObjects[8] = {
    NULL, NULL, NULL, NULL, NULL, NULL, &Cube_data, &Cube_wireframe_data,
};