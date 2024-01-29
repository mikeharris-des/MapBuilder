#include "ExpandedMatrix.h"

// ctor
ExpandedMatrix::ExpandedMatrix(int d):Matrix(d){
    if(DEBUG)cout << "\ExpandedMatrix::ctor |" << __LINE__ << endl;

    this->dimensionExpanded = dimension + (dimension - 1);      // not using d as d could be < default size, dimension is inherited Matrix instance dimension and may be defaulted to minimum

    this->matrixExpanded = new int[this->dimensionExpanded*this->dimensionExpanded];    // 1d bitstring dynamically allocated

    this->numConnections = 0;   // number of 2s in the array
    this->coordConnections = new Coordinate[2*dimension*dimension]; // 2*dimension*dimension: cant have more connections than 2x original matrix of only 1s, dynamically allocated array of coordinates of 2 cell locations

    expandMatrix();         // from inherited matrix instance, expands the matrix filling every cell between the original cells with a zero  eg 1101 -> 1010001
    buildConnections();     // from the now expanded matrix of 1s & 0s, will replace a 0 with a 2 if it is seperated by atleast two 1s    eg 1010001 -> 1210001
}

ExpandedMatrix::~ExpandedMatrix(){
    if(DEBUG)cout << "\ExpandedMatrix::dtor |" << __LINE__ << endl;

    delete [] this->matrixExpanded;

    delete [] this->coordConnections;
}

void ExpandedMatrix::expandMatrix(){
    if(DEBUG)cout << "\ExpandedMatrix::expandMatrix |" << __LINE__ << endl;
    int yC = 0; // row counter reader for reading og matrix
    int xC = 0; // column counter reader for reading og matrix
    for (int y = 0; y < this->dimensionExpanded; ++y) {
        if (y % 2 == 0) {  // if even row - 0 index
            for (int x = 0; x < this->dimensionExpanded; ++x) {
                if (x % 2 == 0) {   // if even column -0 index
                    this->matrixExpanded[ y*this->dimensionExpanded + x ] = matrix[ yC*dimension + xC ];
                    ++xC; // increment column index
                } else { // if odd column -0 index
                    this->matrixExpanded[ y*this->dimensionExpanded + x ] = 0; // default = 0 at a odd x index
                }
            }
            ++yC;   // row counter reader for reading og matrix (reads every other row)
        } else { // if odd row - 0 index
            for (int x = 0; x < this->dimensionExpanded; ++x) {
                this->matrixExpanded[ y*this->dimensionExpanded + x ] = 0; // default = 0 at a odd y index
            }
        }
        xC = 0; // reset column counter reader for og matrix (reads every other column only)
    }
    if(EM_DEBUG)cout << "Matrix Expanded" << endl;
}

// builds connections(puts 2s) between all rooms(1s) in matrix to produce a reliable map (after door is room always in direction)
void ExpandedMatrix::buildConnections(){
    if(DEBUG)cout << "\nExpandedMatrix::buildConnections |" << __LINE__ << endl;

    for(int y = 0; y<dimension;++y){
        for(int x = 0; x<dimension;++x){ //
            if(EM_DEBUG)cout << "    x: " << x << " y: " << y << "       " << " -> ("<< x*2 << "," << y*2 << ") on exp "<<endl;

            if(matrix[ y*dimension + x ]==ROOM){ // if cell is 1

                for (int i = 0; i < Direction::DIRECTION_COUNT; ++i) { // iterate through all directions

                    Direction::Value dir = static_cast<Direction::Value>(i); // cast i to direction
                    if(EM_DEBUG)cout << "    -> " << Direction::toString(dir) << " _ ";

                    Coordinate c(x,y); // make new coordinate at x,y
                    c.set(dir);        // set coord to cell in designated direction

                    if(checkElement(c, ROOM)){ // if there is a 1 in that direction make a 2 inbetween

                        Coordinate cEx(2*x,2*y); // make new coord at the translated matrixexpanded coord

                        cEx.set(dir);           // set +1 cell in direction to build connection

                        if(!checkElementExpanded(cEx, DOOR)){                       // make sure it is not a door already
                            if(EM_DEBUG) cout << "  + " << cEx;
                            this->matrixExpanded[ cEx.y*this->dimensionExpanded + cEx.x ] = DOOR;              // set as a door
                            this->coordConnections[this->numConnections++] = cEx;   // add to door array at numConnections, then increment numConnections
                        }else{
                            if(EM_DEBUG) cout << "  @ " << cEx << " already mapped";
                        }

                    }else{if(EM_DEBUG) cout << "  - ";}

                    if(EM_DEBUG)cout << endl;
                }
            }
            if(EM_DEBUG)cout << endl<<endl;

        } //
    }
}

