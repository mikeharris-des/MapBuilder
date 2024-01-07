#include "MapFoundation.h"


MapFoundation::MapFoundation(ExpandedMatrix* exMatrix){
    if(DEBUG)cout << "\nMapFoundation::ctor |" << __LINE__ << endl;

    this->matrix = exMatrix;

    this->rooms = new Coordinate[matrix->getMatrixSize()]; // dynamically allocated array of coordinate objects
    this->numRooms = 0;

    this->doors = new Coordinate[matrix->getMatrixSize()]; // dynamically allocated array of coordinate objects
    this->numDoors = 0;

    int center = matrix->getCenterCoordEx();
    Coordinate currCoord(center,center);

    // store all coord bounds as the center coord as all should be updated (minimum matrix is 5X5)
    this->coordMostN = center;
    this->coordMostS = center;
    this->coordMostE = center;
    this->coordMostW = center;

    this->finalMap = nullptr; // allocated in makeMap

    matrixWalk(currCoord);
    filterIsolated();
    makeMap();
    if(this->finalMap){
        Coordinate mapStart(center,center);
        if(DEBUG)cout << " OLD CENTER " << mapStart << endl;
        this->finalMap->setStart(translateCoordMatrixToMap(mapStart));
        if(DEBUG)cout << " NEW START " << this->finalMap->getStart() << endl;
    }

}

MapFoundation::~MapFoundation(){
    if(DEBUG)cout << "\nMapFoundation::dtor |" << __LINE__ << endl;
    delete [] this->rooms; // dynamically allocated array of coordinate objects
    delete [] this->doors; // dynamically allocated array of coordinate objects

    if(this->finalMap){
        delete this->finalMap; // dynamically allocated pointer to map object
    }
}

/*
    Recursive function to walk through this->matrix starting at the centre coord (defaulted to a room element)
        Algorithm:

            calls storeBounds();
                * check this coordinate if its x coord is < coordMostW || > coordMostE
                * check this coordinate if its y coord is < coordMostN || > coordMostS
                    - if true to any 4 updates the respective member coord with new x and/or y

            check all directions (ENWS) from the current coordinate

                * check for a door 1 index away in the direction being checked
                    - if the first door of the directional search store that direction (next recursive call will be in that direction)
                    - if a door exists add the door to door array
                    - move 1 index in the same direction and add that room ( garunteed to be room there from ExpandedMatrix )

            Mark the current room as visited if not already visited

            check if there is a valid next direction to move to
                * if so mark the door inbetween as visited and call matrixWalk on the room in that direction

            else = deadend
                * call getNextPath()
                    iterates through room array if there is a room whose cell != visted return that room coordinate
                * if getNextPath coordinate exists call matrixWalk() on that coordinate
                * else finish recursion by checking door array if any doors were missed -> doors marked visited only if walked passed to room in direction


*/
void MapFoundation::matrixWalk(Coordinate& currCoord){
    if(DEBUG)cout << "\nMapFoundation::matrixWalk | " << __LINE__ << " | " << currCoord<< endl;
    storeBounds(currCoord);
    int nextDir = DIRECTION_COUNT;
    bool first = true;
    for (int i = 0; i < DIRECTION_COUNT; ++i) { // check all directions (ENWS) from curr coord
        Direction dir = static_cast<Direction>(i);
        Coordinate tempCoord;

        int move = 1; // from current position move this many coordinate values
        tempCoord.setInDirection(currCoord,dir,move); // move one index from the current coordinate to the direction specified
        if(this->matrix->checkElementEx(tempCoord, DOOR)){ // if door in that direction
            if(first){ // move consistent order direction first
                nextDir = i; // first valid direction to move
                first = false;
            }
            // store seen
            addUniqueDoor(tempCoord);// add door
            ++move; // increment to +1 for the room in that direction
            tempCoord.setInDirection(currCoord,dir,move); // set this coordinate to the coordinate in the respective direction at move number of indecees
            addUniqueRoom(tempCoord); // add room

        } else{
            if(DEBUG)cout << "_______________BH: " << tempCoord << endl; // message showing boundry hit
        }
    }

    // Mark the current room as visited if not already visited
    if(matrix->checkElementEx(currCoord, ROOM)){
        matrix->modCoordElement(currCoord,VISITED); // visit this room
    }

    Direction dir = static_cast<Direction>(nextDir);
    /*
    check if there is a valid next direction to move to
        * if so mark the door inbetween as visited and call matrixWalk on the room in that direction
    */
    if(dir!=DIRECTION_COUNT){ // in !default dir assignment (new direction assigned and not deadend)
        // mark door visited
        currCoord.setInDirection(currCoord,dir);
        matrix->modCoordElement(currCoord,VISITED);

        currCoord.setInDirection(currCoord,dir); // new element
        matrixWalk(currCoord);

    } else{ // dead end
        if(DEBUG)cout << "*DeadEnd*" << endl; // dead end hit
        //iterates through room array if there is a room whose cell != visted return that room coordinate
        Coordinate* temp = getNextPath();
        // ^ this is a pointer to the array index not a new coordinate
        if(temp){ // if there exists a getNextPath coordinate
            currCoord = (*temp);
            matrixWalk(currCoord); // recursive call to walk the matrix at the new coordinate (unvisited)
        } else{
            // recursion is done
            checkDoors(); // check if any doors (very few if any) were not visited do to bidirectional walking
        }
    }
}

