#include "WindowHandler.h"
#include "Maze.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

char *WINDOW_TITLE = "Not Among Us";
int WIDTH = 600;
int HEIGHT = 800;

void exitWindow() {
    glfwTerminate();
}

void updateWindow(WindowHandler windowHandler) {
    /* Set window background to Black */
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    /* Check and call events and swap buffers */
    glfwSwapBuffers(windowHandler.window);
    glfwPollEvents();
}

int main() {
    auto windowHandler = WindowHandler(WIDTH, HEIGHT, WINDOW_TITLE);

    /* While window is not closed */
    while (!glfwWindowShouldClose(windowHandler.window)) {
        updateWindow(windowHandler);
    }
    exitWindow();
}