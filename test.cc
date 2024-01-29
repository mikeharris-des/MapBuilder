#include "defs.h"               // includes cstdlib for srand and ctime for time
#include "Matrix.h"             // base matrix
#include "ExpandedMatrix.h"     // matrix expanded with doors
#include "MapFoundation.h"      // object bridges matrix to full map object
#include "Map.h"

#define DEBUG_MAIN 0
#define SHOW_MAP_EVOLUTION 0

void pBreak();

int main(){
    srand(static_cast<unsigned int>(time(nullptr)));

    // random generate base matrix of 1s or 0s size of dimension x dimension
    int dimension = 30; // dimension will get rounded down if it is not odd

    //-> makes expanded matrix of size dimension + (dimension-1)    * currently not optimized for dimension > 100
    ExpandedMatrix m(dimension); // adds 2s to indicate connections between rooms
    m.removeCommonLoops(); // removes alot of tight loops makes map with long branches and aesthetic look (try commenting it out)

    if(SHOW_MAP_EVOLUTION){
        pBreak();
        m.print();
        pBreak();
        m.printExpanded();
    }

    // Foundation crops map and removes rooms/doors not accessible to starting location
    MapFoundation mapF(&m);

    // make final map object -> full map without now unusable/irrelevant member functions or variables required to build it
    Map map(&mapF);

    if(SHOW_MAP_EVOLUTION){
        pBreak();
        mapF.print(); // print map uncropped, after mapwalk, after removing of isolated elements
    }

    pBreak();
    map.print(); // print final map object
    pBreak();
    cout << "   SPAWN1 START @ " << map.getStart() << endl; // default starting coordinate of map
    cout << "   SPAWN2 START G " << map.getRoom(map.getNumRooms()-1) << endl; // print last room added for option to add additional element to map -> eg PLAYER 2 SPAWN
    pBreak();

    if(DEBUG_MAIN){
        pBreak();
        map.printRooms(); // printing all room coordinates
        pBreak();
        map.printDoors(); // printing all door coordinates
    }

    return 0;
}

// formatted console line break print for readibility
void pBreak(){
    cout << endl << "-------------------------------------------------------" << endl;
}