// check all rooms previously seen in the consistent priority order (ENWS) if they have not been visited
Coordinate* MapFoundation::getNextPath(){
    if(DEBUG)cout << "\nMapFoundation::getNextPath |" << __LINE__ << endl;
    for(int i = 0; i<this->numRooms; ++i){
        if(matrix->checkElementEx(this->rooms[i], ROOM)){
            return &this->rooms[i];
        }
    }
    return nullptr;
}

// if any doors were not marked visited in the doors array of all visited -> used for a door initialization confirmation
void MapFoundation::checkDoors(){
    if(DEBUG)cout << "\nMapFoundation::checkDoors |" << __LINE__ << endl;
    for(int i = 0; i<this->numDoors; ++i){
        if(matrix->checkElementEx(this->doors[i], DOOR)){
            matrix->modCoordElement(this->doors[i],VISITED);
            if(DEBUG)cout<<this->doors[i]<<" was missed"<<endl;
        }
    }
}


/*
    * check coordinate if its x coord is < coordMostW || > coordMostE
    * check coordinate if its y coord is < coordMostN || > coordMostS
        - if true to any 4 updates the respective member coord with new x and/or y
*/
void MapFoundation::storeBounds(const Coordinate& currCoord){
    if(DEBUG)cout << "MapFoundation::storeBounds |" << __LINE__ << endl;
    int x = currCoord.x();
    int y = currCoord.y();
    if(y < this->coordMostN){ // closest to y == 0; or TOP
        this->coordMostN = y;
    }
    if(y > this->coordMostS){ // closest to y == dimensionEx or BOTTOM
        this->coordMostS = y;
    }
    if(x < this->coordMostW){ // closest to x == 0; or LEFT
        this->coordMostW = x;
    }
    if(x > this->coordMostE){ // closest to x == dimensionEx; or RIGHT
        this->coordMostE = x;
    }
}

// add if not already present in member array add
bool MapFoundation::addUniqueDoor(const Coordinate& currCoord){
    if(DEBUG)cout << "\nMapFoundation::addUniqueDoor |" << __LINE__ << endl;
    for(int i = 0; i<this->numDoors;++i){
        if(this->doors[i]==currCoord){
            if(DEBUG)cout << " duplicate found no door add @ "<< currCoord << endl;
            return false;
        }
    }
    this->doors[this->numDoors++] = currCoord;
    return true;
}

// add if not already present in member array add
bool MapFoundation::addUniqueRoom(const Coordinate& currCoord){
    if(DEBUG)cout << "\nMapFoundation::addUniqueRoom |" << __LINE__ << endl;
    for(int i = 0; i<this->numRooms;++i){
        if(this->rooms[i]==currCoord){
            if(DEBUG)cout << " duplicate found no room add @ "<< currCoord<< endl;
            return false;
        }
    }
    this->rooms[this->numRooms++] = currCoord;
    return true;
}

