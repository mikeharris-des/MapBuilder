#pragma once

#include "defs.h"    // for time
#include "Coordinate.h"

#define ALLOW_DEFAULTS 1 // 1 to allow default elements at
#define ROOM 1

class Matrix {
    protected:
        int dimension;
        int** matrix; // 2d array of coordinates?

    private:
        void buildMatrix(); // makes initial matrix of 1s & 0s randomly
        void buildMatrix(const Matrix& m); // copies to initial matrix of 1s & 0s from the given matrix

    public:
        Matrix(int d = 3);
        Matrix(const Matrix& m); // cpy ctor
        ~Matrix();
        bool defaultElement(int x, int y) const; // inserts valid elements (1s) at default coordinates

        int getCenterCoord() const;
        bool checkElement(const Coordinate& coord, int compare) const;

        bool offBounds(const Coordinate& coord) const;

        void print() const;

};
