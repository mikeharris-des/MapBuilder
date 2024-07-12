#include "Map.h"                                     // map object header file


void printBreak(const Map& map);               // formatted console printout utility function


int main()
{
    srand( static_cast<unsigned int>(time(nullptr)) ); // seed random with current system time

    /*
    maxBaseDimension is a factor in determining the maximum size of final map on random generation
        -> base dimension becomes expanded to: maxBaseDimension + (maxBaseDimension-1)
        -> randomly generated map will crop out unconnected nodes to the main map

    numberOfSpawnLocations determines how many unique nodes are stored in the map
        -> minimum is 2
        -> maximum is the number of nodes generated on the map
    */
    int maxBaseDimensionOfMatrix = 30;
    int numberOfSpawnLocations = 2;

    Map map( maxBaseDimensionOfMatrix, numberOfSpawnLocations );  // create map object

    printBreak(map);
    map.printAdjList();         // print the adjacency list first
    printBreak(map);

    map.print();                // print 2d visual of map object formatted with coordinate display
    printBreak(map);

    map.printSelectNodes();     // print the coordinates of select nodes in map
    printBreak(map);

    return 0;
}


// formatted console printout utility function
void printBreak( const Map& map )
{
    int size = 2*map.getXDim() + 3; // magic numbers correspond to size of formatted output spacing and coordinate display
    cout << endl;
    for(int i = 0 ; i < size ; ++i) { cout << "-"; }
    cout << endl;
}
