#pragma once

#include "defs.h"    // for time seed in rand()
#include "Coordinate.h"

#define ROOM 1            // the binary matrix, rooms are code for 1s  -> dynamically allocated!

class Matrix {
    protected:
        int dimension;  // the dimension used for the square matrix
        int* matrix;    // 1 dimensional bitstring of binary numbers to represent matrix

    private:
        void buildMatrix(); // makes initial matrix of 1s & 0s randomly
        void buildMatrix(const Matrix& m); // cpy ctor utility function similar to buildMatrix but with input

    public:
        Matrix(int d = 3);       // dimension defaulted to 3
        Matrix(const Matrix& m); // cpy ctor
        ~Matrix();

        bool defaultElement(int x, int y) const; // inserts valid elements (1s) at predetermined default coordinates (centre and neighbors will always be 1)

        int getCenterCoord() const;

        bool checkElement(const Coordinate& coord, int compare) const;  // returns true if at that coodinate of matrix the value of the cell is compare

        bool offBounds(const Coordinate& coord) const;                  // bounds check of matrix, returns true if out of bounds:  if( offBounds() )

        void print() const;                                             // print the matrix formatted

};
