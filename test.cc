#include "defs.h"    // for time
#include "Matrix.h"
#include "ExpandedMatrix.h"
#include "MapFoundation.h"
#include "Map.h"

#define DEBUG_MAIN 0

void pBreak();

int main(){
    srand(static_cast<unsigned int>(time(nullptr)));

    ExpandedMatrix m(99);
    m.removeCommonLoops();

    MapFoundation mapF(&m);

    if(DEBUG_MAIN)pBreak();
    if(DEBUG_MAIN)mapF.printBoundsAbsolute(); // printing map foundation cropped
    pBreak();

    Map map(&mapF);
    map.print(); // printing full map object
    pBreak();

    return 0;
}

void pBreak(){
    cout << endl << "-------------------------------------------------------" << endl;
}
