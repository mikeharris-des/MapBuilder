#pragma once
#include "MapDefs.h"            // includes cstdlib for srand and ctime for time
#include "MapFoundation.h"      // object bridges matrix to full map object
#include "Matrix.h"             // base matrix and expanded matrix
#include "Coordinate.h"         // 2d coordinate object, PARAMETER ORDERING WILL ALWAYS BE X THEN Y: (x,y)

class MapFoundation;                    // foward declare class for use in ctor

class Map {

    private:

        Matrix* mapData;                     // dynamically allocated matrix

        CoordinateArray* nodes;              // dynamically allocated array of all room coordinates
        CoordinateArray* edges;              // dynamically allocated array of all door coordinates

        /* adjacencyListTable
        hash table storing node adjacency coordinates if an attaches them at a given node in order of ENWS

            INTERNAL USE : (*adjacencyListTable)[coord] = nodeAdjList
        */
        unordered_map<Coordinate, CoordinateArray, CoordinateHashFunction> *adjacencyListTable;

        CoordinateArray selectNodes;        // coordinate array of random nodes designated for unique map features at these nodes

        void copyMapData(const MapFoundation& mapFoundation, int numSelectNodes); // utility function for copying MapFoundation object data *will throw err if dimension missmatch

        void mapDebug(const ExpandedMatrix& expandedMatrix, const MapFoundation& mapFoundation); // prints data at each stage of building the map

        bool addNode(const Coordinate& c);      // adds a coordinate to nodes array if in bounds, if this is the first node added to the array sets c as mapStart
        bool addEdge(const Coordinate& c);      // adds a coordinate to edges array if in bounds

    public:
        Map(int maxBaseDimension = DEFAULT_DIMENSION, int numSelectNodes = MINIMUM_SELECT_NODES);

        ~Map();

        int getYDim() const;        // getter for y dimension of this map
        int getXDim() const;        // getter for x dimension of this map

        int getNumNodes() const;            // getter size of nodes arr
        int getNumEdges() const;            // getter size of edges arr

        Coordinate getSelectNode(int index) const;                  // returns coordinate to a select node by index in the array

        int getCell(const Coordinate& c);                                   // returns cell value at that coordinate if in bounds
        Coordinate getNodes(int i) const;                                   // returns node at index of node array if i exists

        const CoordinateArray& getAdjacency(const Coordinate& c) const;     // returns the adjacency list of coordinates to edges in order 0->3 : ENWS
        Coordinate getEdge(const Coordinate& a, const Coordinate& b) const; // returns the edge connecting a and b if it exists

        void set(int x, int y, int value);  // sets a cell of the map to the 'value' if in bounds

        bool offBounds(int x, int y) const;     // returns true if the x,y coordinate is not in bounds eg. if offBounds(): break

        void print() const;                     // prints the map formatted to include two marked nodes, @ being spawn 1 or mapStart, G being spawn 2 or this->nodes[this->numNodes-1]
        void printNodes() const;                // prints all coordinates of nodes in map
        void printEdges() const;                // prints all coordinates of edges in map
        void printSelectNodes() const;          // prints all coordinates of selectNodes in map

        void printAdjList() const;              // prints all coordinates of nodes adjacent to each node in map
};
