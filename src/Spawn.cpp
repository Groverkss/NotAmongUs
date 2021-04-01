#include "Spawn.h"

Spawn::Spawn(const std::pair<float, float> &startPoint, Maze *maze)
    : Model(startPoint, Color::GREEN, maze, 0.7f) {
    powerupScore = 100;
    obstacleScore = -20;
};

void Spawn::spawnPowerups(int count) {
    powerups.resize(count);
    for (auto &it: powerups) {
        auto x = getRandom(0, maze->gridBreadth - 1);
        auto y = getRandom(0, maze->gridLength - 1);

        it = new Model({x, y}, Color::BLUE, maze, 0.7f);
    }
    show = false;
}

void Spawn::spawnObstacles(int count) {
    obstacles.resize(count);
    for (auto &it: obstacles) {
        auto x = getRandom(0, maze->gridBreadth - 1);
        auto y = getRandom(0, maze->gridLength - 1);

        it = new Model({x, y}, Color::GREY, maze, 0.4f);
    }
    show = false;
}

void Spawn::draw() {
    Model::draw();

    for (auto &it: powerups) {
        it->draw();
    }

    for (auto &it: obstacles) {
        it->draw();
    }
}

void Spawn::setCameraAndProjection(glm::mat4 camera, glm::mat4 projection) {
    Model::setCameraAndProjection(camera, projection);
    for (auto &it: powerups) {
        it->setCameraAndProjection(camera, projection);
    }
    for (auto &it: obstacles) {
        it->setCameraAndProjection(camera, projection);
    }
}