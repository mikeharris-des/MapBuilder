#pragma once
#include "defs.h"
#include "Coordinate.h"
#include "ExpandedMatrix.h"
#include "Map.h"

#define VISITED 2

class Map;

//crops ExpandedMatrix with only navigable rooms and doors
class MapFoundation {

    friend class Map; // for constructor access to member data (inheritance created issues with other objects)

    private:
        ExpandedMatrix* matrix;

        Coordinate* rooms;
        int numRooms;

        Coordinate* doors;
        int numDoors;

        int coordMostN;
        int coordMostE;
        int coordMostS;
        int coordMostW;

        Map* finalMap;

        void matrixWalk(Coordinate& currCoord);
        void filterIsolated();
        void makeMap();

    public:
        MapFoundation(ExpandedMatrix* exMatrix);
        ~MapFoundation();

        Coordinate* getNextPath();

        void storeBounds(const Coordinate& currCoord);

        void checkDoors();
        void print() const;
        void printBoundsAbsolute() const;

};
