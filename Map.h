#pragma once
#include "defs.h"
#include "Coordinate.h"
#include "MapFoundation.h"

#define MAP_DEBUG 0                     // debug for map class 0 == off
#define DEFAULT_DIMENSION 3             // if map obj is instantiated with negative integer dimensions or no dimensions is the default

class MapFoundation;                    // foward declare class for use in ctor

class Map {

    private:
        int* mapData;                   // dynamically allocated array 'bitstring' of 0s as void, 1s as rooms, 2s as doors between rooms
        int xDim;                       // size of x dimension for map
        int yDim;                       // size of y dimension for map

        Coordinate* rooms;              // dynamically allocated array of all room coordinates
        int numRooms;                   // size of this->rooms array or number of rooms in map

        Coordinate* doors;              // dynamically allocated array of all door coordinates
        int numDoors;                   // size of this->doors array or number of doors in map

        Coordinate mapStart;            // the coordinate of the spawn room ( around the centre of map ), to get another spawn use getRoom( getNumRooms()-1 )

        void copyMapData(const Map& map); // utility function for copying MapFoundation object data *will throw err if dimension missmatch

    public:
        Map(int sizeX = DEFAULT_DIMENSION, int sizeY = DEFAULT_DIMENSION);
        Map(MapFoundation* mapFoundation);
        ~Map();

        int getYDim() const;        // getter for y dimension of this map
        int getXDim() const;        // getter for x dimension of this map

        int getNumRooms() const;            // returns numRooms
        int getNumDoors() const;            // returns numDoors

        Coordinate getStart() const;        // returns mapStart
        int get(const Coordinate& c);       // returns cell value at that coordinate if in bounds
        Coordinate getRoom(int i) const;    // returns room at index of room array if i exists

        void set(int x, int y, int value);  // sets a cell of the map to the 'value' if in bounds
        void setStart(const Coordinate& c);     // set mapStart if it is in bounds

        bool offBounds(int x, int y) const;     // returns true if the x,y coordinate is not in bounds eg. if offBounds(): break
        bool addRoom(const Coordinate& c);      // adds a coordinate to rooms array if in bounds, if this is the first room added to the array sets c as mapStart
        bool addDoor(const Coordinate& c);      // adds a coordinate to doors array if in bounds

        void print() const;                     // prints the map formatted to include two marked rooms, @ being spawn 1 or mapStart, G being spawn 2 or this->rooms[this->numRooms-1]
        void printRooms() const;                // prints all coordinates of rooms in map
        void printDoors() const;                // prints all coordinates of doors in map 
};