// utility function in removeCommonLoops to remove a 2 at a coordinate if specified, removes from member array aswell
bool ExpandedMatrix::removeConnection(const Coordinate& c){
    if(DEBUG)cout << "\nExpandedMatrix::removeConnection: " << c << endl; // message showing boundry hit

    if( offBounds(c) ){
        if(EM_DEBUG)cout << "OFF BOUNDS " << c << endl; // message showing boundry hit
        return false;
    }

    clearCell(c); // clear cell

    // find index if it exists and remove from array
    int i = 0;
    bool found = false; // boolean if the coordinate is found

    for(i = 0; i<this->numConnections; ++i){ // iterate through coordinates of connections and get index of the coordinate
        if(c==this->coordConnections[i]){
            found=true;
            break;
        }
    }

    if(found){
        --this->numConnections;
        int j = i;                      // start shifting of elements back one index
        while(j<this->numConnections){
            this->coordConnections[j] = this->coordConnections[j+1]; // shift elements
            ++j;
        }
    }

    return found;
}


/*
11
11
= COMMON LOOP
1.1
...
1.1
-> becomes (random removal of connection)
121
2..
121
-> instead of
121
2.2
121
*/
// must be called explicitly after instantiating ExpandedMatrix obj, removes 1 random door to rooms with 4 doors, makes a realistic looking map
void ExpandedMatrix::removeCommonLoops(){
    if(DEBUG)cout << "\nExpandedMatrix::removeCommonLoops |" << __LINE__ << endl;

    for (int y = 0; y < this->dimensionExpanded; ++y) { // check loops after built connection & delete loops
        for (int x = 0; x < this->dimensionExpanded; ++x) { //

            if( this->matrixExpanded[ y*this->dimensionExpanded + x ]==0 ){ // need to get a coordinate that can chaeck in all directions from that point if there are doors in all connections (2s) == only at 0s or empty elements

                int countLoop = 0; // if countLoop == DIRECTION_COUNT means common loop
                Coordinate cEx(x,y); // current cell

                for (int i = 0; i < Direction::DIRECTION_COUNT; ++i) {

                    Coordinate cCopy(cEx); // start at cEx (current cel), make a copy of the coordinate
                    Direction::Value dir = static_cast<Direction::Value>(i);
                    cCopy.set(dir);
                    if(checkElementExpanded(cCopy, DOOR)){++countLoop;} // if in all directions is a door increment counter
                }

                if(countLoop == Direction::DIRECTION_COUNT){ // if door in all directions

                    Direction::Value dir = static_cast<Direction::Value>( rand() % Direction::DIRECTION_COUNT ); // get random direction

                    cEx.set(dir); // change coordinate to random door coordinate
                    if(!removeConnection(cEx)){ // remove door
                        cout << " ** no connection at coord: " << cEx << endl;
                    }
                }

            }

        } //
    }
}

