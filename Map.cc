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

// get starting coordinate
Coordinate Map::getStart() const{
    return this->mapStart;
}

// get cell value of coordinate
int Map::get(const Coordinate& c){
    if(offBounds(c.x(),c.y())){return 0;}
    return this->mapData[c.y()][c.x()];
}

// set these x,y values in map to the value
void Map::set(int x, int y, int value){
    if(offBounds(x,y)){return;}
    this->mapData[y][x] = value;
}

// set starting coordinate
void Map::setStart(const Coordinate& c){
    int cell = get(c);
    if(cell==ROOM){
        this->mapStart = c;
    } else {
        cout << " * invalid set start location: " << c << endl;
    }
}

// need to translate coords to new map
bool Map::offBounds(int x, int y) const{
    if(DEBUG)cout << "Map::offBounds -> x: " << x << " y: " << y << endl;
    return (x<0||x>=this->xDim||y<0||y>=this->yDim);
}

// need to translate coords to new map
bool Map::addRoom(const Coordinate& c){
    if(DEBUG)cout << " Map::addRoom : " << c << endl;
    if(offBounds(c.x(),c.y())){return false;}
    if(this->numRooms>=(this->xDim*this->yDim)){return false;}
    if(this->numRooms==0){setStart(c);} // of first add make it the start
    this->rooms[this->numRooms++] = Coordinate(c); // copy constructor for array for
    return true;
}

// need to translate coords to new map
bool Map::addDoor(const Coordinate& c){
    if(DEBUG)cout << " Map::addDoor : " << c << endl;
    if(offBounds(c.x(),c.y())){return false;}
    if(this->numDoors>=(this->xDim*this->yDim)){return false;}

    this->doors[this->numDoors++] = Coordinate(c);
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
    int cx = c.x();
    int cy = c.y();
    for(int y = 0; y<this->yDim;++y){
        for(int x = 0; x<this->xDim;++x){
            if(this->mapData[y][x]==0){cout << ".";}
            else{
                if(cx==x && cy==y){
                    cout << "@";    // print start location
                }else{
                    cout << this->mapData[y][x];
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
