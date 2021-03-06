#include "Maze.h"

Maze::Maze(int breadth, int length) {
    gridBreadth = breadth, gridLength = length;
    grid.resize(gridBreadth, std::vector<Cell *>(gridLength));

    int rowIdx = 0, colIdx = 0;
    for (auto &row: grid) {
        for (auto &col: row) {
            col = new Cell(rowIdx, colIdx);
            ++colIdx;
        }
        ++rowIdx;
        colIdx = 0;
    }

    /* Init RNG */
    std::random_device dev;
    std::mt19937 rngInit(dev());
    rng = rngInit;

    startPoint = {
        getRandom(0, gridBreadth - 1),
        getRandom(0, gridLength - 1)
    };

    createMaze();
    createIndices();
    VAO = createVAO();
    shaders = createShaders();
}

int Maze::getRandom(int low, int high) {
    std::uniform_int_distribution<std::mt19937::result_type> randGen(low, high);
    return randGen(rng);
}

/* Prims algorithm to create a maze */
void Maze::createMaze() {
    std::deque<Wall *> wallQueue;

    /* Left, Right, Up, Down */
    std::vector<std::pair<int, int>>
        directions = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    grid[startPoint.first][endPoint.second]->taken = true;

    /* Put all neighbouring cells of starting pos in container */
    for (int i = 0; i < 4; i++) {
        int x = startPoint.first + directions[i].first;
        int y = startPoint.second + directions[i].second;

        if (x < 0 or y < 0 or x >= gridBreadth or y >= gridLength) {
            continue;
        }

        auto newWall = new Wall();
        newWall->cell[0] = grid[startPoint.first][startPoint.second];
        newWall->cell[1] = grid[x][y];
        newWall->wall[0] = i;
        newWall->wall[1] = i xor 1;
        newWall->points[0] = startPoint;
        newWall->points[1] = {x, y};

        wallQueue.push_back(newWall);
    }

    while (!wallQueue.empty()) {
        /* Get a random wall */
        int chooseIdx = getRandom(0, wallQueue.size() - 1);
        auto chooseWall = wallQueue[chooseIdx];

        /* Move to end before removing to get O(1) delete */
        std::swap(wallQueue[chooseIdx], wallQueue[wallQueue.size() - 1]);
        wallQueue.pop_back();

        if (chooseWall->cell[0]->taken and chooseWall->cell[1]->taken) {
            delete chooseWall;
            continue;
        }

        /* Break partition joining the chosen cells */
        for (int wallIdx = 0; wallIdx < 2; wallIdx++) {
            auto cell = chooseWall->cell[wallIdx];
            cell->wall[chooseWall->wall[wallIdx]] = false;

            /* Visit the not taken cell */
            if (cell->taken) {
                continue;
            }
            cell->taken = true;

            /* Put all neighboring not visited cells of the cell in container */
            for (int i = 0; i < 4; i++) {
                auto x =
                    chooseWall->points[wallIdx].first + directions[i].first;
                auto y =
                    chooseWall->points[wallIdx].second + directions[i].second;

                if (x < 0 or y < 0 or x >= gridBreadth or y >= gridLength) {
                    continue;
                }

                auto newWall = new Wall();
                newWall->cell[0] = cell;
                newWall->cell[1] = grid[x][y];
                newWall->wall[0] = i;
                newWall->wall[1] = i xor 1;
                newWall->points[0] = chooseWall->points[wallIdx];
                newWall->points[1] = {x, y};

                wallQueue.push_back(newWall);
            }

            /* Set endpoint as last visited point */
            endPoint = chooseWall->points[wallIdx];
        }
        delete chooseWall;
    }
}

void Maze::debug() {
    for (auto it: vertices) {
        std::cout << it << " ";
    }
    std::cout << "\n";
}

void Maze::createIndices() {
    struct Direction {
        int x1, y1;
        int x2, y2;

        Direction(int x1, int y1, int x2, int y2) {
            this->x1 = x1, this->y1 = y1, this->x2 = x2, this->y2 = y2;
        }
    };

    std::vector<Direction>
        directions =
        {
            Direction(1, 0, 0, 0),
            Direction(0, 1, 1, 1),
            Direction(0, 0, 0, 1),
            Direction(1, 1, 1, 0),
        };

    /* Get all unique vertices */
    for (auto row: grid) {
        for (auto col: row) {
            std::pair<int, int> curr = {col->row, col->col};
            for (int i = 0; i < directions.size(); i++) {
                if (!col->wall[i]) {
                    continue;
                }

                auto it = directions[i];
                hashVertex[{curr.first + it.x1, curr.second + it.y1}];
                hashVertex[{curr.first + it.x2, curr.second + it.y2}];
            }
        }
    }

    /* Assign index to each unique vertex and store it in vertex buffer */
    int currIdx = 0;

    /* Everything here should be in the model matrix */
    for (auto &it: hashVertex) {
        auto vertexX = (float) it.first.first;
        auto vertexY = (float) it.first.second;

        vertices.push_back(vertexX);
        vertices.push_back(vertexY);

        it.second = currIdx++;

        invHashVertex[it.second] = it.first;
    }

    /* Store line indices */
    for (auto row: grid) {
        for (auto col: row) {
            std::pair<int, int> curr = {col->row, col->col};
            for (int i = 0; i < directions.size(); i++) {
                if (!col->wall[i]) {
                    continue;
                }

                auto it = directions[i];
                indices.push_back(hashVertex[{curr.first + it.x1,
                                              curr.second + it.y1}]);
                indices.push_back(hashVertex[{curr.first + it.x2,
                                              curr.second + it.y2}]);
            }
        }
    }
}

unsigned int Maze::createVAO() {
    /* Create VAO */
    unsigned int newVAO;
    glGenVertexArrays(1, &newVAO);
    glBindVertexArray(newVAO);

    /* Set vertex buffer */
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(float),
                 vertices.data(),
                 GL_STATIC_DRAW);

    /* Set element buffer */
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned int),
                 indices.data(),
                 GL_STATIC_DRAW);

    /* Set vertex pointers */
    glVertexAttribPointer(0,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          2 * sizeof(float),
                          (void *) 0);
    glEnableVertexAttribArray(0);

    return newVAO;
}

Shader *Maze::createShaders() {
    auto newShader = new Shader(vertexShader, fragmentShader);
    return newShader;
}

void Maze::setCameraAndProjection(glm::mat4 camera,
                                  glm::mat4 projection,
                                  glm::vec3 lightPosition) {
    viewTransform = camera;
    projectionTransform = projection;
    this->lightPosition = lightPosition;
}

void Maze::draw() {
    shaders->use();
    shaders->setMat4("view", viewTransform);
    shaders->setMat4("projection", projectionTransform);
    shaders->setVec3("lightpos", lightPosition);
    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}