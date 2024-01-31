#include "defs.h"               // includes cstdlib for srand and ctime for time
#include "Matrix.h"             // base matrix
#include "ExpandedMatrix.h"     // matrix expanded with doors
#include "MapFoundation.h"      // object bridges matrix to full map object
#include "Map.h"

#define SHOW_MAP_EVOLUTION 0       // if enabled will print to console the instances of the map before it is modified in each rendering phase
#define DEBUG_MAIN 0               // if enabled will print to console the coordinates of all rooms and doors in map

void pBreak();

int main(){
    srand(static_cast<unsigned int>(time(nullptr))); // seed random with current system time 

    // randomly generated base matrix of 1s or 0s size of dimension x dimension. Program Optimized for assigning dimension = 3-400 
    int dimension = 30; // dimension will get rounded down if it is not odd 

    ExpandedMatrix m(dimension); // makes expanded matrix with new dimensions nxn, n = dimension + (dimension-1)
    m.removeCommonLoops(); // removes a random 2 on for a room that has four 2s adjacent to it. Makes an aestetic looking map, try commenting it out 

    if(SHOW_MAP_EVOLUTION){  
        pBreak();
        m.print();         // print base matrix of only 1s and 0s before expand
        pBreak();
        m.printExpanded(); // print expanded matrix of 1s 2s and 0s after expand
    }

    MapFoundation mapF(&m); // Foundation crops map and removes rooms/doors not accessible to starting location

    Map map(&mapF); // make final map object: full map without now unusable/irrelevant member functions or variables required to build it

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
