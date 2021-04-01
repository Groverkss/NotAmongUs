#include "Imposter.h"

Imposter::Imposter(const std::pair<float, float> &startPoint,
                   Maze *maze,
                   Player *player) : Model(startPoint, Color::RED, maze) {
    moveSpeed = 0.04f;
    this->player = player;

    auto x = getRandom(0, maze->gridBreadth - 1);
    auto y = getRandom(0, maze->gridLength - 1);
    killButton = new Model({x, y}, Color::WHITE, maze, 0.7f);
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

    /**
     * New model class with coordinates at player's
     * position to prevent object slicing
     **/
    auto tempPlayer =
        new Model(player->currPoint, Color::BLACK, maze, 0.2f, false);

    if (buttonPressed(tempPlayer)) {
        player->task1 = true;
        killButton->show = false;
        show = false;
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

    if (playerCollided(tempPlayer)) {
        player->state = 1;
    }

    delete tempPlayer;
}

bool Imposter::playerCollided(Model *tempPlayer) {
    return checkCollisionWithModel(tempPlayer);
}

bool Imposter::buttonPressed(Model *tempPlayer) {
    return killButton->checkCollisionWithModel(tempPlayer);
}

void Imposter::setCameraAndProjection(glm::mat4 camera, glm::mat4 projection) {
    Model::setCameraAndProjection(camera, projection);
    killButton->setCameraAndProjection(camera, projection);
}

void Imposter::draw() {
    Model::draw();
    killButton->draw();
}