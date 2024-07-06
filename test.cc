#include "Map.h"                                     // map object header file


void printBreak(const Map& map);               // formatted console printout utility function


int main()
{
    srand(static_cast<unsigned int>(time(nullptr))); // seed random with current system time

    /*
    determines max size of map on random generation
        -> base dimension becomes expanded to: maxBaseDimension + (maxBaseDimension-1)
        -> randomly generated map will crop out unconnected nodes to the main map
    */
    int maxBaseDimension = 30;
    Map map(maxBaseDimension,5);  // create map object

    printBreak(map);
    map.printAdjList();         // print the adjacency list first
    printBreak(map);

    map.printNodes();
    printBreak(map);

    map.printSelectNodes();
    printBreak(map);

    map.print();                // print map object
    printBreak(map);

    cout << "   ENTITY SPAWN1 START: @ " << map.getMapStart1() << endl;     // default primary starting coordinate of map
    cout << "   ENTITY SPAWN2 START: G " << map.getMapStart2() << endl;     // default secondary starting coordinate of map
    printBreak(map);

    return 0;
}


// formatted console printout utility function
void printBreak(const Map& map)
{
    int size = 2*map.getXDim() + 3;
    cout << endl;
    for(int i = 0 ; i < size ; ++i) { cout << "-"; }
    cout << endl;
}
