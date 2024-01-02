#pragma once

#include "defs.h"    // for time

#define ALLOW_DEFAULTS 1 // 1 to allow default elements at
enum Direction { EAST, NORTH, WEST, SOUTH, DIRECTION_COUNT };

class Matrix {
    protected:
        int dimension;
        int** matrix;
        int numElements;
        int** coordElements; //[MAX_ARR][2]

    private:
        // int a[DIM][DIM];
        void buildMatrix(); // makes initial matrix of 1s & 0s randomly

    public:
        Matrix(int d = 3);
        ~Matrix();
        bool defaultElement(int y, int x); // inserts valid elements (1s) at default coordinates

        int getCenterCoord();
        bool getCoord(int currX, int currY, Direction dir, int* coord);
        bool checkElement(int xCheck, int yCheck, int compare);

        string directionToString(Direction dir);
        void print();

};
