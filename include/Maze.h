#ifndef NOTAMONGUS_SRC_MAZE_H_
#define NOTAMONGUS_SRC_MAZE_H_

#include <vector>
#include <utility>
#include <random>
#include <deque>
#include <iostream>

class Maze {
private:
    struct Cell {
        /* Left, Right, Up, Down */
        std::vector<bool> wall;
        bool taken = false;

        Cell() {
            wall.resize(4, true);
        }
    };
    struct Wall {
        Cell *cell[2];
        int wall[2];
        std::pair<int, int> points[2];
    };

    std::vector<std::vector<Cell *>> grid;
    std::mt19937 rng;

    int getRandom(int low, int high);
    void createMaze();
public:
    int gridLength, gridBreadth;
    std::pair<int, int> startPoint, endPoint;

    Maze(int breadth, int length);

    void debug();
};

#endif //NOTAMONGUS_SRC_MAZE_H_