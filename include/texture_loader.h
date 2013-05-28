#ifndef TEXTURE_LOADER_H_INCLUDED
#define TEXTURE_LOADER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <FreeImage.h>
#include "macros.h"

void initFreeImage();
void destroyFreeImage();
GLuint loadtexture(const char* file);

#endif // TEXTURE_LOADER_H_INCLUDED
