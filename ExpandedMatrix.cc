#include "ExpandedMatrix.h"

ExpandedMatrix::ExpandedMatrix(int d):Matrix(d){
    this->dimensionEx = dimension + (dimension - 1);

    this->matrixEx = new int*[this->dimensionEx];
    for(int i = 0; i<this->dimensionEx;++i){
        this->matrixEx[i] = new int[this->dimensionEx];
    }
    expandMatrix();
    buildConnections();
}

ExpandedMatrix::~ExpandedMatrix(){
    for(int i = 0; i<this->dimensionEx;++i){
        delete [] this->matrixEx[i];
    }
    delete [] this->matrixEx;
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
    int result[2];
    int resultEx[2];
    for(int k = 0; k<numElements;++k){
        int x = coordElements[k][0];
        int y = coordElements[k][1];
        if(DEBUG)cout << "*[" << x << ',' << y << "]" << endl;
        for (int j = 0; j < DIRECTION_COUNT; ++j) {
            Direction dir = static_cast<Direction>(j);
            if(getCoord(x, y, dir, result)){
                //if(DEBUG2)cout << "_______________   dir :" << directionToString(dir) << ": " << result[0] << ", " << result[1] << endl;
                //if(DEBUG2)cout << "Value is: " << matrix[result[1]][result[0]] << " : " << checkElement(matrix, result[0], result[1], ROOM) << endl;
                if(checkElement(result[0], result[1], ROOM)){ // if there is a 1 in that direction make a 2 inbetween

                    if(getCoordEx(2*x, 2*y, dir, resultEx)){ // covert coordinate to expanded coord = *2 as need to get 1 in direction if not out of bounds (double check)
                        //if(DEBUG2)cout << "                   BH dir :" << directionToString(dir) << ": " << result2[0] << ", " << result2[1] << endl;
                        //if(DEBUG2)cout << "       Value is: " << this->matrixEx[result2[1]][result2[0]] << " : " << checkElement2(this->matrixEx, result2[0], result2[1], ROOM) << endl;
                        this->matrixEx[resultEx[1]][resultEx[0]] = DOOR;
                        //if(DEBUG2)cout << "       Value is: " << b[result2[1]][result2[0]] << " : " << checkElement2(b, result2[0], result2[1], ROOM) << endl;
                    } else{
                        cout << endl << "BIG ERRR bC " << __LINE__ << endl << endl;
                    }
                }
            }
            else{
                if(DEBUG2)cout << "_______________BH dir :" << directionToString(dir) << endl;
            }
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
    for (int y = 0; y < this->dimensionEx; ++y) { // check loops after built connection & delete loops
        for (int x = 0; x < this->dimensionEx; ++x) {
            if(this->matrixEx[y][x]==0){ // need to get a coordinate that can chaeck in all directions from that point if there are doors in all connections (2s) == only at 0s or empty elements
                int resultEx[2]; // coordinate pair on getCoord
                int countLoop = 0; // if countLoop == DIRECTION_COUNT means common loop
                for (int j = 0; j < DIRECTION_COUNT; ++j) {
                    Direction dir = static_cast<Direction>(j);
                    //getCoordEx(x, y, dir, resultEx);
                    if(getCoordEx(x, y, dir, resultEx)){
                        if(DEBUG2)cout << "_______________   dir :" << directionToString(dir) << ": " << resultEx[0] << ", " << resultEx[1] << endl;
                        if(DEBUG2)cout << "Value is: " << this->matrixEx[resultEx[1]][resultEx[0]] << " : " << checkElementEx(resultEx[0], resultEx[1], DOOR) << endl;
                        if(checkElementEx(resultEx[0], resultEx[1], DOOR)){++countLoop;}
                    } else{
                        if(DEBUG)cout << "_______________BH dir :" << directionToString(dir) << endl; // message showing boundry hit
                    }
                }
                if(countLoop==DIRECTION_COUNT){
                    int coordLoop[2];
                    Direction dir = static_cast<Direction>(rand()%DIRECTION_COUNT); // remove road in random location
                    getCoordEx(x, y, dir, coordLoop);
                    this->matrixEx[coordLoop[1]][coordLoop[0]] = 0; // remove road
                }
            }
        }
    }
}


int ExpandedMatrix::getCenterCoordEx(){
    return this->dimensionEx/2;
}


// gets coordinates in the direction given returns false if out of bounds of expanded matrix dimensions
bool ExpandedMatrix::getCoordEx(int currX, int currY, Direction dir, int* coord){
    int tempX = currX;
    int tempY = currY;
    int jump = 1;
    switch(dir){
        case EAST:
        tempX = tempX + jump;
            break;
        case NORTH:
        tempY = tempY - jump;
            break;
        case WEST:
        tempX = tempX - jump;
            break;
        case SOUTH:
        tempY = tempY + jump;
            break;
        default:
            tempX = -1;
            tempY = -1;
            cout << "error getCoord" << endl;
            break;
    }
    if(tempX<0 || tempX>=this->dimensionEx || tempY<0 || tempY>=this->dimensionEx){
        coord[0] = currX;
        coord[1] = currY;
        return false;
    } else{
        coord[0] = tempX;
        coord[1] = tempY;
        return true;
    }
}

bool ExpandedMatrix::checkElementEx(int xCheck, int yCheck, int compare){
    return (this->matrixEx[yCheck][xCheck] == compare);
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
