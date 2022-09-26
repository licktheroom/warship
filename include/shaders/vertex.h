// Copyright (C) 2022  licktheroom //

#ifndef VERTEX_SHADER_HEADER
#define VERTEX_SHADER_HEADER
// this isn't fun to read
// that's why there's a plain text version

static const char * VERTEX_shader =
"// Copyright (C) 2022  licktheroom //\n"
"\n"
"#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"
"\n"
"uniform ivec2 scn;\n"
"uniform ivec3 pos[4001];\n"
"\n"
"uniform float scale;\n"
"\n"
"out float col;\n"
"\n"
"void main()\n"
"{\n"
"    vec2 finished = aPos;\n"
"\n"
"    finished.x += pos[gl_InstanceID].x;\n"
"    finished.y += pos[gl_InstanceID].y;\n"
"\n"
"    finished.x *= scale;\n"
"    finished.y *= scale;\n"
"\n"
"    finished /= scn;\n"
"\n"
"    finished.x = -1 + finished.x;\n"
"    finished.y = 1 - finished.y;\n"
"\n"
"    col = pos[gl_InstanceID].z;\n"
"\n"
"    gl_Position = vec4(finished, 0.0f, 1.0f);\n"
"}\0";

#endif
