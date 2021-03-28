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

    /* Init Maze */
    startPoint =
        {getRandom(0, gridBreadth - 1), getRandom(0, gridLength - 1)};
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
    std::string debugLine(gridLength, '-');
    std::cout << debugLine << "\n";
    for (auto row: grid) {
        for (auto col: row) {
            for (int i = 0; i < 4; i++) {
                std::cout << col->wall[i];
            }
            std::cout << " ";
        }
        std::cout << "\n";
    }
    std::cout << debugLine << "\n";
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
    std::map<std::pair<int, int>, int> hashVertex;
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
        auto vertex1 = (float) it.first.first / (float) gridBreadth;
        auto vertex2 = (float) it.first.second / (float) gridLength;

        /* Scale by 1.5 and send 0.75, 0 to -1, 1*/
        vertex1 *= 1.5, vertex2 *= 1.5;
        vertex1 -= 0.75, vertex2 -= 0.75;

        vertices.push_back(vertex1);
        vertices.push_back(vertex2);

        it.second = currIdx++;
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

void Maze::draw() {
    shaders->use();
    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
}

bool Maze::checkCollision(Model *otherModel) {
    /* TODO: Implement collision detection in direction */
    return false;
}