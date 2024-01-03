#include "MapFoundation.h"


MapFoundation::MapFoundation(ExpandedMatrix* exMatrix){
    this->map = exMatrix;

    this->rooms = new Coordinate[map->getMatrixSize()];
    this->numRooms = 0;

    this->doors = new Coordinate[map->getMatrixSize()];
    this->numDoors = 0;

    // recursive walk through map -> store max coord top left max coord bottom right -> to crop -> make make a walk object
    // store visited
    // handle loops
    int center = map->getCenterCoordEx();
    Coordinate currCoord(center,center);

    this->coordMostN = center;
    this->coordMostS = center;
    this->coordMostE = center;
    this->coordMostW = center;

    mapWalk(currCoord);
}

MapFoundation::~MapFoundation(){
    delete [] this->rooms;
    delete [] this->doors;
}

void MapFoundation::mapWalk(Coordinate& currCoord){
    storeBounds(currCoord);
    int nextDir = DIRECTION_COUNT;
    bool first = true;
    for (int i = 0; i < DIRECTION_COUNT; ++i) {
        Direction dir = static_cast<Direction>(i);
        Coordinate tempCoord;

        int move = 1; // from current position move this many coordinate values
        tempCoord.setInDirection(currCoord,dir,move);
        if(map->checkElementEx(tempCoord, DOOR)){ // if door in that direction
            if(first){ // move consistent
                nextDir = i;
                first = false;
            }
            // store seen

            // add door
            this->doors[numDoors++] = tempCoord;
            ++move;
            // add room
            tempCoord.setInDirection(currCoord,dir,move);
            this->rooms[numRooms++] = tempCoord; // dont need a second check

        } else{
            if(DEBUG)cout << "_______________BH: " << tempCoord << endl; // message showing boundry hit
        }
    }

    // store visited this element
    if(map->checkElementEx(currCoord, ROOM)){
        map->modCoordElement(currCoord,VISITED); // visit this room
    }
    Direction dir = static_cast<Direction>(nextDir);
    if(dir!=DIRECTION_COUNT){
        // mod door
        currCoord.setInDirection(currCoord,dir); // visit next door too
        map->modCoordElement(currCoord,VISITED);

        currCoord.setInDirection(currCoord,dir); // new element
        mapWalk(currCoord);
    } else{
        if(DEBUG)cout << "*DeadEnd*" << endl;
        Coordinate* temp = getNextPath();

        if(temp){
            currCoord = (*temp);
            mapWalk(currCoord);
        } else{
            checkDoors(); // check if any doors were not visited
        }
    }
}

Coordinate* MapFoundation::getNextPath(){
    for(int i = 0; i<this->numRooms; ++i){
        if(map->checkElementEx(this->rooms[i], ROOM)){
            return &this->rooms[i];
        }
    }
    return nullptr;
}

void MapFoundation::checkDoors(){
    for(int i = 0; i<this->numDoors; ++i){
        if(map->checkElementEx(this->doors[i], DOOR)){
            map->modCoordElement(this->doors[i],VISITED);
            cout<<this->doors[i]<<" was missed"<<endl;
        }
    }
}

void MapFoundation::storeBounds(const Coordinate& currCoord){
    int x = currCoord.x();
    int y = currCoord.y();
    if(y < this->coordMostN){ // closest to y == 0;
        this->coordMostN = y;
    }
    if(y > this->coordMostS){ // closest to y == dimensionEx
        this->coordMostS = y;
    }
    if(x < this->coordMostW){ // closest to x == 0;
        this->coordMostW = x;
    }
    if(x > this->coordMostE){ // closest to x == dimensionEx;
        this->coordMostE = x;
    }
}

void MapFoundation::printBoundsAbsolute() const{
    cout << "N: " << this->coordMostN << " | E: " << this->coordMostE << " | S: " << this->coordMostS << " | W: " << this->coordMostW << endl;
    for(int i = this->coordMostN; i<=this->coordMostS;++i){
        for(int j = this->coordMostW; j<=this->coordMostE; ++j){
            int out = map->get(j,i);
            if(out==0){
                cout << ".";
            } else{
                cout << out;
            }
        }
        cout<<endl;
    }
}

void MapFoundation::print() const{
    this->map->printEx();
}
