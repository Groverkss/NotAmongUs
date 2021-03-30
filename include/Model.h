#ifndef NOTAMONGUS_SRC_MODEL_H_
#define NOTAMONGUS_SRC_MODEL_H_

#include "Shaders.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <utility>
#include <iostream>

class Model {
private:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    glm::mat4 modelTransform, viewTransform, projectionTransform;

    unsigned int VAO;
    Shader *shaders;

    char *vertexShader = "resources/shaders/modelVertex.glsl";
    char *fragmentShader = "resources/shaders/modelFragment.glsl";

    std::pair<float, float> currPoint;

    void createIndices();
    unsigned int createVAO();
    Shader *createShaders();
public:
    /* Points in clockwise direction starting from top left */
    std::vector<std::pair<float, float>> points;
    std::vector<float> color;

    Model(std::pair<float, float> startPoint,
          const std::vector<float> &color
    );

    void debug();
    void draw();

    bool checkCollision(Model *otherModel);
};

#endif //NOTAMONGUS_SRC_MODEL_H_
