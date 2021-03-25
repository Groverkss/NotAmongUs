#ifndef NOTAMONGUS_SRC_MAZE_H_
#define NOTAMONGUS_SRC_MAZE_H_

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

    std::vector<std::vector<Cell *>> grid;
    std::mt19937 rng;

    std::vector<float> vertices;
    std::vector<int> indices;

    int getRandom(int low, int high);
    void createMaze();
    void createIndices();
public:
    int gridBreadth, gridLength;
    std::pair<int, int> startPoint, endPoint;

    Maze(int breadth, int length);

    void debug();
};

#endif //NOTAMONGUS_SRC_MAZE_H_