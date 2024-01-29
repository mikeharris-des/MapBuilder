#pragma once

#include "defs.h"
#include "Coordinate.h"
#include "Matrix.h"

#define DOOR 2         // int value for connection between two 1 elements in matrix, dubbed 'Door'
#define VISITED 2      // visited marking increment for walking the expanded matrix
#define EM_DEBUG 0     // debug for this class
#define OFF_BOUNDS -1  // default int return if off bounds

class ExpandedMatrix : public Matrix {
    private:
        int dimensionExpanded;              // dimension of this matrix expanded from the base matrix = baseMatrix.dimension + ( baseMatrix.dimension - 1 )
        int* matrixExpanded;                // 1 dimensional 'bitstring' of 0s,1s,2s integers to represent expanded matrix  -> dynamically allocated!

        int numConnections;                 // size of coordConnections array representing number of 1s connected with 2s or the number of doors in the expanded matrix
        Coordinate* coordConnections;       // dynamically allocated array of coordinate objects storing location of all 2s in the 'bitstring' array

        void expandMatrix();                            // from inherited matrix instance, expands the matrix filling every cell between the original cells with a zero  eg 1101 -> 1010001
        void buildConnections();                        // from the now expanded matrix of 1s & 0s, will replace a 0 with a 2 if it is seperated by atleast two 1s    eg 1010001 -> 1210001
        bool removeConnection(const Coordinate& c);     // utility function in removeCommonLoops to remove a 2 at a coordinate if specified, removes from member array aswell

    public:
        ExpandedMatrix(int d = 3);                          // initialize this object with a minimum default dimension passed to inherited instance, 3x3 -> 5x5 expanded
        ~ExpandedMatrix();                                  // dtor

        void removeCommonLoops();                           // must be called explicitly after instantiating ExpandedMatrix obj, removes 1 random door to rooms with 4 doors, makes a realistic looking map
        void markCellVisited(const Coordinate& c);          // increments matrix cell value with the VISITED constant to indicate it has been visited
        void clearCell(const Coordinate& c);                // changes cell value to 0 at that coordinate regardless of its current value

        int getDimension() const;                           // getter for dimensionExpanded
        int getNumCells() const;                            // getter for dimensionExpanded*dimensionExpanded or number of cells in this matrix

        int getCenterCoordExpanded() const;                 // get the absolute centre coordinate of this expanded matrix ( y=x in this case therefore the int return )
        int get(const Coordinate& c) const;                 // getter of cell value for specified coordinate
        int get(int x, int y) const;                        // getter of cell value for specified coordinate using its direct x,y values ( x,y is assumed zero index ie x: 0,1,2,3,4  for 5x5 matrix )

        bool checkElementExpanded(const Coordinate& c, int compare) const;      // returns true if at that coordinate, the cell value is == compare
        bool offBounds(const Coordinate& c) const;                              // returns true if that coordinate is off bounds for the expanded matrix : if( offBounds() )

        void printExpanded() const;                     // print the expanded matrix cell values formatted to visualize the map
        void printConnections() const;                  // print the array of coordinates of all 2 locations
};
