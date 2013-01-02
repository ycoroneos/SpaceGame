#include "Engine.h"
glm::mat4 projection;
int width, height;

void GLFWCALL keypress(int key, int state)
{
    DEBUG_PRINT(("key %d is in state %d\n", key, state));
}

void GLFWCALL windowresize( int nwidth, int nheight )
{
    width=nwidth;
    height=nheight;
    glViewport(0,0,width,height);
}

void setup()
{
    projection=glm::perspective(45.0f, (float)(width/height), 0.1f, 100.0f);

    ModelObject test;
    //glf::initmesh(&test);
    glf::loadmodel(&test,"sponza","spnoza.jpeg","sponza.");
    glBindBuffer(GL_ARRAY_BUFFER, test.vbo);
}

void renderloop()
{
    while(!glfwGetKey(GLFW_KEY_ESC))
    {
        glClearColor(255,255,255,255);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glfwSwapBuffers();
    }
}

void cleanup()
{

}
