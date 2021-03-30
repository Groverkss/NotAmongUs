#ifndef NOTAMONGUS_SRC_PLAYER_H_
#define NOTAMONGUS_SRC_PLAYER_H_

#include "Model.h"
#include "Color.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <utility>

class Player : public Model {
private:
    void processInput();

    float moveSpeed;
    GLFWwindow *window;

    float horizontalSpeed, verticalSpeed;
public:
    Player(const std::pair<float, float> &startPoint,
           Maze *maze,
           GLFWwindow *window);

    void move();
};

#endif //NOTAMONGUS_SRC_PLAYER_H_