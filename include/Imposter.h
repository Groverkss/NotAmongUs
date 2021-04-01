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

class Imposter : public Model {
private:
    std::pair<float, float> decideSpeed();
    float moveSpeed;
    Player *player;
    Model *killButton;
    bool buttonPressed(Model *tempPlayer);
    bool playerCollided(Model *tempPlayer);
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