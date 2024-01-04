#pragma once
#include "defs.h"
#include "Coordinate.h"
#include "MapFoundation.h"

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

    public:
        Map(int sizeX = 3, int sizeY = 3);
        Map(MapFoundation* mapFoundation);
        ~Map();

        void set(int x, int y, int value);

        int getYDim() const;
        int getXDim() const;
        int getNumRooms() const;
        int getNumDoors() const;

        bool offBounds(int x, int y) const;
        bool addRoom(const Coordinate& c);
        bool addDoor(const Coordinate& c);
        void copyMapData(const Map& map);

        void print() const;

};
