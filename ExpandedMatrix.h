#pragma once

#include "defs.h"
#include "Coordinate.h"
#include "Matrix.h"

#define DOOR 2

class ExpandedMatrix : public Matrix {
    private:
        int dimensionEx;
        int** matrixEx;
        Coordinate* coordConnections;
        int numConnections;

        void expandMatrix(); // makes initial matrix of 1s & 0s randomly
        void buildConnections();
        bool removeConnection(const Coordinate& c);

    public:
        ExpandedMatrix(int d);
        ~ExpandedMatrix();
        void removeCommonLoops();
        void printEx();
        void printConnections();
        int getCenterCoordEx();
        bool checkElementEx(const Coordinate& c, int compare) const;
};
