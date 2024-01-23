#include "Map.h"


Map::Map(int sizeX, int sizeY){
    if(DEBUG)cout << "\nMap::ctor | LINE:" << __LINE__ << endl;

    if (sizeX <= 0 || sizeY <= 0) {
        this->xDim = DEFAULT_DIMENSION;
        this->yDim = DEFAULT_DIMENSION;
    } else {
        this->xDim = sizeX;
        this->yDim = sizeY;
    }

    this->mapData = new int*[sizeY];
    for(int y = 0; y<this->yDim; ++y){
        this->mapData[y] = new int[sizeX];
    }

    this->rooms = new Coordinate[sizeX*sizeY];
    this->numRooms = 0;

    this->doors = new Coordinate[sizeX*sizeY];
    this->numDoors = 0;
}


Map::Map(MapFoundation* mapFoundation){
    if(DEBUG)cout << "\nMap::copy ctor | LINE:" << __LINE__ << endl;

    int sizeX = mapFoundation->finalMap->getXDim();
    int sizeY = mapFoundation->finalMap->getYDim();

    this->xDim = sizeX;
    this->yDim = sizeY;

    this->mapData = new int*[sizeY];
    for(int y = 0; y<this->yDim; ++y){
        this->mapData[y] = new int[sizeX];
    }

    this->rooms = new Coordinate[sizeX*sizeY];
    this->numRooms = 0;


    this->doors = new Coordinate[sizeX*sizeY];
    this->numDoors = 0;

    try{
        copyMapData(*mapFoundation->finalMap);
    } catch(const string& error){
        cout << endl<< " * MAP: FAILED TO COPY MAP DATA * " << error << endl;
    }
}

Map::~Map(){
    if(DEBUG)cout << "\nMap::dtor | LINE:" << __LINE__ << endl;

    delete [] this->doors;
    delete [] this->rooms;

    for(int y = 0; y<this->yDim; ++y){
        delete [] this->mapData[y];
    }
    delete [] this->mapData;
}

int Map::getYDim() const{return this->yDim;}

int Map::getXDim() const{return this->xDim;}

int Map::getNumRooms() const{return this->numRooms;} // get number of rooms in map (1s or vertexes)

int Map::getNumDoors() const{return this->numDoors;} // get number of doors in map (2s or connections/edges between rooms/vertexes)

Coordinate Map::getStart() const{return this->mapStart;} // get starting coordinate

// get cell value of coordinate
int Map::get(const Coordinate& c){
    if( offBounds(c.x,c.y) ){
        if(MAP_DEBUG)cout << "Map::get OFF BOUNDS" << endl;
        return 0;
    }

    return this->mapData[c.y][c.x];
}

// returns room at index of room array
Coordinate Map::getRoom(int i) const{
    Coordinate room;
    if(i<0 || this->numRooms<=i){
        return room;
    } else{
        room.set(this->rooms[i].x,this->rooms[i].y);
    }
    return room;
}


// set these x,y values in map to the value
void Map::set(int x, int y, int value){
    if(DEBUG)cout << "\nMap::set x:" << x << " y:" << y << " to: " <<  value << " | LINE:" << __LINE__ << endl;

    if( offBounds(x,y) ){
        if(MAP_DEBUG)cout << "Map::set OFF BOUNDS" << endl;
        return;
    }

    this->mapData[y][x] = value;
}

// set starting coordinate
void Map::setStart(const Coordinate& c){
    if(DEBUG)cout << "\nMap::setStart " << c << " | LINE:" << __LINE__ << endl;

    int cell = get(c);

    if( cell == ROOM ){
        this->mapStart = c;
    } else {
        cout << " * invalid set start location: " << c << endl; // no if debug something is really wrong if branches here
    }
}

bool Map::offBounds(int x, int y) const{
    return ( x<0 || x>=this->xDim || y<0 || y>=this->yDim );
}

// need to translate coords to new map
bool Map::addRoom(const Coordinate& c){
    if(DEBUG)cout << "\nMap::addRoom " << c << " | LINE:" << __LINE__ << endl;
    if( offBounds(c.x,c.y) ){
        if(MAP_DEBUG)cout << "Map::addRoom OFF BOUNDS" << endl;
        return false;
    } // if off bounds
    if(this->numRooms>=(this->xDim*this->yDim)){
        if(MAP_DEBUG)cout << "Map::addRoom FULL" << endl;
        return false;
    } // if size is full

    if(this->numRooms==0){setStart(c);} // if first item in array make it starting location
    this->rooms[this->numRooms++] = c; // add to array & increment after
    return true;
}

// need to translate coords to new map
bool Map::addDoor(const Coordinate& c){
    if(DEBUG)cout << "\nMap::addDoor " << c << " | LINE:" << __LINE__ << endl;
    if( offBounds(c.x,c.y) ){
        if(MAP_DEBUG)cout << "Map::addDoor OFF BOUNDS" << endl;
        return false;

    }                        // if off bounds return
    if(this->numDoors>=(this->xDim*this->yDim)){
        return false;
        if(MAP_DEBUG)cout << "Map::addDoor FULL" << endl;
    } // if size is full

    this->doors[this->numDoors++] = c; // add to array & increment after
    return true;
}


void Map::copyMapData(const Map& map){
    if(DEBUG)cout << "\nMap::copyMapData | LINE:" << __LINE__ << endl;

    if( this->xDim != map.getXDim() || this->yDim!= map.getYDim() ){
        string error = "Map::copyMapData Dimension mismatch of map to copy ";
        throw error;
    }

    for(int y = 0; y<this->yDim;++y){
        for(int x = 0; x<this->xDim;++x){
            this->mapData[y][x] = map.mapData[y][x];
        }
    }

    for(int i = 0; i < map.numDoors; ++i){
        addDoor(map.doors[i]);
    }
    for(int j = 0; j < map.numRooms; ++j){
        addRoom(map.rooms[j]);
    }

    setStart(map.getStart()); // set the starting location to the same
}

// print full map with the center location emphasized as @ char
void Map::print() const{
    Coordinate c = getStart();
    Coordinate lastRoom = this->rooms[this->numRooms-1];

    for(int y = 0; y<this->yDim;++y){
        for(int x = 0; x<this->xDim;++x){

            if(this->mapData[y][x]==0){cout << " .";}
            else{
                if(c.x==x && c.y==y){
                    cout << " @";    // print start location
                }else if(lastRoom.x==x && lastRoom.y==y){
                    cout << " G"; // print last room added for option to add additional element to map -> eg PLAYER 2 SPAWN
                }else{
                    cout << " "<< this->mapData[y][x];
                }
            }

        }
        cout << endl;
    }

}

void Map::printRooms() const{
    cout << "\nALL ROOMS" << endl;
    for(int i = 0; i<this->numRooms;++i){
        cout << this->rooms[i] << endl;
    }
}

void Map::printDoors() const{
    cout << "\nALL DOORS" << endl;
    for(int i = 0; i<this->numDoors;++i){
        cout << this->doors[i] << endl;
    }
}
