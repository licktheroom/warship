#ifndef VERTEX_SHADER_HEADER
#define VERTEX_SHADER_HEADER
// this isn't fun to read
// that's why there's a plain text version

static const char * VERTEX_shader =
"#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"
"\n"
"uniform ivec2 scn;\n"
"uniform ivec2 pos[1500];\n"
"\n"
"uniform float scale;\n"
"\n"
"void main()\n"
"{\n"
"    vec2 finished = aPos;\n"
"\n"
"    finished += pos[gl_InstanceID];\n"
"\n"
"    finished.x *= scale;\n"
"    finished.y *= scale;\n"
"\n"
"    finished /= scn;\n"
"\n"
"    finished.x = -1 + finished.x;\n"
"    finished.y = 1 - finished.y;\n"
"\n"
"    gl_Position = vec4(finished, 0.0f, 1.0f);\n"
"}\0";



#endif
