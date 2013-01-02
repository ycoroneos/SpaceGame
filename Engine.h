#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED
//#include <GL/glfw.h>
#include <stdio.h>
#include "macros.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glf.h"
#include "objloader.h"

void GLFWCALL windowresize( int width, int height );
void GLFWCALL keypress(int key, int state);
void setup();
void renderloop();
void cleanup();

#endif // ENGINE_H_INCLUDED
