#include "MapFoundation.h"

// TODO

MapFoundation::MapFoundation(ExpandedMatrix* exMatrix){
    if(DEBUG)cout << "\nMapFoundation::ctor |" << __LINE__ << endl;

    this->expandedMatrix = exMatrix; // assign the base matrix of this object to one passed in constructor

    int center = this->expandedMatrix->getCenterCoordExpanded(); // center coordinate of the matrix x==y always
    Coordinate currCoord(center,center);

    // coordMost boundries: initializes max coordinate bounds as the center coord to be compared with every x & y coordinate visited on matrix walk in graph
    this->coordMostN = center; // assign min y to center y coord
    this->coordMostS = center; // assign max y to center y coord
    this->coordMostE = center; // assign max x to center x coord
    this->coordMostW = center; // assign min x to center x coord

    this->finalMatrix = nullptr; // allocated/instantiated in makeMap
    this->finalNodes = new CoordinateArray();
    this->finalEdges = new CoordinateArray();

    addUniqueNode(currCoord); // add the center coord first to make it the default primary start location on map (first node of node array = default primary start location)
    matrixWalk(currCoord); // recursive function to walk through map and mark all connected nodes and edges

    /* makeMap()
    builds final map data
        -> crops map if necessary using the final coordMost bounds
        -> adds extra cells to make square n x n matrix using n = (y>x)?y:x
        -> removes any isolated nodes or edges within final cropping but are not connected by main graph
    */
    makeMap();

    if(DEBUG)cout << " OLD CENTER " << Coordinate(center,center) << " . NEW START  " << this->nodes[0] << endl; // translated starting coordinate

}

MapFoundation::~MapFoundation(){
    if(DEBUG)cout << "\nMapFoundation::dtor |" << __LINE__ << endl;

    delete this->finalEdges;
    delete this->finalNodes;
    if(this->finalMatrix){
        delete this->finalMatrix; // dynamically allocated pointer to map object
    }
}

/* MapFoundation::matrixWalk

recursive function to traverse the matrix, starting at centre most coordinate visiting every 1 connected by atleast one 2. Handles dead ends using 'this->nodes' array member storing seen but
not visited nodes. Moves in priority ordering of East (x+1), then North (y-1), then South (y+1), then West (x-1) from currCoord.

    Algorithm to walk through this->matrix starting at the centre coord (defaulted to a node element):

            Step 1: compare x,y values of current cell to largest x,y values previously visited for cropping at the end

            Step 2: Mark the current node as visited if not already visited

            Step 3: Explore all directions from the current coordinate in constant order (EAST then NORTH then WEST then SOUTH or ENWS)
                3a. * mark any connected edges and their respective nodes in that direction as seen (and was not visited) by adding them to either global sets of seen edges and nodes

            Step 4: Check the first edge marked seen from Step 2 if it exists
                4a. RECURSIVE STEP: if a new edge exists mark it as visited and call matrixWalk on the edges corresponding node
                4b. if no new edge exists handle dead end

                    Step 5: On DeadEnd Event, check the the global set of seen nodes in the order they were first seen for any unvisited nodes
                        5a. RECURSIVE STEP: if there exists a node seen but not visited call matrixWalk on that nodes coordinate
                        5b. BASE CASE: if all seen nodes are visited, ensure all seen edges were marked visited and break from recursive calling

*/

