#include "Imposter.h"

Imposter::Imposter(const std::pair<float, float> &startPoint, Maze *maze)
    : Model(startPoint, Color::RED, maze) {
    moveSpeed = 0.04f;
}

std::pair<float, float> Imposter::decideSpeed() {
    /* TODO: Add path finding */
    return {getRandom(-1, 1) * moveSpeed,
            getRandom(-1, 1) * moveSpeed};
}

void Imposter::move() {
    if (!show) {
        return;
    }

    auto movingSpeed = decideSpeed();

    currPoint.first += movingSpeed.first;
    currPoint.second += movingSpeed.second;
    if (!checkCollisionWithMaze()) {
        auto moveTransform = glm::translate(glm::mat4(1.0f),
                                            glm::vec3(movingSpeed.second,
                                                      movingSpeed.first,
                                                      0.0f));
        modelTransform = moveTransform * modelTransform;
    } else {
        currPoint.first -= movingSpeed.first;
        currPoint.second -= movingSpeed.second;
    }
}