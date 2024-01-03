#include "ExpandedMatrix.h"

ExpandedMatrix::ExpandedMatrix(int d):Matrix(d){
    this->dimensionEx = dimension + (dimension - 1); // not using d as d could be <default size

    this->matrixEx = new int*[this->dimensionEx];
    for(int i = 0; i<this->dimensionEx;++i){
        this->matrixEx[i] = new int[this->dimensionEx];
    }
    this->numConnections = 0;
    this->coordConnections = new Coordinate[this->dimensionEx*this->dimensionEx]; // cant have more connections than
    expandMatrix();
    buildConnections();
}

ExpandedMatrix::~ExpandedMatrix(){
    for(int i = 0; i<this->dimensionEx;++i){
        delete [] this->matrixEx[i];
    }
    delete [] this->matrixEx;

    delete [] this->coordConnections;
}

void ExpandedMatrix::expandMatrix(){
    int yC = 0; // row counter reader for reading og matrix
    int xC = 0; // column counter reader for reading og matrix
    for (int y = 0; y < this->dimensionEx; ++y) {
        if (y % 2 == 0) {  // if even row -0 index
            for (int x = 0; x < this->dimensionEx; ++x) {
                if (x % 2 == 0) {   // if even column -0 index
                    this->matrixEx[y][x] = matrix[yC][xC];
                    ++xC; // increment column index
                } else { // if odd column -0 index
                    this->matrixEx[y][x] = 0; // default
                }
            }
            ++yC;   // row counter reader for reading og matrix (reads every other row)
        } else { // if odd row -0 index
            for (int x = 0; x < this->dimensionEx; ++x) {
                this->matrixEx[y][x] = 0; // default
            }
        }
        xC = 0; // reset column counter reader for og matrix (reads every other column only)
    }
    if(DEBUG)cout << "Matrix Expanded" << endl;
}

