// Generated with Shader Minifier 1.5.1 (https://github.com/laurentlb/Shader_Minifier/)
#ifndef DEBUG_3D_VERT_H_
# define DEBUG_3D_VERT_H_

const char *debug_3d_vert_glsl =
 "#version 420 core\n"
 "layout(location=0)in vec3 aPos;"
 "uniform vec3 color;"
 "uniform mat4 view_projection,model;"
 "layout(location=0)out vec3 vColor;"
 "void main()"
 "{"
   "gl_Position=view_projection*model*vec4(aPos,1);"
   "vColor=color;"
 "}";

#endif // DEBUG_3D_VERT_H_
