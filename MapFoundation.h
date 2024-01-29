#pragma once
#include "defs.h"
#include "Coordinate.h"
#include "ExpandedMatrix.h"
#include "Map.h"

#define MF_DEBUG 0
#define MF_DEBUG_MAKEMAP 0

#define MAKE_SQUARE 1 //    if final map must be square (for rendering ui is easiest if square) ( const used in MapFoundation::makeMap() )

class Map; // pre declare Map class to enable friending

/*
MapFoundation crops ExpandedMatrix with only navigable rooms and doors. Inheritance created issues with instantiating, ptr to ExpandedMatrix must be passed in ctor
*/
class MapFoundation {

    friend class Map; // for constructor access to member data (inheritance created issues with other objects makes simple the access of abstracted members without inheritance)

    private:
        ExpandedMatrix* expandedMatrix;     // pointer to expandedMatrix instance passed in constructor & stored as member

        Coordinate* rooms;                  // dynamic array maintained as a set of locations of 1s added when marked seen but not visited on matrixWalk from centre coordinate, is eventually visited
        int numRooms;                       // size of rooms array

        Coordinate* doors;                  // dynamic array maintained as a set of locations of 2s added when marked seen but not visited on matrixWalk from centre coordinate, is eventually visited
        int numDoors;                       // size of doors array

        // coordMost Bounds: used for cropping map
        int coordMostN;                     // smallest y coordinate value of visited 1s from expandedMatrix in matrixWalk -> most north room location regarding rows 0->n on nxn matrix, y >= 0
        int coordMostE;                     // largest x coordinate value of visited 1s from expandedMatrix in matrixWalk -> most east room location regarding columns 0->n on nxn matrix, x <= n-1
        int coordMostS;                     // largest y coordinate value of visited 1s from expandedMatrix in matrixWalk -> most south room location regarding rows 0->n on nxn matrix, y <= n-1
        int coordMostW;                     // smallest x coordinate value of visited 1s from expandedMatrix in matrixWalk -> most west room location regarding rows 0->n on nxn matrix, x >= 0

        Map* finalMap;                      // pointer to final cropped & usable Map object created from MapFoundation and its passed ExpandedMatrix instance data

        /*
        recursive function to traverse the matrix, starting at centre most coordinate visiting every 1 connected by atleast one 2. Handles dead ends using 'this->rooms' array member storing seen but
        not visited rooms. Moves in priority ordering of East (x+1), then North (y-1), then South (y+1), then West (x-1) from currCoord.
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


    public:
        MapFoundation(ExpandedMatrix* exMatrix);    // ctor is passed with pointer to exMatrix as it requires its map data (1s,2s) to make the final map
        ~MapFoundation();

        Coordinate* getNextPath();              // in matrixWalk, if a dead end is hit, will iterate through list of seen rooms and return the first room seen but not visited, otherwise will return null

        /*
        updates coordMost bounds based on currCoord x,y values respective to x: 0->n || west->east, y: 0->n || north->south. The coordMost bounds refers to the current room being higher (closer to y>=0)
        or lower (closer to y <= n-1) than any previously visted room on map in respect to its y coordinate. A similar coordMost bounds check and store occurs for the x bounds north and south. This is
        stored for a final cropping of the map and a translation to the top left (closest to 0,0) of the final map without removing map data.
        */
        void storeBounds(const Coordinate& currCoord);

        /*
        will iterate through the door array on exiting matrixWalk, if there is any doors added & tf marked seen but not marked visited, they will be marked visited.
        missed doors happens due to bidirectional walking and because doors marked visited if they are walked over to next room. If that room in direction is marked visited wont walk there.
        */
        void checkDoors();
        bool addUniqueDoor(const Coordinate& currCoord);                // adds coordinates to the door array if they are not already present, maintaining a set of door coordinates existing in final map
        bool addUniqueRoom(const Coordinate& currCoord);                // adds coordinates to the room array if they are not already present, maintaining a set of room coordinates existing in final map

        void print() const;                                             // prints the original expandedMatrix map but with all the isolated rooms and doors removed after a MapFoundation instance is made
        void printBoundsAbsolute() const;                               // prints the mapFoundation instance cropped based on current coordMost bounds
        bool offBoundsFinalMap(const Coordinate& currCoord) const;      // returns true if this coordinate is out of bounds of the final map dimensions after cropping and translation

        void filterIsolated();                              // remove any cell not connectected to main branch (rooms or doors unvisited)

};
