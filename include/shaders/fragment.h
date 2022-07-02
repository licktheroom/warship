#ifndef FRAGMENT_SHADER_HEADER
#define FRAGMENT_SHADER_HEADER

static const char * FRAGMENT_shader =
"#version 330 core\n"
"out vec4 FragColor;\n"
"\n"
"uniform int color;\n"
"\n"
"void main()\n"
"{\n"
"    if(color == 0)\n"
"        FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"    else if(color == 1)\n"
"        FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);\n"
"    else\n"
"        FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
"}\0";

#endif
