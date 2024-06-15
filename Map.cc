#include "Map.h"

Map::Map(int maxBaseDimension){
    if(DEBUG)cout << "\nMap::ctor | LINE:" << __LINE__ << endl;

    if (maxBaseDimension < DEFAULT_DIMENSION) maxBaseDimension = DEFAULT_DIMENSION;

    ExpandedMatrix expandedMatrix(maxBaseDimension); // makes expanded matrix with new dimensions nxn, n = dimension + (dimension-1)

    expandedMatrix.removeCommonLoops(); // removes a random 2 on for a room that has four 2s adjacent to it. Makes an aestetic looking map, try commenting it outCLEARCOMMENT

    MapFoundation mapFoundation(&expandedMatrix); // Foundation crops map and removes rooms/doors not accessible to starting locationCLEARCOMMENT

    try{
        copyMapData(mapFoundation); // if there is an invalid matrix from mapFoundation there will be an error thrown instead of a crash
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

void Map::copyMapData(const MapFoundation& mapFoundation){
    if(DEBUG)cout << "\nMap::copyMapData | LINE:" << __LINE__ << endl;

    // the following is assigning the pointers to the map data generated in mapFoundation object
    this->mapData = mapFoundation.finalMatrix;
    this->nodes = mapFoundation.finalNodes;
    this->edges = mapFoundation.finalEdges;
    this->adjacencyListTable = mapFoundation.finalAdjacencyListTable;

    setMapStart1((*this->nodes)[0]); // set the starting location to the same
    setMapStart2((*this->nodes)[this->nodes->getSize()-1]); // set the starting location to the same
}

// debug utility function
void pBreak(){cout << endl << "-------------------------------------------------------" << endl;}

void Map::mapDebug(const ExpandedMatrix& expandedMatrix, const MapFoundation& mapFoundation){
    cout << "\nMap::mapDebug | LINE:" << __LINE__ << endl;

    pBreak();
    cout << "expandedMatrix.print()" << endl;
    expandedMatrix.print();         // print base matrix of only 1s and 0s before expandCLEARCOMMENT

    pBreak();
    cout << "expandedMatrix.printExpanded()" << endl;
    expandedMatrix.printExpanded(); // print expanded matrix of 1s 2s and 0s after expandCLEARCOMMENT

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
void Map::print() const{

    for(int y = 0; y<this->mapData->dy;++y){
        for(int x = 0; x<this->mapData->dx;++x){

            if(this->mapData->getCell(x,y)==0){cout << " .";}
            else{
                if( start1.x==x && start1.y==y ){
                    cout << " @";    // print start 1 location as a unique character
                }else if( start2.x==x && start2.y==y ){
                    cout << " G";    // print start 2 location as a unique character
                }else{
                    cout << " "<< this->mapData->getCell(x,y);
                }
            }

        }
        cout << endl;
    }

}

void Map::printNodes() const{
    cout << "\nALL NODES" << endl;
    this->nodes->print();
}

void Map::printEdges() const{
    cout << "\nALL EDGES" << endl;
    this->edges->print();
}

void Map::printAdjList() const{
    cout << "\nALL ADJACENCY DATA" << endl;
    int counter = 1;
    for( const auto& node : *this->adjacencyListTable ){
        const Coordinate& coord = node.first;
        const CoordinateArray& adjacencyList = node.second;

        cout << "  " << counter++ << " : " << coord << " = ";
        int size = adjacencyList.getSize();
        for (int i = 0; i<size; ++i){
            cout << adjacencyList[i] << " ";
        }
        cout << endl;
    }
}
