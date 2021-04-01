#ifndef NOTAMONGUS_SRC_IMPOSTER_H_
#define NOTAMONGUS_SRC_IMPOSTER_H_

#include "Color.h"
#include "Model.h"
#include "Player.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <utility>
#include <cmath>

class Imposter : public Model {
private:
    const int INF = 1e9;

    std::pair<float, float> decideSpeed(bool first = false,
                                        bool second = false);
    float moveSpeed;
    Player *player;
    Model *killButton;
    bool buttonPressed(Model *tempPlayer);
    bool playerCollided(Model *tempPlayer);

    std::vector<std::vector<int>> distance;
    std::vector<std::vector<std::pair<int, int>>> parent;
    void fillDistances();
    std::pair<float, float> toMove;
public:
    Imposter(const std::pair<float, float> &startPoint,
             Maze *maze,
             Player *player);

    void move();
    void draw() override;
    void setCameraAndProjection(glm::mat4 camera,
                                glm::mat4 projection) override;
};

#endif //NOTAMONGUS_SRC_IMPOSTER_H_