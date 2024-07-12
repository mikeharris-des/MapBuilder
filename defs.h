#pragma once
#include <iostream>
#include <cstdlib>       // for srand
#include <ctime>         // for time
#include <string>
#include <unordered_map> // for hash map
#include <iomanip>       // for setw and setfill


// DEBUG FLAGS
#define DEBUG                   0      // MASTER DEBUG FOR EXECUTION ORDER OF FUNCTIONS
#define MAP_DEBUG               0      // debug for Map class member functions 0 == off

#define MF_DEBUG                0      // debug for MapFoundation class member functions 0 == off
#define MF_DEBUG_MAKEMAP        0      // debug for MapFoundation class makeMap function only 0 == off

#define EM_DEBUG                0      // debug for ExpandedMatrix class member functions 0 == off

#define DEFAULT_ARRAY_SIZE      32     // default size of array backing of collection objects before they need to resize

#define DEFAULT_DIMENSION       3      // if map obj is instantiated with negative integer dimensions or no dimensions is the default
#define MINIMUM_SELECT_NODES    2      // number of select nodes in the map for unique map features at these nodes
#define REMOVE_COMMON_LOOPS     1      // removes small cyclic structures generated on map, makes map with long branches

#define MAKE_SQUARE             1      // if final map must be square (for rendering ui is easiest if square) ( const used in MapFoundation::makeMap() )

#define EDGE                    2      // int value for connection between two node elements in matrix
#define NODE                    1      // in the binary matrix, nodes are valued 1 -> expanded maintain value and relative position but are translated
#define VISITED                 2      // visited marking increment for walking the expanded matrix, must be even value >= EDGE

#define OFF_BOUNDS              -1     // default int return if off bounds when accessing a coordinate cell value in the matrix

using namespace std;
