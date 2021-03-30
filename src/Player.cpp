#include "Player.h"

Player::Player(const std::pair<float, float> &startPoint,
               Maze *maze,
               GLFWwindow *window) : Model(startPoint, Color::YELLOW, maze) {
    this->window = window;
    horizontalSpeed = 0;
    verticalSpeed = 0;
    moveSpeed = 0.04f;
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
    if (!checkCollisionWithMaze()) {
        auto moveTransform = glm::translate(glm::mat4(1.0f),
                                            glm::vec3(horizontalSpeed,
                                                      verticalSpeed,
                                                      0.0f));
        modelTransform = moveTransform * modelTransform;
    } else {
        currPoint.first -= verticalSpeed, currPoint.second -= horizontalSpeed;
    }
}