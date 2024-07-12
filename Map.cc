#include "Map.h"

Map::Map( int maxBaseDimension, int numSelectNodes ){
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


Map::~Map(){
    if(DEBUG)cout << "\nMap::dtor | LINE:" << __LINE__ << endl;
    delete this->edges;
    delete this->nodes;
    delete this->mapData;
    delete this->adjacencyListTable;
}

void Map::copyMapData(const MapFoundation& mapFoundation, int numSelectNodes){
    if(DEBUG)cout << "\nMap::copyMapData | LINE:" << __LINE__ << endl;

    // the following is assigning the pointers to the map data generated in mapFoundation object
    this->mapData = mapFoundation.finalMatrix;
    this->nodes = mapFoundation.finalNodes;
    this->edges = mapFoundation.finalEdges;
    this->adjacencyListTable = mapFoundation.finalAdjacencyListTable;

    setMapStart1((*this->nodes)[0]); // set the starting location to the same
    setMapStart2((*this->nodes)[this->nodes->getSize()-1]); // set the starting location to the same

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

// debug utility function
void pBreak(){cout << endl << "-------------------------------------------------------" << endl;}

void Map::mapDebug(const ExpandedMatrix& expandedMatrix, const MapFoundation& mapFoundation){
    cout << "\nMap::mapDebug | LINE:" << __LINE__ << endl;

    pBreak();
    cout << "expandedMatrix.print()" << endl;
    expandedMatrix.print();

    pBreak();
    cout << "expandedMatrix.printExpanded()" << endl;
    expandedMatrix.printExpanded();

    pBreak();
    cout << "mapFoundation.print()" << endl;
    mapFoundation.print(); // print map uncropped, after mapwalk, after removing of isolated elements

    pBreak();
    cout << "mapFoundation.printBoundsAbsolute()" << endl;
    mapFoundation.printBoundsAbsolute(); // print map uncropped, after mapwalk, after removing of isolated elements

    pBreak();
    cout << "mapFoundation.printFinalNodes()" << endl;
    mapFoundation.finalNodes->print();

    pBreak();
    cout << "mapFoundation.finalMatrix->print()" << endl;
    mapFoundation.finalMatrix->print();

    if(this->nodes->getSize()>=1){
        pBreak();
        cout << "map.print()" << endl;
        print();

        pBreak();
        cout << "   SPAWN1 START @ " << getMapStart1() << endl; // default starting coordinate of map or first node added to node array
        cout << "   SPAWN2 START G " << getMapStart2() << endl; // print last node added for option to add additional element to map -> eg PLAYER 2 SPAWN

        pBreak();
        cout << "map.printNodes()" << endl;
        printNodes();

    } else{
        cout << "| nodes arr | < 1" << endl;
    }
    pBreak();
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

Coordinate Map::getMapStart1() const{return this->start1;} // get defaulted starting coordinate 1
Coordinate Map::getMapStart2() const{return this->start2;} // get defaulted starting coordinate 2

// get cell value of coordinate
int Map::get(const Coordinate& c){
    if( offBounds(c.x,c.y) ){
        if(MAP_DEBUG)cout << "Map::get OFF BOUNDS" << endl;
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


// set these x,y values in map to the value
void Map::set(int x, int y, int value){
    if(DEBUG)cout << "\nMap::set x:" << x << " y:" << y << " to: " <<  value << " | LINE:" << __LINE__ << endl;

    if( offBounds(x,y) ){
        if(MAP_DEBUG)cout << "Map::set OFF BOUNDS" << endl;
        return;
    }
    this->mapData->setCell(x,y,value);
}

// set defaulted starting coordinate 1
void Map::setMapStart1(const Coordinate& c){
    if(DEBUG)cout << "\nMap::setMapStart1 " << c << " | LINE:" << __LINE__ << endl;

    int cell = get(c);

    if( cell == NODE ){
        this->start1 = Coordinate(c);
    } else {
        cout << " * invalid set start 1 location: " << c << endl; // no if debug something is really wrong if branches here
    }
}

// set defaulted starting coordinate 1
void Map::setMapStart2(const Coordinate& c){
    if(DEBUG)cout << "\nMap::setMapStart2 " << c << " | LINE:" << __LINE__ << endl;

    int cell = get(c);

    if( cell == NODE ){
        this->start2 = Coordinate(c);
    } else {
        cout << " * invalid set start 2 location: " << c << endl; // no if debug something is really wrong if branches here
    }
}

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
        cout << endl;
    }
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
