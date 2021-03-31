#ifndef NOTAMONGUS_SRC_POWERUP_H_
#define NOTAMONGUS_SRC_POWERUP_H_

#include "Color.h"
#include "Model.h"

#include <vector>
#include <utility>

class Spawn : public Model {
public:
    Spawn(const std::pair<float, float> &startPoint, Maze *maze);
    void spawnPowerups(int count);
    void spawnObstacles(int count);
    std::vector<Model *> powerups;
    std::vector<Model *> obstacles;

    void draw() override;
    void setCameraAndProjection(glm::mat4 camera,
                                glm::mat4 projection) override;
};

#endif //NOTAMONGUS_SRC_POWERUP_H_