#include "Maze.h"

Maze::Maze(int breadth, int length) {
    gridBreadth = breadth, gridLength = length;
    grid.resize(gridBreadth, std::vector<Cell *>(gridLength));
    for (auto &row: grid) {
        for (auto &col: row) {
            col = new Cell;
        }
    }

    /* Init RNG */
    std::random_device dev;
    std::mt19937 rngInit(dev());
    rng = rngInit;

    /* Init Maze */
    startPoint =
        {getRandom(0, gridBreadth - 1), getRandom(0, gridLength - 1)};
    createMaze();
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
        int chooseIdx = getRandom(0, wallQueue.size());
        auto chooseWall = wallQueue[wallQueue.size() - 1];

        /* Move to end before removing to get O(1) delete */
        std::swap(wallQueue[chooseIdx], wallQueue[wallQueue.size() - 1]);
        wallQueue.pop_back();

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
    }
}

void Maze::debug() {
    for (auto it1: grid) {
        for (auto it2: it1) {
            for (int i = 0; i < 4; i++) {
                std::cout << it2->wall[i];
            }
            std::cout << " ";
        }
        std::cout << "\n";
    }
}