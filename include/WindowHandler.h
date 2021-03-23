#ifndef NOTAMONGUS_SRC_WINDOWHANDLER_H_
#define NOTAMONGUS_SRC_WINDOWHANDLER_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

class WindowHandler {
private:
    static void framebufferSizeCallback(GLFWwindow *window,
                                        int width,
                                        int height);
public:
    WindowHandler(int width, int height, char *title);

    GLFWwindow *window;
};

#endif //NOTAMONGUS_SRC_WINDOWHANDLER_H_