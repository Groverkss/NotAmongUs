#ifndef NOTAMONGUS_SRC_MAZE_H_
#define NOTAMONGUS_SRC_MAZE_H_

#include "Shaders.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <utility>
#include <random>
#include <deque>
#include <iostream>
#include <map>

class Maze {
private:
    struct Cell {
        int row, col;
        /* Left, Right, Up, Down */
        std::vector<bool> wall;
        bool taken = false;

        Cell(int row, int col) {
            wall.resize(4, true);
            this->row = row, this->col = col;
        }
    };
    struct Wall {
        Cell *cell[2];
        int wall[2];
        std::pair<int, int> points[2];
    };

    std::mt19937 rng;

    unsigned int VAO;
    Shader *shaders;

    char *vertexShader = "resources/shaders/mazeVertex.glsl";
    char *fragmentShader = "resources/shaders/mazeFragment.glsl";

    void createMaze();
    void createIndices();
    unsigned int createVAO();
    Shader *createShaders();
protected:
    glm::mat4 viewTransform, projectionTransform;
public:
    std::vector<std::vector<Cell *>> grid;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    int gridBreadth, gridLength;
    std::pair<int, int> startPoint, endPoint;

    std::map<std::pair<int, int>, int> hashVertex;
    std::map<int, std::pair<int, int>> invHashVertex;

    Maze(int breadth, int length);

    void debug();
    void draw();

    void setCameraAndProjection(glm::mat4 camera, glm::mat4 projection);
    int getRandom(int low, int high);
};

#endif //NOTAMONGUS_SRC_MAZE_H_