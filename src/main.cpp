#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include <GL/glew.h>
#include <GL/glfw.h>
#include "defines.h"
#include "Engine.h"
#include "texture_loader.h"

int main()
{
    glfwInit();
    if (glfwOpenWindow(WIDTH, HEIGHT, 0, 0, 0, 0, 0, 0, GLFW_WINDOW)==GL_FALSE)
    {
        DEBUG_PRINT(("Error opening window, goodbye\n"));
        return 1;
    }
    GLenum err=glewInit();
    if (GLEW_OK!=err)
    {
        fprintf(stderr, "GLEW error: %s\n", glewGetErrorString(err));
        return 1;
    }
    initFreeImage();
    glfwSetWindowTitle("Space Ship Game!!!");
    glfwSetKeyCallback(keypress);
    glfwSetWindowSizeCallback(windowresize);
    setup();
    renderloop();
    cleanup();
    glfwCloseWindow();
    glfwTerminate();
    destroyFreeImage();
    DEBUG_PRINT(("program ended successfully"));
    return 0;
}
