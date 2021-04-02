#include "HUD.h"
#include "WindowHandler.h"
#include "Player.h"
#include "Imposter.h"
#include "Spawn.h"
#include "Model.h"
#include "Maze.h"
#include "Color.h"
#include "RayTracing.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

char *WINDOW_TITLE = "Not Among Us";
int WIDTH = 600;
int HEIGHT = 800;

int gridBreadth = 8, gridLength = 8;
int gameTime = 300;

void exitWindow(WindowHandler *windowHandler, HUD *hud) {
    while (!glfwWindowShouldClose(windowHandler->window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        hud->endgame();

        glfwSwapBuffers(windowHandler->window);
        glfwPollEvents();
    }
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
                  Imposter *imposter,
                  Spawn *spawn,
                  HUD *hud,
                  RayTracing *stencil) {
    /* Set window background to Black */
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_STENCIL_TEST);

    /* Move objects */
    player->move();
    imposter->move();

    auto camera =
        createCamera(player->currPoint.second, player->currPoint.first);
    glm::mat4 projection;

    projection = createProjection(2);

    /* Set view and projection */
    maze->setCameraAndProjection(camera,
                                 projection,
                                 glm::vec3(player->currPoint.second,
                                           player->currPoint.first,
                                           0.0f));
    player->setCameraAndProjection(camera, projection);
    imposter->setCameraAndProjection(camera, projection);
    spawn->setCameraAndProjection(camera, projection);
    stencil->setCameraAndProjection(camera, projection);

    /* Draw the maze */
    stencil->drawStencil(player->currPoint.first + player->objectWidth / 2,
                         player->currPoint.second + player->objectWidth / 2);
    player->draw();
    imposter->draw();
    spawn->draw();
    maze->draw();

    glDisable(GL_STENCIL_TEST);

    hud->draw();

    /* Check and call events and swap buffers */
    glfwSwapBuffers(windowHandler->window);
    glfwPollEvents();
}

int main() {
    auto windowHandler = new WindowHandler(WIDTH, HEIGHT, WINDOW_TITLE);

    auto maze = new Maze(gridBreadth, gridLength);

    std::pair<float, float> spawnPoint =
        {maze->getRandom(0, gridBreadth - 1),
         maze->getRandom(0, gridLength - 1)};
    std::pair<float, float> imposterPoint =
        {maze->getRandom(0, gridBreadth - 1),
         maze->getRandom(0, gridLength - 1)};

    auto spawn = new Spawn(spawnPoint, maze);
    auto player =
        new Player(maze->startPoint, maze, windowHandler->window, spawn);
    auto imposter = new Imposter(imposterPoint, maze, player);
    auto hud = new HUD(player, windowHandler->window);
    auto stencil = new RayTracing(maze);

    player->time = gameTime;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* While window is not closed */
    while (!glfwWindowShouldClose(windowHandler->window)
        and player->state == 0) {
        updateWindow(windowHandler,
                     maze,
                     player,
                     imposter,
                     spawn,
                     hud,
                     stencil);

        player->time -= 0.001f;
        if (player->time < 0.0f) {
            player->state = 1;
        }
    }

    exitWindow(windowHandler, hud);
};