#include "MapFoundation.h"



MapFoundation::MapFoundation(ExpandedMatrix* exMatrix){
    if(DEBUG)cout << "\nMapFoundation::ctor |" << __LINE__ << endl;

    this->expandedMatrix = exMatrix; // assign the base matrix of this object to one passed in constructor

    this->rooms = new Coordinate[this->expandedMatrix->getNumCells()]; // dynamically allocated array of coordinate objects
    this->numRooms = 0;

    this->doors = new Coordinate[this->expandedMatrix->getNumCells()]; // dynamically allocated array of coordinate objects
    this->numDoors = 0;

    int center = this->expandedMatrix->getCenterCoordExpanded(); // center coordinate of the matrix x==y always
    Coordinate currCoord(center,center);

    // store all final coord bounds as the center coord as all should be updated (minimum matrix is 5X5)
    this->coordMostN = center; // assign min y to center y coord
    this->coordMostS = center; // assign max y to center y coord
    this->coordMostE = center; // assign max x to center x coord
    this->coordMostW = center; // assign min x to center x coord

    this->finalMap = nullptr; // allocated in makeMap

    matrixWalk(currCoord); // recursive function to walk through map and mark all connected rooms and doors
    filterIsolated(); // remove after any rooms or doors not visited (isolated from main branch)
    makeMap(); // make finalMap member variable -> crops map if necessary (uses the max coordinates ENSW and crops to those) -> adds extra cells to make square matrix y=x

    if(this->finalMap){ // if finalMap is made

        Coordinate mapStart(center,center);
        if(MF_DEBUG)cout << " OLD CENTER " << mapStart << endl;

        this->finalMap->setStart(translateCoordMatrixToMap(mapStart)); // translate coordinate to new map if it is cropped -> set this previous center coordinate as start

        if(MF_DEBUG)cout << " NEW START  " << this->finalMap->getStart() << endl;
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

            Step 1: Store bounds for cropping at the end

            Step 2: Explore all directions from the current coordinate in constant order (EAST then NORTH then WEST then SOUTH)
                2a. * mark any connected doors and their respective rooms in that direction as seen (and was not visited) by adding them to either global sets of seen doors and rooms

            Step 3: Mark the current room as visited if not already visited

            Step 4: Check the first door marked seen from Step 2 if it exists
                4a. if a new door exists mark it as visited and call matrixWalk on the doors corresponding room
                4b. if no new door exists handle dead end

                    Step 5: On DeadEnd Event, check the the global set of seen rooms in the order they were first seen for any unvisited rooms
                        5a. if there exists a room seen but not visited call matrixWalk on that rooms coordinate
                        5b. if all seen rooms are visited, ensure all seen doors were marked visited and break from recursive calling

*/
void MapFoundation::matrixWalk(Coordinate& currCoord){
    if(DEBUG)cout << "\nMapFoundation::matrixWalk | " << __LINE__ << " | " << currCoord<< endl;

    // Step 1: Store bounds for cropping at the end
    storeBounds(currCoord); // checks the max x,y for ENSW coordinates -> store max for cropping at the end

    int nextDir = Direction::DIRECTION_COUNT; // default if nextDir isnt assigned to a valid direction == dead end hit

    bool first = true;
    if(MF_DEBUG)cout << " \n____________________________  " << endl;
    // Step 2: Explore all directions from the current coordinate in constant order (EAST then NORTH then WEST then SOUTH)
    for (int i = 0; i < Direction::DIRECTION_COUNT; ++i) { // check all directions (ENWS) from curr coord
        Direction::Value dir = static_cast<Direction::Value>(i);

        Coordinate tempCoord(currCoord); // copy curr cord

        // int move = 1; // from current position move this many coordinate values
        tempCoord.set(dir); // move one cell from the current coordinate to the direction specified

        if(MF_DEBUG)cout << currCoord << " -> " << Direction::toString(dir) << " = " << tempCoord << " ";
        if(this->expandedMatrix->checkElementExpanded(tempCoord, DOOR)){ // if door is at that direction
            if(first){ // move consistent order direction first
                nextDir = i; // first valid direction to move
                first = false;
            }
            // 2a. * mark any connected doors and their respective rooms in that direction as seen (and was not visited) by adding them to either global sets of seen doors and rooms
            addUniqueDoor(tempCoord);// add door

            tempCoord.set(dir); // set this coordinate to the coordinate in the respective direction to get room
            addUniqueRoom(tempCoord); // add room
            if(MF_DEBUG)cout << endl;

        } else{
            if(MF_DEBUG)cout << " is not a door " << endl; // message showing boundry hit
        }
    }
    if(MF_DEBUG)cout << " \n____________________________  " << endl;

    // Step 3: Mark the current room as visited if not already visited
    if(this->expandedMatrix->checkElementExpanded(currCoord, ROOM)){
        if(MF_DEBUG)cout << currCoord << " MARKED VISITED " << endl;
        this->expandedMatrix->markCellVisited(currCoord); // visit this room
    } else {
        if(MF_DEBUG)cout << " CURRCORD  "<< currCoord  << " = "<< this->expandedMatrix->get(currCoord) << endl;
    }

    Direction::Value dir = static_cast<Direction::Value>(nextDir);
    /*
    Step 4: Check the first door marked seen from Step 2 if it exists
        4a. if a new door exists mark it as visited and call matrixWalk on the doors corresponding room
    */
    if( dir != Direction::DIRECTION_COUNT ){ // in !default dir assignment (new direction assigned and not deadend)

        currCoord.set(dir); // increment to adjacent cell that is door
        this->expandedMatrix->markCellVisited(currCoord); // mark door visited

        currCoord.set(dir);  // increment to adjacent cell that is room
        matrixWalk(currCoord); // recursively call walk on that coordinate that is an unvisited room

    } else{ // 4b. if no new door exists handle dead end
        if(MF_DEBUG)cout << "*DeadEnd* @" << currCoord << endl;

        // Step 5: On DeadEnd Event, check the the global set of seen rooms in the order they were first seen for any unvisited rooms
        Coordinate* temp = getNextPath();  //iterates through room array if there is a room whose cell != visted return that room coordinate
        // ^ this is a pointer to the array index not a new coordinate

        if(temp){ // 5a. if there exists a room seen but not visited call matrixWalk on that rooms coordinate
            currCoord = (*temp);
            matrixWalk(currCoord); // recursive call to walk the matrix at the new coordinate (unvisited)
        } else{
            // 5b. if all seen rooms are visited, ensure all seen doors were marked visited and break from recursive calling
            checkDoors(); // check if any doors (very few if any) were not visited do to bidirectional walking and only marking door visited if walking through it
        }
    }
}

// check all rooms previously seen in the consistent priority order (ENWS) if they have not been visited
Coordinate* MapFoundation::getNextPath(){
    if(DEBUG)cout << "\nMapFoundation::getNextPath |" << __LINE__ << endl;

    for(int i = 0; i<this->numRooms; ++i){

        if( this->expandedMatrix->checkElementExpanded(this->rooms[i], ROOM) ){
            return &this->rooms[i];
        }
    }
    return nullptr; // handled in MapFoundation::matrixWalk so no issue
}

// if any doors were not marked visited in the doors array of all visited -> used for a door initialization confirmation
void MapFoundation::checkDoors(){
    if(DEBUG)cout << "\nMapFoundation::checkDoors |" << __LINE__ << endl;

    for(int i = 0; i<this->numDoors; ++i){

        if(this->expandedMatrix->checkElementExpanded(this->doors[i], DOOR)){ // if any doors != DOORS + VISITED at coordinate cell value

            this->expandedMatrix->markCellVisited(this->doors[i]); //if so mark visited

            if(MF_DEBUG)cout<<this->doors[i]<<" was missed"<<endl;
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

    if(currCoord.y < this->coordMostN){ // closest to y == 0; or TOP
        this->coordMostN = currCoord.y;
    }
    if(currCoord.y > this->coordMostS){ // closest to y == dimensionEx or BOTTOM
        this->coordMostS = currCoord.y;
    }
    if(currCoord.x < this->coordMostW){ // closest to x == 0; or LEFT
        this->coordMostW = currCoord.x;
    }
    if(currCoord.x > this->coordMostE){ // closest to x == dimensionEx; or RIGHT
        this->coordMostE = currCoord.x;
    }
}

// add if not already present in member array add
bool MapFoundation::addUniqueDoor(const Coordinate& currCoord){
    if(DEBUG)cout << "\nMapFoundation::addUniqueDoor |" << __LINE__ << endl;

    // no bounds check ! handled in context
    for(int i = 0; i<this->numDoors;++i){

        if(this->doors[i]==currCoord){
            if(MF_DEBUG)cout << " duplicate found no door add @ "<< currCoord << endl;
            return false;
        }

    }
    this->doors[this->numDoors++] = currCoord;
    if(MF_DEBUG)cout << currCoord << " -> " << this->doors[this->numDoors] << " = num doors " << this->numDoors << " ";
    return true;
}

// add if not already present in member array add
bool MapFoundation::addUniqueRoom(const Coordinate& currCoord){
    if(DEBUG)cout << "\nMapFoundation::addUniqueRoom |" << __LINE__ << endl;

    // no bounds check ! handled in context
    for(int i = 0; i<this->numRooms;++i){
        if(this->rooms[i]==currCoord){
            if(MF_DEBUG)cout << " duplicate found no room add @ "<< currCoord<< endl;
            return false;
        }
    }

    this->rooms[this->numRooms++] = currCoord;
    if(MF_DEBUG)cout << currCoord << " -> " << this->rooms[this->numRooms] << " = num rooms " << this->numRooms << " ";

    return true;
}

// if there is a element integer on the matrix that is not marked visited, remove it from the matrix (isolated from mapWalk)
void MapFoundation::filterIsolated(){
    if(DEBUG)cout << "\nMapFoundation::filterIsolated |" << __LINE__ << endl;

    for(int y = 0; y < this->expandedMatrix->getDimension(); ++y){
        for(int x = 0; x < this->expandedMatrix->getDimension(); ++x){

            Coordinate currCoord(y,x);
            int cell = this->expandedMatrix->get(currCoord); // get value from expanded matrix
            switch(cell){
                case ROOM:
                case DOOR:
                    this->expandedMatrix->clearCell(currCoord); // make cell 0 if isolated
                    if(MF_DEBUG)cout << currCoord << "             cleared " << endl;
                    break;
                default:
                    if(MF_DEBUG)cout << currCoord << "             stored " << endl;
                    break;
            }

        }
    }

}

// creates the Map object from this mapFoundation without unnecessary building member variables or functions for abstraction and usability
// takes the coordMost bounds and translates all of this MapFoundation matrix cropped to a newly allocated finalMap matrix object
void MapFoundation::makeMap() {
    if(MF_DEBUG_MAKEMAP)cout << "\nMapFoundation::makeMap |" << __LINE__ << endl;
    if(MF_DEBUG_MAKEMAP)cout << "N: " << this->coordMostN << " | E: " << this->coordMostE << " | S: " << this->coordMostS << " | W: " << this->coordMostW << endl;

    int sizeY = this->coordMostS - this->coordMostN + 1; // + 1 for array 0 indexing
    int sizeX = this->coordMostE - this->coordMostW + 1;

    if(MAKE_SQUARE){ // if final map must be square (for rendering ui is easiest if square) (const defined in MapFoundation.h)
        makeSquare(sizeX,sizeY); // pass in y and x dim of cropped map and take the largest one, make that both dim and translate to top left coordinate
    }else{
        this->finalMap = new Map(sizeX,sizeY); // make potentially skewed matrix
    }

    // this->finalMap = new Map(sizeX,sizeY);

    int y = 0; // counter for translating to finalMap y
    for(int i = this->coordMostN; i<=this->coordMostS;++i){
        int x = 0; // counter for translating to finalMap x
        for(int j = this->coordMostW; j<=this->coordMostE; ++j){

            int cell = this->expandedMatrix->get(j,i); // get will return -1 if off bounds of extended matrix member
            switch(cell){   // here setting to DOOR or ROOM integer only

                case (DOOR+VISITED):
                    // is a visited door
                    this->finalMap->set(x,y,DOOR); // change cell to any default door value desired
                    break;

                case (ROOM+VISITED):
                    // is a visited room
                    this->finalMap->set(x,y,ROOM); // change cell to any default room value desired
                    break;

                case OFF_BOUNDS: // if this cell is off bounds of the member extended matrix is expanded to square make it default 0
                    this->finalMap->set(x,y,0);
                    break;

                default:
                    // should only be 0 but lets other map values pass (cell)
                    this->finalMap->set(x,y,cell);  // change cell to any default map element value desired such as 0
                    break;
            }
            ++x; // increment x for translated map

        }
        x=0;    // reset x for translated map
        ++y; // increment y for translated map
    }

    if(MF_DEBUG_MAKEMAP)cout << "\n-------------------------------------------" << endl;

    /*
        rooms/doors stored in og map order bellow for purposes of ordering based on center coord(start room) walking priority order East, North, West, South
    */
    for(int k = 0; k< this->numRooms; ++k){

        if(!offBoundsFinalMap(this->rooms[k])){ // outta bounds for maxCoords

            Coordinate roomCoord = translateCoordMatrixToMap(this->rooms[k]); // translate coordinates to the final map bounds
            if(MF_DEBUG_MAKEMAP)cout << " ROOM COORD IS " << roomCoord << endl;
            this->finalMap->addRoom(roomCoord);

        } else{
            if(MF_DEBUG_MAKEMAP)cout << "off bounds: " << this->rooms[k] << endl;
        }

    }
    if(MF_DEBUG_MAKEMAP)cout << "\n-------------------------------------------" << endl;


    for(int l = 0; l< this->numDoors; ++l){

        if(!offBoundsFinalMap(this->doors[l])){
            Coordinate doorCoord = translateCoordMatrixToMap(this->doors[l]); // translate coordinates to the final map bounds
            if(MF_DEBUG_MAKEMAP)cout << " DOOR COORD IS " << doorCoord << endl;
            this->finalMap->addDoor(doorCoord);

        } else{
            if(MF_DEBUG_MAKEMAP)cout << "off bounds: " << this->doors[l] << endl;
        }

    }

}

// makes the final map a square matrix translating it so the top left corner aligns with maxN & maxW coordinates in foundational map AND dimensions are equal x=y for final map
void MapFoundation::makeSquare(int sizeX, int sizeY){

    int square = (sizeY>sizeX)?sizeY:sizeX; // get larger dimension to make square
    if(MF_DEBUG_MAKEMAP)cout << "\n square " << square << endl;
    if(MF_DEBUG_MAKEMAP)cout << " sizeY = coordMostS - coordMostN + 1  == " << sizeY << "   sizeX = coordMostE - coordMostW + 1  == " << sizeX << endl;

    // check which is bigger : extend South or East based on the opposite dimension being bigger
    if(square==sizeY){
        // if Y (S->N) length greater than X(W->E) modify X(W->E) to = Y by adding to East the difference
        if(MF_DEBUG_MAKEMAP)cout << " MODX: +" << (sizeY-sizeX) << endl << " mostEast: " << this->coordMostE  << endl << " newcells right: " << (sizeY-sizeX) << "x" << sizeY << endl;

        this->coordMostE += (sizeY-sizeX);      // add to E
        this->finalMap = new Map(square,square); // make new map with modified dimensions

    } else if(square==sizeX){
        // if X(W->E) length greater than Y(S->N) modify Y(S->N) to = X by adding to South the difference
        if(MF_DEBUG_MAKEMAP)cout << " MODY: +" << (sizeX-sizeY) << endl << " mostSouth: " << this->coordMostS << endl << " newcells bottom: " << (sizeX-sizeY) << "x" << sizeX << endl;

        this->coordMostS += (sizeX-sizeY); // add to S
        this->finalMap = new Map(square,square);// make new map with modified dimensions

    } else{
        this->finalMap = new Map(sizeX,sizeY); // if equal dimensions or issue make normal
    }

}

//translate the current coordinate to the cropped map coordinate
Coordinate MapFoundation::translateCoordMatrixToMap(const Coordinate& currCoord){
    if(DEBUG)cout << "\nMapFoundation::translateCoordMatrixToMap |" << __LINE__ << endl;

    int xRoom = currCoord.x - this->coordMostW; // takes the difference of the west most coordinate from the current coordinate
    int yRoom = currCoord.y - this->coordMostN; // takes the difference of the north most coordinate from the current coordinate

    Coordinate c(xRoom, yRoom);
    if(MF_DEBUG)cout << currCoord << " -> " << c;

    return c;
}

// if off bounds for cropped map
bool MapFoundation::offBoundsFinalMap(const Coordinate& currCoord) const{
    return ( currCoord.y<this->coordMostN || currCoord.y>this->coordMostS || currCoord.x<this->coordMostW || currCoord.x>this->coordMostE );
}

// prints map foundation but cropped to the coordMost values
void MapFoundation::printBoundsAbsolute() const{
    if(DEBUG)cout << "\nMapFoundation::printBoundsAbsolute |" << __LINE__ << endl;
    if(MF_DEBUG)cout << "N: " << this->coordMostN << " | E: " << this->coordMostE << " | S: " << this->coordMostS << " | W: " << this->coordMostW << endl;

    for(int i = this->coordMostN; i<=this->coordMostS;++i){
        for(int j = this->coordMostW; j<=this->coordMostE; ++j){

            int cell = this->expandedMatrix->get(j,i);
            if(cell==0){
                cout << " .";
            } else{
                cout << " " << cell;
            }

        }
        cout<<endl;
    }

}

void MapFoundation::print() const{
    this->expandedMatrix->printExpanded();
}
