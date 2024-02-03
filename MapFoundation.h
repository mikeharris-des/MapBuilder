#pragma once
#include "defs.h"
#include "Map.h"
#include "Coordinate.h"
#include "Matrix.h"

#define MF_DEBUG 0
#define MF_DEBUG_MAKEMAP 0

#define MAKE_SQUARE 1 //    if final map must be square (for rendering ui is easiest if square) ( const used in MapFoundation::makeMap() )

class Map;

class MapFoundation {

    friend class Map; // for Map constructor access to final map data

    protected:
        ExpandedMatrix* expandedMatrix;     // pointer to expandedMatrix instance passed in constructor & stored as member

        CoordinateArray nodes;                  // dynamic array maintained as a set of locations of 1s added when marked seen but not visited on matrixWalk from centre coordinate, is eventually visited

        CoordinateArray edges;                  // dynamic array maintained as a set of locations of 2s added when marked seen but not visited on matrixWalk from centre coordinate, is eventually visited

        // coordMost Bounds: used for cropping map
        int coordMostN;                     // smallest y coordinate value of visited 1s from expandedMatrix in matrixWalk -> most north node location regarding rows 0->n on nxn matrix, y >= 0
        int coordMostE;                     // largest x coordinate value of visited 1s from expandedMatrix in matrixWalk -> most east node location regarding columns 0->n on nxn matrix, x <= n-1
        int coordMostS;                     // largest y coordinate value of visited 1s from expandedMatrix in matrixWalk -> most south node location regarding rows 0->n on nxn matrix, y <= n-1
        int coordMostW;                     // smallest x coordinate value of visited 1s from expandedMatrix in matrixWalk -> most west node location regarding rows 0->n on nxn matrix, x >= 0


        MapFoundation(ExpandedMatrix* exMatrix);    // ctor is passed with pointer to exMatrix as it requires its map data (1s,2s) to make the final map

        ~MapFoundation();
        /*
        recursive function to traverse the matrix, starting at centre most coordinate visiting every 1 connected by atleast one 2. Handles dead ends using 'this->nodes' array member storing seen but
        not visited nodes. Moves in priority ordering of East (x+1), then North (y-1), then South (y+1), then West (x-1) from currCoord.
        */
        void matrixWalk(Coordinate& currCoord);

        /*
        takes the coordMost bounds and translates all of this MapFoundation matrix data cropped to a newly sized finalMap matrix object,
        without unnecessary building member variables or functions for abstraction
        */
        void makeMap();

        /*
        passing in y and x dimensions of the cropped map, allocates memory for the finalMap Map instance as a square matrix using the the largest of the dimensions
        and translate the corresponding mostCoord (mostSouth or mostEast) to reflect the square cropping.
        This ensures no map data is lost and also the squaring of the final map can be used consistently and easily for implementing a square gui display
        */
        void makeSquare(int sizeX, int sizeY);

        // takes the value at that coordinate and correctly translates it to the coordinate it should be at in the final map object after cropping and translating
        Coordinate translateCoordMatrixToMap(const Coordinate& currCoord);

        Coordinate* getNextPath();              // in matrixWalk, if a dead end is hit, will iterate through list of seen nodes and return the first node seen but not visited, otherwise will return null

        /*
        updates coordMost bounds based on currCoord x,y values respective to x: 0->n || west->east, y: 0->n || north->south. The coordMost bounds refers to the current node being higher (closer to y>=0)
        or lower (closer to y <= n-1) than any previously visted node on map in respect to its y coordinate. A similar coordMost bounds check and store occurs for the x bounds north and south. This is
        stored for a final cropping of the map and a translation to the top left (closest to 0,0) of the final map without removing map data.
        */
        void storeBounds(const Coordinate& currCoord);

        /*
        will iterate through the edge array on exiting matrixWalk, if there is any edges added & tf marked seen but not marked visited, they will be marked visited.
        missed edges happens due to bidirectional walking and because edges marked visited if they are walked over to next node. If that node in direction is marked visited wont walk there.
        */
        void checkEdges();
        bool addUniqueEdge(const Coordinate& currCoord);                // adds coordinates to the edge array if they are not already present, maintaining a set of edge coordinates existing in final map
        bool addUniqueNode(const Coordinate& currCoord);                // adds coordinates to the node array if they are not already present, maintaining a set of node coordinates existing in final map

        bool offBoundsFinalMap(const Coordinate& currCoord) const;      // returns true if this coordinate is out of bounds of the final map dimensions after cropping and translation

        void filterIsolated();                              // remove any cell not connectected to main branch (nodes or edges unvisited)

    private:
        // accessible by child inheritance

        void print() const;                                             // prints the original expandedMatrix map but with all the isolated nodes and edges removed after a MapFoundation instance is made
        void printBoundsAbsolute() const;                               // prints the mapFoundation instance cropped based on current coordMost bounds

        Matrix* finalMatrix;            // final matrix object to be copied by Map object

        CoordinateArray* finalEdges;    // translated edges values to be copied by Map object
        CoordinateArray* finalNodes;    // translated nodes values to be copied by Map object

};
