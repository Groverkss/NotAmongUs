#ifndef NOTAMONGUS_SRC_MODEL_H_
#define NOTAMONGUS_SRC_MODEL_H_

#include "Maze.h"
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

    unsigned int VAO;
    Shader *shaders;

    char *vertexShader = "resources/shaders/modelVertex.glsl";
    char *fragmentShader = "resources/shaders/modelFragment.glsl";

    void createIndices();
    unsigned int createVAO();
    Shader *createShaders();
protected:
    glm::mat4 modelTransform, viewTransform, projectionTransform;
    float objectWidth;
    Maze *maze;
public:
    /* Points in clockwise direction starting from top left */
    std::vector<std::pair<float, float>> points;
    std::vector<float> color;
    std::pair<float, float> currPoint;

    Model(const std::pair<float, float> &startPoint,
          const std::vector<float> &color,
          Maze *maze
    );

    void debug();
    void draw();

    void setCameraAndProjection(glm::mat4 camera, glm::mat4 projection);

    bool checkCollisionWithMaze();
};

#endif //NOTAMONGUS_SRC_MODEL_H_