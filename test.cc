#include "defs.h"    // for time
#include "Matrix.h"
#include "ExpandedMatrix.h"
#include "MapFoundation.h"

#define DEBUG_MAIN 0

void pBreak();

int main(){
    srand(static_cast<unsigned int>(time(nullptr)));
    ExpandedMatrix m(99);
    if(DEBUG_MAIN)pBreak();
    if(DEBUG_MAIN)m.print();
    if(DEBUG_MAIN)pBreak();

    if(DEBUG_MAIN)m.printEx();
    if(DEBUG_MAIN)pBreak();
    if(DEBUG_MAIN)m.printConnections();
    if(DEBUG_MAIN)pBreak();

    m.removeCommonLoops();

    if(DEBUG_MAIN)m.print();
    if(DEBUG_MAIN)pBreak();
    if(DEBUG_MAIN)m.printEx();
    if(DEBUG_MAIN)pBreak();
    if(DEBUG_MAIN)m.printConnections();
    if(DEBUG_MAIN)pBreak();

    MapFoundation mapF(&m);
    if(DEBUG_MAIN)mapF.print();

    pBreak();
    mapF.printBoundsAbsolute();
    pBreak();

    return 0;
}

void pBreak(){
    cout << endl << "-------------------------------------------------------" << endl;
}
