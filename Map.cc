#include "Map.h"

Map::Map( int maxBaseDimension, int numSelectNodes )
{
    if(DEBUG)cout << "\nMap::ctor | LINE:" << __LINE__ << endl;

    if (maxBaseDimension < DEFAULT_DIMENSION) maxBaseDimension = DEFAULT_DIMENSION;

    ExpandedMatrix expandedMatrix(maxBaseDimension); // makes expanded matrix with new dimensions nxn, n = dimension + (dimension-1)

    expandedMatrix.removeCommonLoops(); // removes a random 2 on for a room that has four 2s adjacent to it. Makes an aestetic looking map

    MapFoundation mapFoundation(&expandedMatrix); // Foundation crops map and removes nodes/edges not accessible to main starting location / selectNode

    try{
        copyMapData(mapFoundation, numSelectNodes); // if there is an invalid matrix from mapFoundation there will be an error thrown instead of a crash
    } catch(const string& error){
        cout << endl<< " * MAP: FAILED TO COPY MAP DATA * " << error << endl;
    }

    if(DEBUG)mapDebug(expandedMatrix,mapFoundation);
}


Map::~Map()
{
    if(DEBUG)cout << "\nMap::dtor | LINE:" << __LINE__ << endl;
    delete this->edges;
    delete this->nodes;
    delete this->mapData;
    delete this->adjacencyListTable;
}

void Map::copyMapData(const MapFoundation& mapFoundation, int numSelectNodes)
{
    if(DEBUG)cout << "\nMap::copyMapData | LINE:" << __LINE__ << endl;

    // the following is assigning the pointers to the map data generated in mapFoundation object
    this->mapData = mapFoundation.finalMatrix;
    this->nodes = mapFoundation.finalNodes;
    this->edges = mapFoundation.finalEdges;
    this->adjacencyListTable = mapFoundation.finalAdjacencyListTable;

    if (numSelectNodes < MINIMUM_SELECT_NODES) numSelectNodes = MINIMUM_SELECT_NODES;
    else if (numSelectNodes > getNumNodes()) numSelectNodes = getNumNodes();

    for(int i = 0; i < numSelectNodes; ++i)
    {
        if (i==0)
        {
            this->selectNodes+=(*this->nodes)[0];
        }
        else if (i==1)
        {
            this->selectNodes+=(*this->nodes)[this->nodes->getSize()-1];
        }
        else
        {
            int index = ( rand() % (this->nodes->getSize()-2) ) + 1;    // ensures a selectNode is not likely to be one of the chosen nodes (atleast not the default nodes, the first and last one added)
            this->selectNodes+=(*this->nodes)[index];
        }
    }
}

// formatted console printout utility function
void pBreak( const Map& map , int testID )
{
    int size = 2*map.getXDim() + 3; // magic numbers correspond to size of formatted output spacing and coordinate display
    cout << endl;
    for(int i = 0 ; i < size ; ++i) { cout << "-"; }
    cout << " * " << testID << endl;
}

