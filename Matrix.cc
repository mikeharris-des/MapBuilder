#include "Matrix.h"

/**************************************************************************************************************************************************************************************************************************

     MATRIX

**************************************************************************************************************************************************************************************************************************/

Matrix::Matrix(int d){
    if(d%2==0){d = d-1;} // odd constraint
    if(d<3){d = 3;} // minimum size

    this->dx = d;
    this->dy = d;
    this->matrix = new int[d*d];     // 1d array dynamically allocated
    buildMatrix();
}

Matrix::Matrix(int x, int y){

    if(x<2){x = 2;} // minimum size
    if(y<2){y = 2;} // minimum size

    this->dx = x;
    this->dy = y;
    this->matrix = new int[x*y];     // 1d array dynamically allocated
    buildMatrix();
}

Matrix::Matrix(Matrix* m){ // cp ctor

    this->dx = m->dx; // assigning integers
    this->dy = m->dy;
    this->matrix = new int[m->dx*m->dy]; // 1d array dynamically allocated

    buildMatrix(*m);
}


Matrix::Matrix(const Matrix& m){ // cp ctor

    this->dx = m.dx; // assigning integers
    this->dy = m.dy;
    this->matrix = new int[m.dx*m.dy]; // 1d array dynamically allocated

    buildMatrix(m);
}

Matrix::~Matrix(){

    delete [] this->matrix; // delete dynamically allocated 1d array
}

// function for building a matrix using random 1s or 0s at a cell
void Matrix::buildMatrix(){
    for (int y = 0; y < this->dy; ++y) {     // rows = outer loop increments y
        for (int x = 0; x < this->dx; ++x) {  // cols = inner loop increments x

            if(defaultElement(x, y)){
                this->matrix[ y*this->dx + x ] = NODE; // make default elements 1 for start
            } else{
                this->matrix[ y*this->dx + x ] = rand() % 2; // 0 or 1
            }
            if(DEBUG)cout << this->matrix[ y*this->dx + x ];
        }
        if(DEBUG)cout << endl;
    }
}

// copy function for building an identical matrix to m
void Matrix::buildMatrix(const Matrix& m){
    for (int y = 0; y < this->dy; ++y) {     // rows = outer loop increments y
        for (int x = 0; x < this->dx; ++x) {  // cols = inner loop increments x

            this->matrix[ y*this->dx + x ] = m.matrix[ y*m.dx + x ];

            if(DEBUG)cout << this->matrix[ y*this->dx + x ];
        }
        if(DEBUG)cout << endl;
    }
}

// maintain the center coordinate is always a 1, the 4 adjacent additional cells are defaulted to 1 (makes map bigger more frequently)
bool Matrix::defaultElement(int x, int y) const{
    int c = getCenterCoord();
    if(y == c && x == c) return true;       // yx == centre
    if(y == c || x == c){
        if(y == c && x == (c+1) ) return true;   // yx == east of centre
        if(y == (c-1) && x == c) return true;   // yx == north of centre
        if(y == c && x == (c-1) ) return true;   // yx == west of centre
        if(y == (c+1) && x == c ) return true;   // yx == south of centre
    }
    return false;
}

// y == x at center coord
int Matrix::getCenterCoord() const{
    return this->dx/2;
}

// getter for cell value of matrix at specified coordinates
int Matrix::getCell(int x, int y) const{
    if( offBounds(x,y) )return -1;
    return (this->matrix[ y*this->dx + x ]);
}

// // sets the coordinates of matrix to value if in bounds
void Matrix::setCell(int x, int y, int value){
    if( offBounds(x,y) ){
        cout << "Matrix::setCell off bounds" << endl;
        return;
    }
    this->matrix[ y*this->dx + x ] = value;
}

// return true if c cell value == compare
bool Matrix::checkElement(const Coordinate& c, int compare) const{
    if( offBounds(c) ){return false;}
    else{
        return (this->matrix[ c.y*this->dx + c.x ] == compare);
    }
}

// if the x, y is out of bounds for this matrix returns true -> if( offBounds() )
bool Matrix::offBounds(int x, int y) const{
    return ( x <0 || x >= this->dx || y < 0 || y >= this->dy );
}

// if the coordinate is out of bounds returns true -> if( offBounds() )
bool Matrix::offBounds(const Coordinate& coord) const{
    return ( coord.x <0 || coord.x >= this->dx || coord.y < 0 || coord.y >= this->dy );
}

void Matrix::print() const{
    for (int y = 0; y < this->dx; ++y) {     // rows = outer loop increments y
        for (int x = 0; x < this->dy; ++x) {  // cols = inner loop increments x

            if(this->matrix[ y*this->dx + x ]==0){
                cout << " .";
            } else{
                cout << " " << this->matrix[ y*this->dx + x ];
            }
        }
        cout << endl;
    }
}


