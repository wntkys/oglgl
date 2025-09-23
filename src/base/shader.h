#ifndef SHADER_H
#define SHADER_H

unsigned int compileShader(unsigned int type, const char *source);
unsigned int createShaderProgram(const char *vertexShaderSource,
                                 const char *fragmentShaderSource);

#endif // SHADER_H