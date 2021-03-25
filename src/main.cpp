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

void updateWindow(WindowHandler *windowHandler, Maze *maze) {
    /* Set window background to Black */
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    /* Draw the maze */
    maze->draw();

    /* Check and call events and swap buffers */
    glfwSwapBuffers(windowHandler->window);
    glfwPollEvents();
}

int main() {
    auto windowHandler = new WindowHandler(WIDTH, HEIGHT, WINDOW_TITLE);

    auto maze = new Maze(50, 50);

    /* While window is not closed */
    while (!glfwWindowShouldClose(windowHandler->window)) {
        updateWindow(windowHandler, maze);
    }
    exitWindow();
}