#include "defs.h"    // for time
#include "Matrix.h"
#include "ExpandedMatrix.h"
#include "MapFoundation.h"

// MAKE COORDINATE OBJECT CLASS

void pBreak();

int main(){
    srand(static_cast<unsigned int>(time(nullptr)));
    ExpandedMatrix m(5);
    pBreak();
    m.print();
    pBreak();

    m.printEx();
    pBreak();
    m.printConnections();
    pBreak();

    m.removeCommonLoops();

    m.print();
    pBreak();
    m.printEx();
    pBreak();
    m.printConnections();
    pBreak();

    MapFoundation cropMap(&m);
    cropMap.print();
    
    pBreak();
    cropMap.printBoundsAbsolute();
    pBreak();

    return 0;
}

void pBreak(){
    cout << endl << "-------------------------------------------------------" << endl;
}
