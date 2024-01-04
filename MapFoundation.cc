#include "MapFoundation.h"


MapFoundation::MapFoundation(ExpandedMatrix* exMatrix){
    this->matrix = exMatrix;

    this->rooms = new Coordinate[matrix->getMatrixSize()];
    this->numRooms = 0;

    this->doors = new Coordinate[matrix->getMatrixSize()];
    this->numDoors = 0;

    // recursive walk through matrix -> store max coord top left max coord bottom right -> to crop -> make make a walk object
    // store visited
    // handle loops
    int center = matrix->getCenterCoordEx();
    Coordinate currCoord(center,center);

    this->coordMostN = center;
    this->coordMostS = center;
    this->coordMostE = center;
    this->coordMostW = center;

    this->finalMap = nullptr;

    matrixWalk(currCoord);
    filterIsolated();
    makeMap();
}

MapFoundation::~MapFoundation(){
    delete [] this->rooms;
    delete [] this->doors;

    if(this->finalMap){
        delete this->finalMap;
    }
}

void MapFoundation::matrixWalk(Coordinate& currCoord){
    storeBounds(currCoord);
    int nextDir = DIRECTION_COUNT;
    bool first = true;
    for (int i = 0; i < DIRECTION_COUNT; ++i) {
        Direction dir = static_cast<Direction>(i);
        Coordinate tempCoord;

        int move = 1; // from current position move this many coordinate values
        tempCoord.setInDirection(currCoord,dir,move);
        if(matrix->checkElementEx(tempCoord, DOOR)){ // if door in that direction
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
    if(matrix->checkElementEx(currCoord, ROOM)){
        matrix->modCoordElement(currCoord,VISITED); // visit this room
    }
    Direction dir = static_cast<Direction>(nextDir);
    if(dir!=DIRECTION_COUNT){
        // mod door
        currCoord.setInDirection(currCoord,dir); // visit next door too
        matrix->modCoordElement(currCoord,VISITED);

        currCoord.setInDirection(currCoord,dir); // new element
        matrixWalk(currCoord);
    } else{
        if(DEBUG)cout << "*DeadEnd*" << endl;
        Coordinate* temp = getNextPath();

        if(temp){
            currCoord = (*temp);
            matrixWalk(currCoord);
        } else{
            checkDoors(); // check if any doors were not visited
        }
    }
}

Coordinate* MapFoundation::getNextPath(){
    for(int i = 0; i<this->numRooms; ++i){
        if(matrix->checkElementEx(this->rooms[i], ROOM)){
            return &this->rooms[i];
        }
    }
    return nullptr;
}

void MapFoundation::checkDoors(){
    for(int i = 0; i<this->numDoors; ++i){
        if(matrix->checkElementEx(this->doors[i], DOOR)){
            matrix->modCoordElement(this->doors[i],VISITED);
            if(DEBUG)cout<<this->doors[i]<<" was missed"<<endl;
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

void MapFoundation::filterIsolated(){
    for(int i = 0; i< matrix->getDimension();++i){
        for(int j = 0; j<matrix->getDimension(); ++j){
            Coordinate currCoord(j,i);
            int cell = matrix->get(currCoord);
            switch(cell){
                case ROOM + VISITED:
                case DOOR + VISITED:
                    if(DEBUG)cout << currCoord << " stored " << endl;
                    break;
                default:
                    matrix->clearCoordinate(currCoord);
                    if(DEBUG)cout << currCoord << " cleared " << endl;
                    break;
            }
        }
    }
}

void MapFoundation::printBoundsAbsolute() const{
    if(DEBUG)cout << "N: " << this->coordMostN << " | E: " << this->coordMostE << " | S: " << this->coordMostS << " | W: " << this->coordMostW << endl;
    for(int i = this->coordMostN; i<=this->coordMostS;++i){
        for(int j = this->coordMostW; j<=this->coordMostE; ++j){
            int cell = matrix->get(j,i);
            if(cell==0){
                cout << ".";
            } else{
                cout << cell;
            }
        }
        cout<<endl;
    }
}

void MapFoundation::print() const{
    this->matrix->printEx();
}

void MapFoundation::makeMap() {
    if(DEBUG)cout << "N: " << this->coordMostN << " | E: " << this->coordMostE << " | S: " << this->coordMostS << " | W: " << this->coordMostW << endl;

    int sizeY = coordMostS - coordMostN + 1;
    int sizeX = coordMostE - coordMostW + 1;

    if(DEBUG)cout << "sizeY = coordMostS - coordMostN + 1  == " << sizeY << endl;
    if(DEBUG)cout << "sizeX = coordMostE - coordMostW + 1  == " << sizeX << endl;
    this->finalMap = new Map(sizeX,sizeY);

    int y = 0;
    for(int i = this->coordMostN; i<=this->coordMostS;++i){
        int x = 0;
        for(int j = this->coordMostW; j<=this->coordMostE; ++j){
            int cell = matrix->get(j,i);
            finalMap->set(x,y,cell);
            ++x;
        }
        x=0;
        ++y;
    }
}