/**************************************************************************************************************************************************************************************************************************

    EXPANDED MATRIX

**************************************************************************************************************************************************************************************************************************/

// ctor
ExpandedMatrix::ExpandedMatrix(int d):Matrix(d){
    if(DEBUG)cout << "\nExpandedMatrix::ctor |" << __LINE__ << endl;

    this->exDx = dx + (dx - 1);
    this->exDy = dy + (dy - 1);

    this->matrixExpanded = new int[this->exDx*this->exDy];    // 1d array dynamically allocated

    expandMatrix();         // from inherited matrix instance, expands the matrix filling every cell between the original cells with a zero  eg 1101 -> 1010001
    addEdges();     // from the now expanded matrix of 1s & 0s, will replace a 0 with a 2 if it is seperated by atleast two 1s    eg 1010001 -> 1210001
}

ExpandedMatrix::~ExpandedMatrix(){
    if(DEBUG)cout << "\nExpandedMatrix::dtor |" << __LINE__ << endl;

    delete [] this->matrixExpanded;

}

void ExpandedMatrix::expandMatrix(){
    if(DEBUG)cout << "\nExpandedMatrix::expandMatrix |" << __LINE__ << endl;
    int yC = 0; // row counter reader for reading og matrix
    int xC = 0; // column counter reader for reading og matrix
    for (int y = 0; y < this->exDy; ++y) {
        if (y % 2 == 0) {  // if even row - 0 index
            for (int x = 0; x < this->exDx; ++x) {
                if (x % 2 == 0) {   // if even column -0 index
                    this->matrixExpanded[ y*this->exDx + x ] = matrix[ yC*dx + xC ];
                    ++xC; // increment column index
                } else { // if odd column -0 index
                    this->matrixExpanded[ y*this->exDx + x ] = 0; // default = 0 at a odd x index
                }
            }
            ++yC;   // row counter reader for reading og matrix (reads every other row)
        } else { // if odd row - 0 index
            for (int x = 0; x < this->exDx; ++x) {
                this->matrixExpanded[ y*this->exDx + x ] = 0; // default = 0 at a odd y index
            }
        }
        xC = 0; // reset column counter reader for og matrix (reads every other column only)
    }
    if(EM_DEBUG)cout << "Matrix Expanded" << endl;
}

