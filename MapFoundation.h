#include "defs.h"
#include "Coordinate.h"
#include "ExpandedMatrix.h"

#define VISITED 2

//crops ExpandedMatrix with only navigable rooms and doors
class MapFoundation {
    private:
        ExpandedMatrix* map;

        Coordinate* rooms;
        int numRooms;

        Coordinate* doors;
        int numDoors;

        int coordMostN;
        int coordMostE;
        int coordMostS;
        int coordMostW;

        void mapWalk(Coordinate& currCoord);
        void filterIsolated();

    public:
        MapFoundation(ExpandedMatrix* exMatrix);
        ~MapFoundation();

        Coordinate* getNextPath();

        void storeBounds(const Coordinate& currCoord);

        void checkDoors();
        void print() const;
        void printBoundsAbsolute() const;
};