// increments matrix cell value with the VISITED constant to indicate it has been visited
void ExpandedMatrix::markCellVisited(const Coordinate& c){
    if(DEBUG)cout << "\nExpandedMatrix::markCellVisited "  << c << " |"<< __LINE__ << endl;
    if(offBounds(c)){
        cout << " markCellVisited OFF BOUNDS " << c << endl;
        return;
    }
    int temp = get(c); // temp is value of cell at c
    this->matrixExpanded[ c.y*this->dimensionExpanded + c.x ] = temp + VISITED;
}

// changes cell value to 0 at that coordinate regardless of its current value
void ExpandedMatrix::clearCell(const Coordinate& c){
    if(DEBUG)cout << "\nExpandedMatrix::clearCell "  << c << " |" << __LINE__ << endl;
    if( offBounds(c) ){
        if(EM_DEBUG)cout << c << " OFF BOUNDS CLEAR " << endl;
        return;
    }

    this->matrixExpanded[ c.y*this->dimensionExpanded + c.x ] = 0;
}

 // getter for dimensionExpanded
int ExpandedMatrix::getDimension() const{
    return this->dimensionExpanded;
}

// return the number of cells in matrix
int ExpandedMatrix::getNumCells() const{
    return this->dimensionExpanded*this->dimensionExpanded;
}

// get the absolute centre coordinate of this expanded matrix
int ExpandedMatrix::getCenterCoordExpanded() const{
    if(DEBUG)cout << "\nExpandedMatrix::getCenterCoordEx |" << __LINE__ << endl;

    return this->dimensionExpanded/2;
}

// getter of cell value for specified coordinate
int ExpandedMatrix::get(const Coordinate& c) const{
    if(DEBUG)cout << "\nExpandedMatrix::get(coord) "  << c << " |" << __LINE__ << endl;
    if( offBounds(c) ){return OFF_BOUNDS;}

    return this->matrixExpanded[ c.y*this->dimensionExpanded + c.x ];
}

// getter of cell value for specified coordinate using its direct x,y values ( x,y is assumed zero index ie x: 0,1,2,3,4  for 5x5 matrix )
int ExpandedMatrix::get(int x, int y) const{
    if(DEBUG)cout << "\nExpandedMatrix::get(x,y) |" << __LINE__ << endl;

    if( offBounds( Coordinate(x,y) ) ){return OFF_BOUNDS;}

    return this->matrixExpanded[ y*this->dimensionExpanded + x ];
}

// returns true if at that coordinate, the cell value is == compare
bool ExpandedMatrix::checkElementExpanded(const Coordinate& c, int compare) const{

    if( offBounds(c) ){
        if(EM_DEBUG)cout << "OFF BOUNDS " << c << endl; // message showing boundry hit
        return false;
    }
    else{
        return ( this->matrixExpanded[ c.y*this->dimensionExpanded + c.x ] == compare );
    }
}


// if the coordinate is out of bounds returns true -> if( offBounds() )
bool ExpandedMatrix::offBounds(const Coordinate& c) const{
    return ( c.x < 0 || c.x >= this->dimensionExpanded || c.y < 0 || c.y >= this->dimensionExpanded );
}

void ExpandedMatrix::printExpanded() const{
    for (int i = 0; i < this->dimensionExpanded; ++i) {     // rows = outer loop increments y
        for (int j = 0; j < this->dimensionExpanded; ++j) {  // columns = inner loop increments x
            if( this->matrixExpanded[ i*this->dimensionExpanded + j ]==0 ){
                cout << " .";
            } else{
                cout << " "<< this->matrixExpanded[ i*this->dimensionExpanded + j ];
            }
        }
        cout << endl;
    }
}

void ExpandedMatrix::printConnections() const{
    for (int i = 0; i < this->numConnections; ++i) {

        if(checkElementExpanded(this->coordConnections[i],DOOR)){
            cout << this->coordConnections[i] << endl;

        } else if(checkElementExpanded(this->coordConnections[i],DOOR+VISITED)){
            cout << this->coordConnections[i] << " * Visited * "<< endl;

        } else{
            cout << this->coordConnections[i] << " * Invalid Cell Value * "<< endl;
        }

    }
}