void MapFoundation::matrixWalk(Coordinate& currCoord){
    if(DEBUG)cout << "\nMapFoundation::matrixWalk | " << currCoord << " " << __LINE__ << endl;
    if(MF_DEBUG)cout << " \n____________________________  " << endl;

    // Step 1: compare x,y values of current cell to largest x,y values previously visited for cropping at the end
    storeBounds(currCoord); // checks the max x,y for ENSW coordinates

    // Step 2: Mark the current node as visited if not already visited
    if(this->expandedMatrix->checkElementExpanded(currCoord, NODE)){
        if(MF_DEBUG)cout << currCoord << " MARKED VISITED " << endl;
        this->expandedMatrix->markCellVisited(currCoord); // visit this node
    } else {
        if(MF_DEBUG)cout << " CURRCORD  "<< currCoord  << " = "<< this->expandedMatrix->get(currCoord) << endl;
    }

    int nextDir = Direction::DIRECTION_COUNT; // default if nextDir isnt assigned to a valid direction == dead end hit

    bool first = true;

    // Step 3: Explore all directions from the current coordinate in constant order (EAST then NORTH then WEST then SOUTH)
    for (int i = 0; i < Direction::DIRECTION_COUNT; ++i) { // check all directions (ENWS) from curr coord
        Direction::Value dir = static_cast<Direction::Value>(i);

        Coordinate tempCoord(currCoord); // copy curr cord
        tempCoord.set(dir); // move one cell from the current coordinate to the direction specified

        if(MF_DEBUG)cout << currCoord << " -> " << Direction::toString(dir) << " = " << tempCoord << " ";

        if(this->expandedMatrix->checkElementExpanded(tempCoord, EDGE)){ // if edge is at that direction
            if(first){ // move consistent order direction first
                nextDir = i; // first valid direction to move
                first = false;
            }
            // 3a. * mark any connected edges and their respective nodes in that direction as seen (and was not visited) by adding them to either global sets of seen edges and nodes
            addUniqueEdge(tempCoord);// add edge

            tempCoord.set(dir); // set this coordinate to the coordinate in the respective direction to get node
            addUniqueNode(tempCoord); // add node

        }
        if(MF_DEBUG)cout << endl;
    }
    if(MF_DEBUG)cout << " \n____________________________  " << endl;

    Direction::Value dir = static_cast<Direction::Value>(nextDir);
    /*
    Step 4: Check the first edge marked seen from Step 2 if it exists
        4a. RECURSIVE STEP: if a new edge exists mark it as visited and call matrixWalk on the edges corresponding node
    */
    if( dir != Direction::DIRECTION_COUNT ){ // in !default dir assignment (new direction assigned and not deadend)

        currCoord.set(dir); // increment to adjacent cell that is edge
        this->expandedMatrix->markCellVisited(currCoord); // mark edge visited

        currCoord.set(dir);  // increment to adjacent cell that is node
        matrixWalk(currCoord); // recursively call walk on that coordinate that is an unvisited node

    } else{ // 4b. if no new edge exists handle dead end
        if(MF_DEBUG)cout << "*DeadEnd* @" << currCoord << endl;

        // Step 5: On DeadEnd Event, check the the global set of seen nodes in the order they were first seen for any unvisited nodes
        Coordinate* temp = getNextPath();  //iterates through node array if there is a node whose cell != visted return that node coordinate
        // ^ this is a pointer to the array index not a new coordinate

        if(temp){ // 5a. RECURSIVE STEP: if there exists a node seen but not visited call matrixWalk on that nodes coordinate
            currCoord = (*temp);
            matrixWalk(currCoord); // recursive call to walk the matrix at the new coordinate (unvisited)
        } else{
            // 5b. BASE CASE: if all seen nodes are visited, ensure all seen edges were marked visited and break from recursive calling
            checkEdges(); // check if any edges (very few if any) were not visited do to bidirectional walking and only marking edge visited if walking through it
        }
    }
}

// check all nodes previously seen in the consistent priority order (ENWS) if they have not been visited
Coordinate* MapFoundation::getNextPath(){
    if(DEBUG)cout << "\nMapFoundation::getNextPath |" << __LINE__ << endl;

    for(int i = 0; i<this->nodes.getSize(); ++i){

        if( this->expandedMatrix->checkElementExpanded(this->nodes[i], NODE) ){
            return &this->nodes[i];
        }
    }
    return nullptr; // handled in MapFoundation::matrixWalk so no issue
}

// if any edges were not marked visited in the edges array of all visited -> used for a edge initialization confirmation
void MapFoundation::checkEdges(){
    if(DEBUG)cout << "\nMapFoundation::checkEdges |" << __LINE__ << endl;

    for(int i = 0; i<this->edges.getSize(); ++i){

        if(this->expandedMatrix->checkElementExpanded(this->edges[i], EDGE)){ // if any edges != EDGE + VISITED at coordinate cell value

            this->expandedMatrix->markCellVisited(this->edges[i]); //if so mark visited

            if(MF_DEBUG)cout<<this->edges[i]<<" was missed"<<endl;
        }

    }
}

