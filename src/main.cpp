#include "WindowHandler.h"
#include "Maze.h"
#include "Model.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

char *WINDOW_TITLE = "Not Among Us";
int WIDTH = 600;
int HEIGHT = 800;

void exitWindow() {
    glfwTerminate();
}

void updateWindow(WindowHandler *windowHandler, Maze *maze, Model *obj) {
    /* Set window background to Black */
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    /* Draw the maze */
    maze->draw();
    obj->draw();

    /* Check and call events and swap buffers */
    glfwSwapBuffers(windowHandler->window);
    glfwPollEvents();
}

int main() {
    auto windowHandler = new WindowHandler(WIDTH, HEIGHT, WINDOW_TITLE);

    auto maze = new Maze(30, 30);

    std::vector<std::pair<float, float>> points = {
        {0.0f, 0.0f},
        {0.0f, 1.0f},
        {1.0f, 1.0f},
        {1.0f, 0.0f}
    };
    std::vector<float> color = {0.0f, 0.0f, 0.0f};

    auto obj = new Model(points, color);

    /* While window is not closed */
    while (!glfwWindowShouldClose(windowHandler->window)) {
        updateWindow(windowHandler, maze, obj);
    }
    exitWindow();
}