void Map::mapDebug(const ExpandedMatrix& expandedMatrix, const MapFoundation& mapFoundation)
{
    cout << "\nMap::mapDebug | LINE:" << __LINE__ << endl;
    int testID = 0;

    pBreak(*this, ++testID);
    cout << "expandedMatrix.print()" << endl;
    expandedMatrix.print();

    pBreak(*this, ++testID);
    cout << "expandedMatrix.printExpanded()" << endl;
    expandedMatrix.printExpanded();

    pBreak(*this, ++testID);
    cout << "mapFoundation.print()" << endl;
    mapFoundation.print(); // print map uncropped, after mapwalk, after removing of isolated elements

    pBreak(*this, ++testID);
    cout << "mapFoundation.printBoundsAbsolute()" << endl;
    mapFoundation.printBoundsAbsolute(); // print map uncropped, after mapwalk, after removing of isolated elements

    pBreak(*this, ++testID);
    cout << "mapFoundation.printFinalNodes()" << endl;
    mapFoundation.finalNodes->print();

    pBreak(*this, ++testID);
    cout << "mapFoundation.finalMatrix->print()" << endl;
    mapFoundation.finalMatrix->print();

    pBreak(*this, ++testID);
    cout << "map.printAdjList()" << endl;
    printAdjList();

    pBreak(*this, ++testID);
    cout << "map.print()" << endl;
    print();

    pBreak(*this, ++testID);
    cout << "testing Map adjacency implementation < Map::getAdjacency(), Map::getEdge() > " << endl;
    // test adjacency list table and the corresponding utility functions for accessing adjacency elements
    for(int i = 0; i < (*nodes).getSize() ; ++i )
    {
        cout << endl << "NODE: " << (*nodes)[i] << endl;
        const CoordinateArray temp = getAdjacency( (*nodes)[i] );
        temp.print();
        for(int j = 0; j < temp.getSize() ; ++j )
        {
            cout << "       " << (*nodes)[i] << " --[" << direction::toString( direction::getDirection( (*nodes)[i], temp[j] ) ) << "]-> " << getEdge( (*nodes)[i] , temp[j] )  << " -> " << temp[j] << endl;
        }
        cout << endl;
    }
}


int Map::getYDim() const{return this->mapData->dy;}

int Map::getXDim() const{return this->mapData->dx;}

int Map::getNumNodes() const{return this->nodes->getSize();} // get number of nodes in map (1s = nodes)

int Map::getNumEdges() const{return this->edges->getSize();} // get number of edges in map (2s = connections/edges between nodes)

// returns coordinate to a select node by index in the array
Coordinate Map::getSelectNode(int index) const
{
    int size = this->selectNodes.getSize();

    if(index<0 || index >= size) {return Coordinate( (-1), (-1) );}

    return this->selectNodes[index]; // default return if indexing bad node
}

// get cell value of coordinate
int Map::getCell(const Coordinate& c){
    if( offBounds(c.x,c.y) ){
        if(MAP_DEBUG)cout << "Map::getCell OFF BOUNDS" << endl;
        return 0;
    }

    return this->mapData->getCell(c.x,c.y);
}

// returns room at index of room array
Coordinate Map::getNodes(int i) const{
    Coordinate node;
    if(i<0 || this->nodes->getSize() <= i){
        return node;
    } else{
        node.set((*this->nodes)[i].x,(*this->nodes)[i].y);
    }
    return node;
}

// returns the adjacency list of coordinates to edges in order 0->3 : ENWS
const CoordinateArray& Map::getAdjacency(const Coordinate& c) const
{
    if( offBounds(c.x,c.y) )
    {
        if(MAP_DEBUG)cout << "Map::getAdjacency OFF BOUNDS" << endl;
        exit(1);
    }
    return (*this->adjacencyListTable)[c];
}

// returns the edge connecting a and b if it exists
Coordinate Map::getEdge(const Coordinate& a, const Coordinate& b) const
{
    if( offBounds(a.x,a.y) || offBounds(b.x,b.y) )
    {
        if(MAP_DEBUG)cout << "Map::getEdge OFF BOUNDS" << endl;
        exit(1);
    }

    // 2 is the cell coordinate difference of 2 nodes in the map (seperated by 1 edge)
    int diffX = (b.x - a.x)/2;
    int diffY = (b.y - a.y)/2;

    if( abs(diffX) > 1 || abs(diffY) > 1 ) { return Coordinate(); }

    return Coordinate( a.x + diffX, a.y + diffY );
}

// set these x,y values in map to the value
void Map::set(int x, int y, int value){
    if(DEBUG)cout << "\nMap::set x:" << x << " y:" << y << " to: " <<  value << " | LINE:" << __LINE__ << endl;

    if( offBounds(x,y) ){
        if(MAP_DEBUG)cout << "Map::set OFF BOUNDS" << endl;
        return;
    }
    this->mapData->setCell(x,y,value);
}