/*
    * check coordinate if its x coord is < coordMostW || > coordMostE
    * check coordinate if its y coord is < coordMostN || > coordMostS
        - if true to any 4 updates the respective member coord with new x and/or y
*/
void MapFoundation::storeBounds(const Coordinate& currCoord){
    if(DEBUG)cout << "\nMapFoundation::storeBounds | " << currCoord << " " << __LINE__ << endl;

    if(currCoord.y < this->coordMostN){ // closest to y == 0; or TOP MOST
        this->coordMostN = currCoord.y;
    }
    if(currCoord.y > this->coordMostS){ // closest to y == y dimension of Expanded matrix or BOTTOM MOST
        this->coordMostS = currCoord.y;
    }
    if(currCoord.x < this->coordMostW){ // closest to x == 0; or LEFT MOST
        this->coordMostW = currCoord.x;
    }
    if(currCoord.x > this->coordMostE){ // closest to x == x dimension of Expanded matrix; or RIGHT MOST
        this->coordMostE = currCoord.x;
    }
}

// add if not already present in member container object add
bool MapFoundation::addUniqueEdge(const Coordinate& currCoord){
    if(DEBUG)cout << "\nMapFoundation::addUniqueEdge | " << currCoord << __LINE__ << endl;

    if(!this->edges.contains(currCoord)){
        this->edges+=currCoord;
        return true;
    }
    return false;
}

// add if not already present in member container object add
bool MapFoundation::addUniqueNode(const Coordinate& currCoord){
    if(DEBUG)cout << "\nMapFoundation::addUniqueNode |" << __LINE__ << endl;

    if(!this->nodes.contains(currCoord)){
        this->nodes+=currCoord;
        return true;
    }
    return false;
}


