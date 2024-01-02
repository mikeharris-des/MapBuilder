#pragma once

#include "defs.h"
#include "Matrix.h"

#define ROOM 1
#define DOOR 2

class ExpandedMatrix : public Matrix {
    private:
        int dimensionEx;
        int** matrixEx;
        // int numElementsEx;
        void expandMatrix(); // makes initial matrix of 1s & 0s randomly
        void buildConnections();

    public:
        ExpandedMatrix(int d);
        ~ExpandedMatrix();
        void removeCommonLoops();
        void printEx();
        int getCenterCoordEx();
        bool getCoordEx(int currX, int currY, Direction dir, int* coord);
        bool checkElementEx(int xCheck, int yCheck, int compare);
};
