#include "WindowHandler.h"
#include "Player.h"
#include "Model.h"
#include "Maze.h"
#include "Color.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

char *WINDOW_TITLE = "Not Among Us";
int WIDTH = 600;
int HEIGHT = 800;

int gridBreadth = 30, gridLength = 30;

void exitWindow() {
    glfwTerminate();
}

glm::mat4 createCamera(float x = (float) gridBreadth / 2,
                       float y = (float) gridLength / 2) {
    return glm::lookAt(
        glm::vec3(x, y, 1.0f),
        glm::vec3(x, y, 0.0f),
        glm::vec3(0, 1, 0)
    );
}

glm::mat4 createProjection(float zoom) {
    return glm::ortho((float) -gridLength / zoom,
                      (float) gridLength / zoom,
                      (float) -gridBreadth / zoom,
                      (float) gridBreadth / zoom);
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

    auto camera =
        createCamera(player->currPoint.second, player->currPoint.first);
    auto projection = createProjection(8);

    /* Set view and projection */
    maze->setCameraAndProjection(camera, projection);
    player->setCameraAndProjection(camera, projection);
    imposter->setCameraAndProjection(camera, projection);

    /* Draw the maze */
    maze->draw();
    player->draw();
    imposter->draw();

    /* Check and call events and swap buffers */
    glfwSwapBuffers(windowHandler->window);
    glfwPollEvents();
}

int main() {
    auto windowHandler = new WindowHandler(WIDTH, HEIGHT, WINDOW_TITLE);

    auto maze = new Maze(gridBreadth, gridLength);

    auto player = new Player(maze->startPoint, maze, windowHandler->window);
    auto imposter = new Model(maze->endPoint, Color::RED, maze);

    /* While window is not closed */
    while (!glfwWindowShouldClose(windowHandler->window)) {
        updateWindow(windowHandler, maze, player, imposter);
    }
    exitWindow();
}