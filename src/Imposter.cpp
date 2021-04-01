#include "Imposter.h"

Imposter::Imposter(const std::pair<float, float> &startPoint,
                   Maze *maze,
                   Player *player) : Model(startPoint, Color::RED, maze) {
    moveSpeed = 0.04f;
    this->player = player;

    auto x = getRandom(0, maze->gridBreadth - 1);
    auto y = getRandom(0, maze->gridLength - 1);
    killButton = new Model({x, y}, Color::WHITE, maze, 0.7f);

    fillDistances();
}

void Imposter::fillDistances() {
    auto xMax = maze->gridBreadth;
    auto yMax = maze->gridLength;

    distance.resize(xMax * yMax, std::vector<int>(xMax * yMax, INF));
    parent.resize(xMax * yMax, std::vector<std::pair<int, int>>(xMax * yMax));

    for (int i = 0; i < xMax; i++) {
        for (int j = 0; j < yMax; j++) {
            auto index = i * yMax + j;
            distance[index][index] = 0;
            for (int k = 0; k < 4; k++) {
                auto cell = maze->grid[i][j];
                if (cell->wall[k]) {
                    continue;
                }

                int newIdx;
                switch (k) {
                    case 0:newIdx = i * yMax + (j - 1);
                        break;
                    case 1:newIdx = i * yMax + (j + 1);
                        break;
                    case 2:newIdx = (i - 1) * yMax + j;
                        break;
                    case 3:newIdx = (i + 1) * yMax + j;
                        break;
                }
                distance[index][newIdx] = 1;
                parent[index][newIdx] =
                    {(newIdx / yMax) - i, (newIdx % yMax) - j};
            }
        }
    }

    /* Floyd Washell */
    for (int k = 0; k < xMax * yMax; k++) {
        for (int i = 0; i < xMax * yMax; i++) {
            for (int j = 0; j < xMax * yMax; j++) {
                if (distance[i][j] > distance[i][k] + distance[k][j]) {
                    distance[i][j] = distance[i][k] + distance[k][j];
                    parent[i][j] = parent[i][k];
                }
            }
        }
    }
}

std::pair<float, float> Imposter::decideSpeed() {
    std::pair<int, int> playerPos = player->currPoint;
    std::pair<int, int> impostPos = currPoint;

    auto xMax = maze->gridBreadth;
    auto yMax = maze->gridLength;

    auto indexP = playerPos.first * yMax + playerPos.second;
    auto indexI = impostPos.first * yMax + impostPos.second;

    std::pair<int, int> movement;

    if (indexP != indexI) {
        movement = parent[indexI][indexP];
    } else {
        movement = {0, 0};
    }

    return {movement.first * moveSpeed, movement.second * moveSpeed};
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