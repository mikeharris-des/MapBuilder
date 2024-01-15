#pragma once
#include "defs.h"
#include "Coordinate.h"
#include "ExpandedMatrix.h"
#include "Map.h"

#define MF_DEBUG 0

class Map;

//crops ExpandedMatrix with only navigable rooms and doors
class MapFoundation {

    friend class Map; // for constructor access to member data (inheritance created issues with other objects)

    private:
        ExpandedMatrix* expandedMatrix;

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
        void makeMap();
        Coordinate translateCoordMatrixToMap(const Coordinate& currCoord);

    public:
        MapFoundation(ExpandedMatrix* exMatrix);
        ~MapFoundation();

        Coordinate* getNextPath();

        void storeBounds(const Coordinate& currCoord);

        void checkDoors();
        bool addUniqueDoor(const Coordinate& currCoord);
        bool addUniqueRoom(const Coordinate& currCoord);

        void print() const;
        void printBoundsAbsolute() const;
        bool offBoundsFinalMap(const Coordinate& currCoord) const;

        void filterIsolated(); // remove any cell not connectected to main branch (rooms or doors unvisited)

};
