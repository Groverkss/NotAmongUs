#ifndef NOTAMONGUS_SRC_IMPOSTER_H_
#define NOTAMONGUS_SRC_IMPOSTER_H_

#include "Color.h"
#include "Model.h"

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
public:
    Imposter(const std::pair<float, float> &startPoint, Maze *maze);

    void move();
};

#endif //NOTAMONGUS_SRC_IMPOSTER_H_