#include "defs.h"    // for time
#include "Matrix.h"    // for time
#include "ExpandedMatrix.h"    // for time

// MAKE COORDINATE OBJECT CLASS

void pBreak();

int main(){
    ExpandedMatrix m(11);
    pBreak();
    m.print();
    pBreak();

    m.printEx();
    pBreak();

    m.removeCommonLoops();
    m.print();
    pBreak();
    m.printEx();
    pBreak();

    // recursive walk through map -> store max coord top left max coord bottom right -> to crop -> make make a walk object
    // store visited
    // handle loops

    /*
    MapFoundation map(m); // map cropper?
    will have
        int maxCoordN[2];
        int maxCoordW[2];
        int maxCoordE[2];
        int maxCoordS[2];
        int centerCoord[2];

        int** storedCoords; // array of all coordinates visited on walk through is either 2 or 1 -> sort left to right top down
        int** matrixMap; // build array using stored coords


    */


    return 0;
}

void pBreak(){
    cout << endl << "-------------------------------------------------------" << endl;
}
