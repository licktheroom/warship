// Copyright (C) 2022  licktheroom //

#ifndef FRAGMENT_SHADER_HEADER
#define FRAGMENT_SHADER_HEADER

static const char * FRAGMENT_shader =
"// Copyright (C) 2022  licktheroom //\n"
"\n"
"#version 330 core\n"
"out vec4 FragColor;\n"
"\n"
"in float col;\n"
"\n"
"void main()\n"
"{\n"
"    if(col == 0)\n"
"        FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"    else if(col == 1)\n"
"        FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);\n"
"    else if(col == 2)\n"
"        FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
"    else\n"
"        FragColor = vec4(0.0f, 0.0f, 0.5f, 1.0f);\n"
"}\0";


#endif