void ExpandedMatrix::buildConnections(){
    if(DEBUG)cout << "ExpandedMatrix::buildConnections :" << __LINE__ << endl;

    for(int k = 0; k<numElements;++k){
        int x = coordElements[k].x();
        int y = coordElements[k].y();
        if(DEBUG)cout << "*[" << x << ',' << y << "]" << endl;

        for (int j = 0; j < DIRECTION_COUNT; ++j) {
            Coordinate c;
            Coordinate cEx;
            Direction dir = static_cast<Direction>(j);
            if(DEBUG)cout << "      " << dir << " |";
            c.setInDirection(x,y,dir);
            if(DEBUG)cout << c << " | ";
            if(checkElement(c, ROOM)){ // if there is a 1 in that direction make a 2 inbetween
                cEx.setInDirection(2*x,2*y,dir);
                if(DEBUG)cout << cEx;
                if(!checkElementEx(cEx, DOOR)){
                    this->matrixEx[cEx.y()][cEx.x()] = DOOR;
                    this->coordConnections[this->numConnections] = cEx;
                    if(DEBUG)cout << cEx << " ";
                    ++this->numConnections;
                }else{
                    if(DEBUG) cout << " door already mapped";
                }
            }else{
                if(DEBUG2)cout << "_______________BH dir :" << cEx.directionToString(dir) << endl;
            }
            if(DEBUG)cout << endl;
        }
    }
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
// common loop is where the map is generated with paths connecting all 4 elements in within a 2x2 sub matrix and is not ideal for a true unique generation
void ExpandedMatrix::removeCommonLoops(){
    if(DEBUG)cout << "removeCommonLoops"<< endl; // message showing boundry hit
    for (int y = 0; y < this->dimensionEx; ++y) { // check loops after built connection & delete loops
        for (int x = 0; x < this->dimensionEx; ++x) {
            if(this->matrixEx[y][x]==0){ // need to get a coordinate that can chaeck in all directions from that point if there are doors in all connections (2s) == only at 0s or empty elements
                Coordinate cEx;

                //int resultEx[2]; // coordinate pair on getCoord
                int countLoop = 0; // if countLoop == DIRECTION_COUNT means common loop
                for (int j = 0; j < DIRECTION_COUNT; ++j) {
                    Direction dir = static_cast<Direction>(j);
                    //getCoordEx(x, y, dir, resultEx);
                    cEx.setInDirection(x, y, dir);
                    if(checkElementEx(cEx, DOOR)){++countLoop;}
                    else{if(DEBUG)cout << "_______________BH: " << x << "," << y << " dir :" << cEx.directionToString(dir) << endl;} // message showing boundry hit}
                }
                if(countLoop==DIRECTION_COUNT){

                    Direction dir = static_cast<Direction>(rand()%DIRECTION_COUNT); // remove road in random location
                    // cEx.setInDirection(x,y,dir,this->dimensionEx);
                    cEx.setInDirection(x, y, dir);
                    this->matrixEx[cEx.y()][cEx.x()] = 0; // remove road
                    if(!removeConnection(cEx)){
                        if(DEBUG)cout << " ** no connection at coord: " << cEx << endl;
                    }
                }
            }
        }
    }
}

void ExpandedMatrix::modCoordElement(const Coordinate& c, int increment){
    if(c.x()<0 || c.x() >= this->dimensionEx || c.y()<0 || c.y() >= this->dimensionEx ){ // bounds check
        return;
    }
    this->matrixEx[c.y()][c.x()] += increment;
    if(DEBUG)cout << c << " modified to: " << this->matrixEx[c.y()][c.x()] << endl;
}

void ExpandedMatrix::clearCoordinate(const Coordinate& c){
    if(c.x()<0 || c.x() >= this->dimensionEx || c.y()<0 || c.y() >= this->dimensionEx ){ // bounds check
        return;
    }
    this->matrixEx[c.y()][c.x()] = 0;
    if(DEBUG)cout << c << " cleared to: " << this->matrixEx[c.y()][c.x()] << endl;
}

int ExpandedMatrix::getDimension(){
    return this->dimensionEx;
}

int ExpandedMatrix::getMatrixSize(){
    return this->dimensionEx*this->dimensionEx;
}

int ExpandedMatrix::get(const Coordinate& c){
    if(c.x()<0 || c.x() >= this->dimensionEx || c.y()<0 || c.y() >= this->dimensionEx ){ // bounds check
        return -1;
    }
    return this->matrixEx[c.y()][c.x()];
}

int ExpandedMatrix::get(int x, int y){
    if(x<0 || x >= this->dimensionEx || y<0 || y >= this->dimensionEx ){ // bounds check
        return -1;
    }
    return this->matrixEx[y][x];
}

int ExpandedMatrix::getCenterCoordEx(){
    return this->dimensionEx/2;
}

bool ExpandedMatrix::checkElementEx(const Coordinate& c, int compare) const{
    if(c.x()<0 || c.x() >= this->dimensionEx || c.y()<0 || c.y() >= this->dimensionEx){
        return false;
    } else{
        return (this->matrixEx[c.y()][c.x()] == compare);
    }
}

bool ExpandedMatrix::removeConnection(const Coordinate& c){
    if(DEBUG)cout << "  removeConnection: " << c << endl; // message showing boundry hit
    this->matrixEx[c.y()][c.x()] = 0;
    if(DEBUG)cout << this->matrixEx[c.y()][c.x()] << endl; // message showing boundry hit

    // find index if it exists
    int i=0;
    bool found = false;
    for(i = 0; i<this->numConnections; ++i){
        if(DEBUG)cout << this->coordConnections[i] << endl; // message showing boundry hit
        if(c==this->coordConnections[i]){
            found=true;
            if(DEBUG)cout << "equals: "<<found << endl; // message showing boundry hit
            break;
        }
    }
    if(found){
        if(DEBUG)cout << "found: " <<found << " | NUMc" << this->numConnections << endl; // message showing boundry hit
        --this->numConnections;
        if(DEBUG)cout << "found: " <<found << " | NUMc" << this->numConnections << endl; // message showing boundry hit
        int j = i;
        while(j<this->numConnections){
            if(DEBUG)cout << "loop: " << j << endl; // message showing boundry hit
            if(DEBUG)cout << "[j]: " << this->coordConnections[j] << " | [j+1]: " << this->coordConnections[j+1] <<endl; // message showing boundry hit
            this->coordConnections[j] = this->coordConnections[j+1]; // shift
            ++j;
        }

        return true;
    } else{
        if(DEBUG)cout << "not found: "<<found << endl; // message showing boundry hit
        return false;
    }
}

void ExpandedMatrix::printEx(){
    for (int i = 0; i < this->dimensionEx; ++i) {     // rows = outer loop increments y
        for (int j = 0; j < this->dimensionEx; ++j) {  // columns = inner loop increments x
            if(this->matrixEx[i][j]==0){
                cout << ".";
            } else{
                cout << this->matrixEx[i][j];
            }
        }
        cout << endl;
    }
}

void ExpandedMatrix::printConnections(){
    for (int i = 0; i < this->numConnections; ++i) {     // rows = outer loop increments y
        if(checkElementEx(this->coordConnections[i],DOOR)){
            cout << this->coordConnections[i] << endl;
        }
    }
}
