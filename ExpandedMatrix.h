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
        ExpandedMatrix(int d = 3);
        ~ExpandedMatrix();
        void removeCommonLoops();
        void modCoordElement(const Coordinate& c, int increment);

        void printEx();
        void printConnections();

        int getDimension();
        int getMatrixSize();

        int getCenterCoordEx();
        int get(const Coordinate& c);
        int get(int x, int y);

        bool checkElementEx(const Coordinate& c, int compare) const;
};
