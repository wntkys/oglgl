#include <GL/gl3w.h>
#include <stdio.h>

#include "shader.h"

unsigned int compileShader(unsigned int type, const char *source) {
  unsigned int shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  // Check for compilation errors
  int success;
  char infoLog[4096];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 4096, NULL, infoLog);
    printf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
    return 0;
  }

  return shader;
}

unsigned int createShaderProgram(const char *vertexShaderSource,
                                 const char *fragmentShaderSource) {
  unsigned int vertexShader =
      compileShader(GL_VERTEX_SHADER, vertexShaderSource);
  unsigned int fragmentShader =
      compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

  if (!vertexShader || !fragmentShader) {
    return 0;
  }

  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  int success;
  char infoLog[4096];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 4096, NULL, infoLog);
    printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    return 0;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}
