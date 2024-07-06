#pragma once

#include "defs.h"
#include "Coordinate.h"

class Matrix {
    protected:
        int* matrix;    // 1 dimensional array of numbers to represent matrix

    private:
        void buildMatrix(); // makes initial matrix of 1s & 0s randomly
        void buildMatrix(const Matrix& m); // cpy ctor utility function similar to buildMatrix but with input

    public:
        int dx;  // the x dimension of matrix
        int dy;  // the y dimension of matrix

        Matrix(int d = 3);       // constructor for making square matrix, d is minimum
        Matrix(int x, int y);    // constructor for making matrix x*y dimension

        Matrix(Matrix* m); // cpy ctor
        Matrix(const Matrix& m); // cpy ctor

        ~Matrix();

        bool defaultElement(int x, int y) const; // inserts valid elements (1s) at predetermined default coordinates (centre and neighbors will always be 1)

        int getCenterCoord() const;                                     // return center coordinate of matrix ( int xdimension / 2 )

        int getCell(int x, int y) const;                                // getter for cell value of matrix at specified coordinates

        void setCell(int x, int y, int value);                              // sets the coordinates of matrix to value if in bounds

        bool checkElement(const Coordinate& coord, int compare) const;  // returns true if at that coodinate of matrix the value of the cell is compare

        bool offBounds(int x, int y) const;                             // if the x, y is out of bounds for this matrix returns true -> if( offBounds() )
        bool offBounds(const Coordinate& coord) const;                  // boundary check of matrix, returns true if out of bounds:  if( offBounds() )

        void print() const;                                             // print the matrix formatted
};


class ExpandedMatrix : public Matrix {
    private:
        int exDx;              // dimension x of this matrix expanded from the base matrix
        int exDy;              // dimension y of this matrix expanded from the base matrix

        int* matrixExpanded;                // 1 dimensional array of 0s,1s,2s integers to represent expanded matrix  -> dynamically allocated

        CoordinateArray edges;              // dynamically allocated array of coordinate objects storing location of all 2s in the 'array' array representing edges between nodes of the graph

        void expandMatrix();                            // from inherited matrix instance, expands the matrix filling every cell between the original cells with a zero  eg 1101 -> 1010001

        void addEdges();                                 // from the now expanded matrix of 1s & 0s, will replace a 0 with a 2 if it is seperated by atleast two 1s    eg 1010001 -> 1210001
        bool removeEdge(const Coordinate& c);           // utility function in removeCommonLoops to remove a 2 at a coordinate if specified, removes from member array aswell

    public:
        ExpandedMatrix(int d = 3);                          // initialize this object with a minimum default dimension passed to inherited instance, 3x3 -> 5x5 expanded
        ~ExpandedMatrix();                                  // dtor

        void removeCommonLoops();                           // must be called explicitly after instantiating ExpandedMatrix obj, removes 1 random edge to nodes with 4 edges, makes a realistic looking map
        void markCellVisited(const Coordinate& c);          // increments matrix cell value with the VISITED constant to indicate it has been visited
        void clearCell(const Coordinate& c);                // changes cell value to 0 at that coordinate regardless of its current value

        int getxDimensionExpandedMatrix() const{return this->exDx;} // getter for dimension x of Expanded matrix
        int getyDimensionExpandedMatrix() const{return this->exDy;} // getter for dimension x of Expanded matrix

        int getNumCells() const;                            // getter for number of cells in this matrix

        int getCenterCoordExpanded() const;                 // get the absolute centre coordinate of this expanded matrix ( y=x in this case therefore the int return )
        int get(const Coordinate& c) const;                 // getter of cell value for specified coordinate
        int get(int x, int y) const;                        // getter of cell value for specified coordinate using its direct x,y values ( x,y is assumed zero index ie x: 0,1,2,3,4  for 5x5 matrix )

        bool checkElementExpanded(const Coordinate& c, int compare) const;      // returns true if at that coordinate, the cell value is == compare
        bool offBoundsExpanded(const Coordinate& c) const;                              // returns true if that coordinate is off bounds for the expanded matrix : if( offBounds() )

        void printExpanded() const;                     // print the expanded matrix cell values formatted to visualize the map
        void printEdges() const;                  // print the array of coordinates of all 2 locations
};