// if there is a element integer on the matrix that is not marked visited, remove it from the matrix (isolated from mapWalk)
void MapFoundation::filterIsolated(){
    if(DEBUG)cout << "\nMapFoundation::filterIsolated |" << __LINE__ << endl;

    for(int y = this->coordMostN; y<=this->coordMostS; ++y){
        for(int x = this->coordMostW; x<=this->coordMostE; ++x){

            Coordinate currCoord(x,y);
            int cell = this->expandedMatrix->get(currCoord); // get value from expanded matrix
            switch(cell){
                case NODE:
                case EDGE:
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

/*
takes the coordMost bounds and translates all of this MapFoundation matrix data cropped to a newly sized finalMap matrix object,
without unnecessary building member variables or functions for abstraction
*/
void MapFoundation::makeMap() {
    if(MF_DEBUG_MAKEMAP){
        cout << "\nMapFoundation::makeMap |" << __LINE__ << endl;
        cout << "N: " << this->coordMostN << " | E: " << this->coordMostE << " | S: " << this->coordMostS << " | W: " << this->coordMostW << endl;
    }

    int sizeY = this->coordMostS - this->coordMostN + 1; // + 1 for array 0 indexing
    int sizeX = this->coordMostE - this->coordMostW + 1;

    if(MAKE_SQUARE){ // if final map must be square (for rendering ui is easiest if square) (const defined in MapFoundation.h)
        makeSquare(sizeX,sizeY); // pass in y and x dim of cropped map and take the largest one, make that both dim and translate to top left coordinate
    }else{
        this->finalMatrix = new Matrix(sizeX,sizeY); // make potentially skewed matrix
    }

    filterIsolated(); // remove after any nodes or edges not visited & are isolated from main branch ( will only remove elements in coordMost bounds for efficiency )

    int y = 0; // counter for translating to finalMap y
    for(int i = this->coordMostN; i<=this->coordMostS;++i){
        int x = 0; // counter for translating to finalMap x
        for(int j = this->coordMostW; j<=this->coordMostE; ++j){

            int cell = this->expandedMatrix->get(j,i); // get will return -1 if off bounds of extended matrix member
            switch(cell){   // here setting to EDGE or NODE integer only

                case (EDGE+VISITED):
                    // is a visited edge
                    this->finalMatrix->setCell(x,y,EDGE); // change cell to any default EDGE value desired
                    break;

                case (NODE+VISITED):
                    // is a visited NODE
                    this->finalMatrix->setCell(x,y,NODE); // change cell to any default NODE value desired
                    break;

                case OFF_BOUNDS: // if this cell is off bounds of the member extended matrix is expanded to square make it default 0
                    this->finalMatrix->setCell(x,y,0);
                    break;

                default:
                    // should only be 0 but lets other map values pass (cell)
                    this->finalMatrix->setCell(x,y,cell);  // change cell to any default map element value desired such as 0
                    break;
            }
            ++x; // increment x for translated map

        }
        x=0;    // reset x for translated map
        ++y; // increment y for translated map
    }

    if(MF_DEBUG_MAKEMAP)cout << "\n-------------------------------------------" << endl;

    /*
        nodes/edges stored in og map order bellow for purposes of ordering based on center coord(starting node) walking priority order East, North, West, South
    */

    // iterates through all nodes & checks coordMost boundries to see if k coordinate is within the final cropped map, if it is translate it and add
    for(int k = 0; k< this->nodes.getSize(); ++k){

        if( !offBoundsFinalMap(this->nodes[k]) ){    // checks coordMost boundries to see if k coordinate is within the final cropped map

            Coordinate nodeCoord = translateCoordMatrixToMap(this->nodes[k]); // translate coordinates to the final map bounds
            *this->finalNodes += nodeCoord;

            if(MF_DEBUG_MAKEMAP)cout << " shifted nodeCoord " << nodeCoord << endl;

        } else if(MF_DEBUG_MAKEMAP)cout << "off bounds nodeCoord: " << this->nodes[k] << endl;

    }
    if(MF_DEBUG_MAKEMAP)cout << "\n-------------------------------------------" << endl;

    // iterates through all edges & checks coordMost boundries to see if l coordinate is within the final cropped map, if it is translate it and add
    for(int l = 0; l< this->edges.getSize(); ++l){

        if( !offBoundsFinalMap(this->edges[l]) ){     // this checks coordMost boundries to see if l coordinate is within the final cropped map

            Coordinate edgeCoord = translateCoordMatrixToMap(this->edges[l]); // translate coordinates to the final map
            *this->finalEdges += edgeCoord;

            if(MF_DEBUG_MAKEMAP)cout << " shifted edgeCoord " << edgeCoord << endl;

        } else if(MF_DEBUG_MAKEMAP)cout << " off bounds edgeCoord: " << this->edges[l] << endl;

    }

}

/*
passing in y and x dimensions of the cropped map, allocates memory for the finalMap Map instance as a square matrix using the the largest of the dimensions
and translate the corresponding mostCoord (mostSouth or mostEast) to reflect the square cropping.
This ensures no map data is lost and also the squaring of the final map can be used consistently and easily for implementing a square gui window display
*/
void MapFoundation::makeSquare(int sizeX, int sizeY){

    int square = (sizeY>sizeX)?sizeY:sizeX; // get larger dimension to make square

    if(MF_DEBUG_MAKEMAP){
        cout << "\n square " << square << endl;
        cout << " sizeY = coordMostS - coordMostN + 1  == " << sizeY << "   sizeX = coordMostE - coordMostW + 1  == " << sizeX << endl;
    }

    // check which is bigger : extend South or East based on the opposite dimension being bigger
    if(square==sizeY){
        // if Y (S->N) length greater than X(W->E) modify X(W->E) to = Y by adding to East the difference
        if(MF_DEBUG_MAKEMAP)cout << " MODX: +" << (sizeY-sizeX) << endl << " mostEast: " << this->coordMostE  << endl << " newcells right: " << (sizeY-sizeX) << "x" << sizeY << endl;

        this->coordMostE += (sizeY-sizeX);      // add to E
        this->finalMatrix = new Matrix(square,square); // make new map with modified dimensions

    } else if(square==sizeX){
        // if X(W->E) length greater than Y(S->N) modify Y(S->N) to = X by adding to South the difference
        if(MF_DEBUG_MAKEMAP)cout << " MODY: +" << (sizeX-sizeY) << endl << " mostSouth: " << this->coordMostS << endl << " newcells bottom: " << (sizeX-sizeY) << "x" << sizeX << endl;

        this->coordMostS += (sizeX-sizeY); // add to S
        this->finalMatrix = new Matrix(square,square);// make new map with modified dimensions

    } else{
        this->finalMatrix = new Matrix(sizeX,sizeY); // if equal dimensions or issue make normal
    }

}

// takes the value at that coordinate and correctly translates it to the coordinate it should be at in the final map object after cropping and translating
Coordinate MapFoundation::translateCoordMatrixToMap(const Coordinate& currCoord){
    if(DEBUG)cout << "\nMapFoundation::translateCoordMatrixToMap |" << __LINE__ << endl;

    int xNode = currCoord.x - this->coordMostW; // takes the difference of the west most coordinate from the current coordinate
    int yNode = currCoord.y - this->coordMostN; // takes the difference of the north most coordinate from the current coordinate

    Coordinate c(xNode, yNode);
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

// prints the original expandedMatrix map but with all the isolated nodes and edges removed after a MapFoundation instance is made
void MapFoundation::print() const{
    this->expandedMatrix->printExpanded();
}
