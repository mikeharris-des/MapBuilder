#pragma once

#include "defs.h"
#include "Coordinate.h"
#include "Matrix.h"

#define DOOR 2
#define VISITED 2
#define EM_DEBUG 0
#define OFF_BOUNDS -1 // return int get if off bounds 

class ExpandedMatrix : public Matrix {
    private:
        int dimensionExpanded;
        int** matrixExpanded;

        int numConnections;
        Coordinate* coordConnections;

        void expandMatrix(); // makes initial matrix of 1s & 0s randomly
        void buildConnections();
        bool removeConnection(const Coordinate& c);

    public:
        ExpandedMatrix(int d = 3);
        ~ExpandedMatrix();
        void removeCommonLoops();
        void markCellVisited(const Coordinate& c);
        void clearCell(const Coordinate& c);

        int getDimension() const;
        int getNumCells() const;

        int getCenterCoordExpanded() const;
        int get(const Coordinate& c) const;
        int get(int x, int y) const;

        bool checkElementExpanded(const Coordinate& c, int compare) const;
        bool offBounds(const Coordinate& c) const;

        void printExpanded() const;
        void printConnections() const;
};
