#pragma once
#include "defs.h"               // includes cstdlib for srand and ctime for time
#include "MapFoundation.h"      // object bridges matrix to full map object
#include "Matrix.h"             // base matrix and expanded matrix
#include "Coordinate.h"         // 2d coordinate object, PARAMETER ORDERING WILL ALWAYS BE X THEN Y: (x,y)

#define MAP_DEBUG 0                     // debug for only map class 0 == off

#define DEFAULT_DIMENSION 3             // if map obj is instantiated with negative integer dimensions or no dimensions is the default
#define REMOVE_COMMON_LOOPS 1           // removes small cyclic structures generated on map, makes map with long branches

class MapFoundation;                    // foward declare class for use in ctor

class Map {

    private:

        Matrix* mapData;                     // dynamically allocated matrix 

        CoordinateArray* nodes;              // dynamically allocated array of all room coordinates
        CoordinateArray* edges;              // dynamically allocated array of all door coordinates

        Coordinate start1;            // the coordinate of the spawn room ( around the centre of map ), to get another spawn use getRoom( getNumRooms()-1 )
        Coordinate start2;

        void copyMapData(const MapFoundation& mapFoundation); // utility function for copying MapFoundation object data *will throw err if dimension missmatch

        void mapDebug(const ExpandedMatrix& expandedMatrix, const MapFoundation& mapFoundation); // prints data at each stage of building the map

    public:
        Map(int maxBaseDimension = DEFAULT_DIMENSION);

        ~Map();

        int getYDim() const;        // getter for y dimension of this map
        int getXDim() const;        // getter for x dimension of this map

        int getNumNodes() const;            // getter size of nodes arr
        int getNumEdges() const;            // getter size of edges arr

        Coordinate getMapStart1() const;        // returns mapStart1
        Coordinate getMapStart2() const;      // returns mapStart2

        int get(const Coordinate& c);       // returns cell value at that coordinate if in bounds
        Coordinate getNodes(int i) const;    // returns node at index of node array if i exists

        void set(int x, int y, int value);  // sets a cell of the map to the 'value' if in bounds

        void setMapStart1(const Coordinate& c);     // set mapStart if it is in bounds
        void setMapStart2(const Coordinate& c);     // set mapStart if it is in bounds

        bool offBounds(int x, int y) const;     // returns true if the x,y coordinate is not in bounds eg. if offBounds(): break
        bool addNode(const Coordinate& c);      // adds a coordinate to nodes array if in bounds, if this is the first node added to the array sets c as mapStart
        bool addEdge(const Coordinate& c);      // adds a coordinate to edges array if in bounds

        void print() const;                     // prints the map formatted to include two marked nodes, @ being spawn 1 or mapStart, G being spawn 2 or this->nodes[this->numNodes-1]
        void printNodes() const;                // prints all coordinates of nodes in map
        void printEdges() const;                // prints all coordinates of edges in map

        /*
        TODO add member/method for adjacency list CoordinateArray for each node, containing data for the edges/nodes connected to each node
        */
};
