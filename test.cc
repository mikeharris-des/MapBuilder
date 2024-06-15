#include "Map.h"                // map object header file

void printBreak();

int main(){
    srand(static_cast<unsigned int>(time(nullptr))); // seed random with current system time

    /*
    determines max size of map on random generation
        -> base dimension becomes expanded to: maxBaseDimension + (maxBaseDimension-1)
        -> randomly generated map will crop out unconnected nodes to the main map
    */
    int maxBaseDimension = 30;
    Map map(maxBaseDimension); // create map object

    printBreak();
    map.printAdjList(); // print the adjacency list first
    printBreak();

    map.print();               // print map object
    printBreak();

    cout << "   SPAWN1 START @ " << map.getMapStart1() << endl; // default primary starting coordinate of map
    cout << "   SPAWN2 START G " << map.getMapStart2() << endl; // default secondary starting coordinate of map
    printBreak();

    return 0;
}


// console printout break utility function
void printBreak(){cout << endl << "-------------------------------------------------------" << endl;}