// checks the bounds of the map using the matrix object function
bool Map::offBounds(int x, int y) const{
    return this->mapData->offBounds(x,y);
}

// adds node if coordinate is unique to array and coordinate is in the boundry of the map
bool Map::addNode(const Coordinate& c){
    if(DEBUG)cout << "\nMap::addNode " << c << " | LINE:" << __LINE__ << endl;

    if(this->nodes->contains(c) || offBounds(c.x,c.y)){
        return false;
    }

    *this->nodes += c;

    return true;
}

// adds Edge if coordinate is unique to array and coordinate is in the boundry of the map
bool Map::addEdge(const Coordinate& c){
    if(DEBUG)cout << "\nMap::addEdge " << c << " | LINE:" << __LINE__ << endl;

    if(this->edges->contains(c) || offBounds(c.x,c.y)){
        return false;
    }

    *this->edges += c;

    return true;
}

// print full map with the center location emphasized as @ char
void Map::print() const
{
    int rowCounter = 0;

    cout << endl << "   ";
    for(int colCounter = 0 ; colCounter < this->mapData->dx ; colCounter++) { cout << colCounter%10 << " ";}
    cout << endl;

    CoordinateArray remainingSelectNodes(this->selectNodes);

    for(int y = 0; y<this->mapData->dy; ++y)
    {
        cout << endl << (rowCounter++)%10 << " ";

        for(int x = 0; x<this->mapData->dx; ++x)
        {
            if(this->mapData->getCell(x,y)==0){ cout << " ."; }
            else
            {
                bool selectNodeAtCoordinate = false;
                for(int i = 0 ; i<remainingSelectNodes.getSize() ; ++i)
                {
                    Coordinate checkNode = remainingSelectNodes[i];

                    if(i==0)    // if the first selectNode will be the Primary selectNode
                    {
                        if(checkNode.x==x && checkNode.y==y)
                        {
                            cout << " @"; // print start 1 location as a unique character
                            selectNodeAtCoordinate = true;
                        }
                    }
                    else if(checkNode.x==x && checkNode.y==y)
                    {
                        cout << " #";    // print alternative location as a unique character
                        remainingSelectNodes -= checkNode;
                        selectNodeAtCoordinate = true;
                        break;
                    }
                }
                if(!selectNodeAtCoordinate)
                {
                    cout << " " << this->mapData->getCell(x,y);  // print node as the cell value
                }
            }
        }
    }
    cout << endl;
}

void Map::printNodes() const{
    cout << "\nALL NODES" << endl;
    this->nodes->print();
    cout << "\nALL SELECT NODES" << endl;
    this->selectNodes.print();
}

void Map::printEdges() const{
    cout << "\nALL EDGES" << endl;
    this->edges->print();
}

void Map::printSelectNodes() const
{
    cout << endl;
    int size = this->selectNodes.getSize();
    for(int i = 0 ; i<size ; ++i)
    {
        cout << "   SELECT NODE [" << setw(2) << (i+1) << " ] START: ";
        if(i == 0)
        {
            cout << "@ "; // default primary starting coordinate of map
        }
        else
        {
            cout << "# "; // default alternative starting coordinate icon on map printout
        }
        cout << this->selectNodes[i] << endl;
    }
}

void Map::printAdjList() const{
    cout << "\nALL ADJACENCY DATA" << endl << endl;
    int counter = 1;

    for (unordered_map<Coordinate, CoordinateArray>::iterator node = this->adjacencyListTable->begin(); node != this->adjacencyListTable->end(); ++node)
    {
        const Coordinate& coord = node->first;
        const CoordinateArray& adjacencyList = node->second;

        cout << setw(5) << counter++ << " : " << coord << " = ";
        int size = adjacencyList.getSize();
        for (int i = 0; i<size; ++i){
            cout << adjacencyList[i] << " ";
        }
        cout << endl;
    }
}
