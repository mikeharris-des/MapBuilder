#pragma once
#include "defs.h"
#include "Coordinate.h"
#include "MapFoundation.h"

#define MAP_DEBUG 0
#define DEFAULT_DIMENSION 3

class MapFoundation;

class Map {

    private:
        int** mapData;
        int xDim;
        int yDim;

        Coordinate* rooms;
        int numRooms;

        Coordinate* doors;
        int numDoors;

        Coordinate mapStart;

    public:
        Map(int sizeX = DEFAULT_DIMENSION, int sizeY = DEFAULT_DIMENSION);
        Map(MapFoundation* mapFoundation);
        ~Map();

        int getYDim() const;
        int getXDim() const;
        int getNumRooms() const;
        int getNumDoors() const;
        Coordinate getStart() const;
        int get(const Coordinate& c);
        Coordinate getRoom(int i) const; // returns room at index of room array

        void set(int x, int y, int value);
        void setStart(const Coordinate& c);

        bool offBounds(int x, int y) const;
        bool addRoom(const Coordinate& c);
        bool addDoor(const Coordinate& c);
        void copyMapData(const Map& map);

        void print() const;
        void printRooms() const;
        void printDoors() const;
};
