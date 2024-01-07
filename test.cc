#include "defs.h"               // includes cstdlib for srand and ctime for time
#include "Matrix.h"             // base matrix
#include "ExpandedMatrix.h"     // matrix expanded with doors
#include "MapFoundation.h"      // object bridges matrix to full map object  
#include "Map.h"

#define DEBUG_MAIN 0

void pBreak();

int main(){
    srand(static_cast<unsigned int>(time(nullptr)));

    // random generate base matrix of 1s or 0s size of dimension x dimension
    int dimension = 5;

    //-> makes expanded matrix of size dimension x (dimension-1)
    ExpandedMatrix m(dimension); // adds 2s to indecate connections between rooms
    m.removeCommonLoops(); // removes alot of tight loops makes map with long branches and aestetic (try commenting it out)

    // Foundation crops map and removes rooms/doors not accessible to starting location
    MapFoundation mapF(&m);

    pBreak(); // console print line break

    // make final map object -> full map without now unusable/irrelevant member functions or variables required to build it
    Map map(&mapF);

    map.print(); // print final map object
    pBreak();
    cout << "MAP START @ " << map.getStart() << endl; // default starting coordinate of map
    pBreak();


    if(DEBUG_MAIN)pBreak();
    if(DEBUG_MAIN)map.printRooms(); // printing full map object
    if(DEBUG_MAIN)pBreak();
    if(DEBUG_MAIN)map.printDoors(); // printing full map object
    return 0;
}

// formatted console line break print for readibility
void pBreak(){
    cout << endl << "-------------------------------------------------------" << endl;
}
