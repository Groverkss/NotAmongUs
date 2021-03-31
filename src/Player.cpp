#include "Player.h"

Player::Player(const std::pair<float, float> &startPoint,
               Maze *maze,
               GLFWwindow *window,
               Spawn *spawn) : Model(startPoint, Color::YELLOW, maze) {
    this->window = window;
    horizontalSpeed = 0;
    verticalSpeed = 0;
    moveSpeed = 0.04f;
    this->spawn = spawn;
    state = "alive";
}

/* Move player model */
void Player::processInput() {
    horizontalSpeed = 0, verticalSpeed = 0;

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
        horizontalSpeed = -moveSpeed;
    } else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        horizontalSpeed = moveSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        verticalSpeed = moveSpeed;
    } else if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        verticalSpeed = -moveSpeed;
    }
}

void Player::move() {
    processInput();

    currPoint.first += verticalSpeed, currPoint.second += horizontalSpeed;
    auto obstacleCollide = checkCollisionsWithSpawns();

    if (obstacleCollide) {
        state = "dead";
    }

    if (!checkCollisionWithMaze() and !obstacleCollide) {
        auto moveTransform = glm::translate(glm::mat4(1.0f),
                                            glm::vec3(horizontalSpeed,
                                                      verticalSpeed,
                                                      0.0f));
        modelTransform = moveTransform * modelTransform;
    } else {
        currPoint.first -= verticalSpeed, currPoint.second -= horizontalSpeed;
    }
}

bool Player::checkCollisionsWithSpawns() {
    /* Check collisions with spawn */
    if (checkCollisionWithModel(spawn)) {
        spawn->spawnPowerups(3);
        spawn->spawnObstacles(20);
    }

    /* Check collisions with power ups */
    for (auto &it: spawn->powerups) {
        if (checkCollisionWithModel(it)) {
            /* TODO: Add score */

            /* Delete powerup */
            it->show = false;
        }
    }

    /* Check collisions with obstacles */
    for (auto &it: spawn->obstacles) {
        if (checkCollisionWithModel(it)) {
            return true;
        }
    }
}