#include "WindowHandler.h"
#include "Player.h"
#include "Model.h"
#include "Maze.h"
#include "Color.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

char *WINDOW_TITLE = "Not Among Us";
int WIDTH = 600;
int HEIGHT = 800;

void exitWindow() {
    glfwTerminate();
}

void updateWindow(WindowHandler *windowHandler,
                  Maze *maze,
                  Player *player,
                  Model *imposter) {
    /* Set window background to Black */
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    /* Move objects */
    player->move();

    /* Draw the maze */
    maze->draw();
    player->draw();
    imposter->draw();

    /* Redundant call which i dont know why needs to be called */
    player->draw();

    /* Check and call events and swap buffers */
    glfwSwapBuffers(windowHandler->window);
    glfwPollEvents();
}

int main() {
    auto windowHandler = new WindowHandler(WIDTH, HEIGHT, WINDOW_TITLE);

    auto maze = new Maze(30, 30);

    auto player = new Player(maze->startPoint, maze, windowHandler->window);
    auto imposter = new Model(maze->endPoint, Color::RED, maze);

    /* While window is not closed */
    while (!glfwWindowShouldClose(windowHandler->window)) {
        updateWindow(windowHandler, maze, player, imposter);
    }
    exitWindow();
}