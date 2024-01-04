#include "Map.h"


Map::Map(int sizeX, int sizeY){
    if (sizeX <= 0 || sizeY <= 0) {
        throw invalid_argument("Invalid dimensions");
    }

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
}


Map::Map(MapFoundation* mapFoundation){

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

    for(int i = 0; i < mapFoundation->numDoors; ++i){
        addDoor(mapFoundation->doors[i]);
    }
    for(int j = 0; j < mapFoundation->numRooms; ++j){
        addRoom(mapFoundation->rooms[j]);
    }
}

Map::~Map(){
    delete [] this->doors;
    delete [] this->rooms;

    for(int y = 0; y<this->yDim; ++y){
        delete [] this->mapData[y];
    }
    delete [] this->mapData;
}

int Map::getYDim() const{
    return this->yDim;
}

int Map::getXDim() const{
    return this->xDim;
}

int Map::getNumRooms() const{
    return this->numRooms;
}

int Map::getNumDoors() const{
    return this->numDoors;
}

void Map::set(int x, int y, int value){
    if(offBounds(x,y)){return;}
    this->mapData[y][x] = value;
}

bool Map::offBounds(int x, int y) const{
    return (x<0||x>=this->xDim||y<0||y>=this->yDim);
}

bool Map::addRoom(const Coordinate& c){
    if(offBounds(c.x(),c.y())){return false;}
    if(this->numRooms>=(this->xDim*this->yDim)){return false;}

    this->rooms[++this->numRooms] = Coordinate(c);
    return true;
}

bool Map::addDoor(const Coordinate& c){
    if(offBounds(c.x(),c.y())){return false;}
    if(this->numDoors>=(this->xDim*this->yDim)){return false;}

    this->doors[++this->numDoors] = Coordinate(c);
    return true;
}

void Map::copyMapData(const Map& map){
    if( this->xDim != map.getXDim() || this->yDim!= map.getYDim() ){
        string error = "Map::copyMapData Dimension mismatch of map to copy ";
        throw error;
    }

    for(int y = 0; y<this->yDim;++y){
        for(int x = 0; x<this->xDim;++x){
            this->mapData[y][x] = map.mapData[y][x];
        }
    }
}

void Map::print() const{
    for(int y = 0; y<this->yDim;++y){
        for(int x = 0; x<this->xDim;++x){
            if(this->mapData[y][x]==0){cout << ".";}
            else{cout << this->mapData[y][x];}
        }
        cout << endl;
    }
}
