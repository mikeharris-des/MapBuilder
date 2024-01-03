#include "defs.h"    // for time
#include "Matrix.h"
#include "ExpandedMatrix.h"
#include "MapFoundation.h"

// MAKE COORDINATE OBJECT CLASS

void pBreak();

int main(){
    srand(static_cast<unsigned int>(time(nullptr)));
    ExpandedMatrix m(99);
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

    MapFoundation mapF(&m);
    mapF.print();

    pBreak();
    mapF.printBoundsAbsolute();
    pBreak();

    return 0;
}

void pBreak(){
    cout << endl << "-------------------------------------------------------" << endl;
}
