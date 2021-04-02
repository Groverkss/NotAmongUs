#ifndef NOTAMONGUS_SRC_PLAYER_H_
#define NOTAMONGUS_SRC_PLAYER_H_

#include "Model.h"
#include "Color.h"
#include "Spawn.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <utility>
#include <string>

class Player : public Model {
private:
    void processInput();

    float moveSpeed;
    GLFWwindow *window;

    float horizontalSpeed, verticalSpeed;

    bool checkCollisionsWithSpawns();

    Model *endButton;

    Spawn *spawn;

    void checkButtonPress();
public:
    Player(const std::pair<float, float> &startPoint,
           Maze *maze,
           GLFWwindow *window,
           Spawn *spawn);

    void move();

    void draw() override;
    void setCameraAndProjection(glm::mat4 camera,
                                glm::mat4 projection) override;

    int score;
    bool task1, task2;
    double time;
    bool light;

    /* 0 --> Player, 1 --> Lost, 2 --> Won */
    int state;
};

#endif //NOTAMONGUS_SRC_PLAYER_H_