// if there is a element integer on the matrix that is not marked visited, remove it from the matrix (isolated from mapWalk)
void MapFoundation::filterIsolated(){
    if(DEBUG)cout << "\nMapFoundation::filterIsolated |" << __LINE__ << endl;
    for(int i = 0; i< matrix->getDimension();++i){
        for(int j = 0; j<matrix->getDimension(); ++j){
            Coordinate currCoord(j,i);
            int cell = matrix->get(currCoord);
            switch(cell){
                case ROOM + VISITED:
                case DOOR + VISITED:
                    if(DEBUG)cout << currCoord << "             stored " << endl;
                    break;
                default:
                    matrix->clearCoordinate(currCoord); // make cell 0
                    break;
            }
        }
    }
}

void MapFoundation::printBoundsAbsolute() const{
    if(DEBUG)cout << "\nMapFoundation::printBoundsAbsolute |" << __LINE__ << endl;
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
    if(DEBUG)cout << "\nMapFoundation::makeMap |" << __LINE__ << endl;
    if(DEBUG)cout << "N: " << this->coordMostN << " | E: " << this->coordMostE << " | S: " << this->coordMostS << " | W: " << this->coordMostW << endl;

    int sizeY = this->coordMostS - this->coordMostN + 1; // + 1 for array 0 indexing
    int sizeX = this->coordMostE - this->coordMostW + 1;

    if(DEBUG)cout << "sizeY = coordMostS - coordMostN + 1  == " << sizeY << endl;
    if(DEBUG)cout << "sizeX = coordMostE - coordMostW + 1  == " << sizeX << endl;
    this->finalMap = new Map(sizeX,sizeY);

    int y = 0;
    for(int i = this->coordMostN; i<=this->coordMostS;++i){
        int x = 0;
        for(int j = this->coordMostW; j<=this->coordMostE; ++j){
            int cell = matrix->get(j,i);
            switch(cell){   // here setting to DOOR or ROOM integer only
                case DOOR+VISITED:
                    this->finalMap->set(x,y,DOOR);
                    break;
                case ROOM+VISITED:
                    this->finalMap->set(x,y,ROOM);
                    break;
                default:
                    this->finalMap->set(x,y,cell);
                    break;
            }
            ++x;
        }
        x=0;
        ++y;
    }

    /*
        rooms stored in og map order bellow for purposes of ordering based on center coord(start room)
    */
    for(int k = 0; k< this->numRooms; ++k){
        if(!offBoundsFinalMap(this->rooms[k])){
            Coordinate roomCoord = translateCoordMatrixToMap(this->rooms[k]); // translate coordinates to the final map bounds
            if(DEBUG)cout << " ROOM COORD IS " << roomCoord << endl;
            this->finalMap->addRoom(roomCoord);
        } else{
            if(DEBUG)cout << "off bounds: " << this->rooms[k] << endl;
        }
    }
    if(DEBUG)cout << "\n-------------------------------------------" << endl;


    for(int l = 0; l< this->numDoors; ++l){
        if(!offBoundsFinalMap(this->doors[l])){
            Coordinate doorCoord = translateCoordMatrixToMap(this->doors[l]); // translate coordinates to the final map bounds
            if(DEBUG)cout << " DOOR COORD IS " << doorCoord << endl;
            this->finalMap->addDoor(doorCoord);
        } else{
            if(DEBUG)cout << "off bounds: " << this->doors[l] << endl;
        }
    }


}
//
Coordinate MapFoundation::translateCoordMatrixToMap(const Coordinate& currCoord){
    if(DEBUG)cout << "\nMapFoundation::translateCoordMatrixToMap |" << __LINE__ << endl;
    int xRoom = currCoord.x();
    int yRoom = currCoord.y();
    Coordinate c(xRoom - this->coordMostW,yRoom - this->coordMostN);
    if(DEBUG)cout << currCoord << " -> " << c;
    return c;
}

bool MapFoundation::offBoundsFinalMap(const Coordinate& currCoord) const{
    return (currCoord.y()<this->coordMostN || currCoord.y()>this->coordMostS || currCoord.x()<this->coordMostW || currCoord.x()>this->coordMostE);
}