// builds connections(puts 2s) between all nodes (1s) in matrix to produce a reliable map (after edge is node always in that direction)
void ExpandedMatrix::addEdges(){
    if(DEBUG)cout << "\nExpandedMatrix::addEdges |" << __LINE__ << endl;

    for(int y = 0; y<dy;++y){
        for(int x = 0; x<dx;++x){ //
            if(EM_DEBUG)cout << "    x: " << x << " y: " << y << "       " << " -> ("<< x*2 << "," << y*2 << ") on exp "<<endl;
            if(matrix[ y*dx + x ]==NODE){ // if cell is 1
                for (int i = 0; i < DIRECTION_COUNT; ++i) { // iterate through all directions

                    DirectionType dir = static_cast<DirectionType>(i); // cast i to direction
                    if(EM_DEBUG)cout << "    -> " << direction::toString(dir) << " _ ";

                    Coordinate c(x,y); // make new coordinate at x,y
                    c.set(dir);        // set coord to cell in designated direction
                    if(checkElement(c, NODE)){ // if there is a 1 in that direction make a 2 inbetween

                        Coordinate cEx(2*x,2*y); // make new coord at the translated matrixexpanded coord

                        cEx.set(dir);           // set +1 cell in direction to build connection
                        if(!checkElementExpanded(cEx, EDGE)){                       // make sure it is not a EDGE already
                            if(EM_DEBUG) cout << "  + " << cEx;
                            this->matrixExpanded[ cEx.y*this->exDx + cEx.x ] = EDGE;              // set as a EDGE
                            this->edges+=cEx;                   // add to EDGE array
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
bool ExpandedMatrix::removeEdge(const Coordinate& c){
    if(DEBUG)cout << "\nExpandedMatrix::removeEdge: " << c << endl; // message showing boundry hit

    if( offBoundsExpanded(c) ) return false;

    bool found = this->edges.contains(c); // boolean if the coordinate is found

    clearCell(c); // clear cell
    this->edges-=c;

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
// must be called explicitly after instantiating ExpandedMatrix obj, removes 1 random edge for nodes with 4 edges, makes a realistic looking map
void ExpandedMatrix::removeCommonLoops(){
    if(DEBUG)cout << "\nExpandedMatrix::removeCommonLoops |" << __LINE__ << endl;

    for (int y = 0; y < this->exDy; ++y) { // check loops after built connection & delete loops
        for (int x = 0; x < this->exDx; ++x) { //

            if( this->matrixExpanded[ y*this->exDx + x ]==0 ){ // need to get a coordinate that can chaeck in all directions from that point if there are edges in all connections (2s) == only at 0s or empty elements

                int countLoop = 0; // if countLoop == DIRECTION_COUNT means common loop
                Coordinate cEx(x,y); // current cell

                for (int i = 0; i < DIRECTION_COUNT; ++i) {

                    Coordinate cCopy(cEx); // start at cEx (current cel), make a copy of the coordinate
                    DirectionType dir = static_cast<DirectionType>(i);
                    cCopy.set(dir);
                    if(checkElementExpanded(cCopy, EDGE)){++countLoop;} // if in all directions is a edge increment counter
                }

                if(countLoop == DIRECTION_COUNT){ // if edge in all directions

                    DirectionType dir = static_cast<DirectionType>( rand() % DIRECTION_COUNT ); // get random direction

                    cEx.set(dir); // change coordinate to random edge coordinate
                    if(!removeEdge(cEx)){ // remove edge
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
    if(offBoundsExpanded(c)){
        cout << " markCellVisited OFF BOUNDS " << c << endl;
        return;
    }
    int temp = get(c); // temp is value of cell at c
    this->matrixExpanded[ c.y*this->exDx + c.x ] = temp + VISITED;
}

// changes cell value to 0 at that coordinate regardless of its current value
void ExpandedMatrix::clearCell(const Coordinate& c){
    if(DEBUG)cout << "\nExpandedMatrix::clearCell "  << c << " |" << __LINE__ << endl;
    if( offBoundsExpanded(c) ){
        if(EM_DEBUG)cout << c << " OFF BOUNDS CLEAR " << endl;
        return;
    }

    this->matrixExpanded[ c.y*this->exDx + c.x ] = 0;
}

// return the number of cells in matrix
int ExpandedMatrix::getNumCells() const{
    return this->exDx*this->exDy;
}

// get the absolute centre coordinate of this expanded matrix
int ExpandedMatrix::getCenterCoordExpanded() const{
    if(DEBUG)cout << "\nExpandedMatrix::getCenterCoordEx |" << __LINE__ << endl;

    return this->exDx/2;
}

// getter of cell value for specified coordinate
int ExpandedMatrix::get(const Coordinate& c) const{
    if(DEBUG)cout << "\nExpandedMatrix::get(coord) "  << c << " |" << __LINE__ << endl;
    if( offBoundsExpanded(c) ){return OFF_BOUNDS;}

    return this->matrixExpanded[ c.y*this->exDx + c.x ];
}

// getter of cell value for specified coordinate using its direct x,y values ( x,y is assumed zero index ie x: 0,1,2,3,4  for 5x5 matrix )
int ExpandedMatrix::get(int x, int y) const{
    if(DEBUG)cout << "\nExpandedMatrix::get(x,y) | "  << Coordinate(x,y) << " "<< __LINE__ << endl;

    if( offBoundsExpanded( Coordinate(x,y) ) ){return OFF_BOUNDS;}

    return this->matrixExpanded[ y*this->exDx + x ];
}

// returns true if at that coordinate, the cell value is == compare
bool ExpandedMatrix::checkElementExpanded(const Coordinate& c, int compare) const{

    if( offBoundsExpanded(c) ){
        if(EM_DEBUG)cout << "OFF BOUNDS " << c << endl; // message showing boundry hit
        return false;
    }
    else{
        return ( this->matrixExpanded[ c.y*this->exDx + c.x ] == compare );
    }
}


// if the coordinate is out of bounds returns true -> if( offBoundsExpanded() )
bool ExpandedMatrix::offBoundsExpanded(const Coordinate& c) const{
    return ( c.x < 0 || c.x >= this->exDx || c.y < 0 || c.y >= this->exDy );
}

void ExpandedMatrix::printExpanded() const{
    for (int y = 0; y < this->exDy; ++y) {     // rows = outer loop increments y
        for (int x = 0; x < this->exDx; ++x) {  // columns = inner loop increments x
            if( this->matrixExpanded[ y*this->exDx + x ]==0 ){
                cout << " .";
            } else{
                cout << " "<< this->matrixExpanded[ y*this->exDx + x ];
            }
        }
        cout << endl;
    }
}

void ExpandedMatrix::printEdges() const{
    cout << "ExpandedMatrix EDGES" << endl;
    for (int i = 0; i < this->edges.getSize(); ++i) {
        cout << (i+1) << " " << this->edges[i] << endl;
    }
}
