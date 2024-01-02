#include "Matrix.h"

Matrix::Matrix(int d){
    if(d<3){d = 3;} // minimum size
    this->dimension = d;
    this->matrix = new int*[d];
    for (int i = 0; i < d; ++i) {
        this->matrix[i] = new int[d]; // needs to be pointer array for memory issues of statically allocating fixed size
    }
    this->numElements = 0;

    this->coordElements = new int*[d*d];
    for (int j = 0; j < d*d; ++j) {
        this->coordElements[j] = new int[2]; // needs to be pointer array for memory issues of statically allocating fixed size
    }
    buildMatrix();
}

Matrix::~Matrix(){
    for(int i = 0; i< this->dimension; ++i){
        delete [] this->matrix[i]; // delete nested arrays dynamically allocated
    }
    delete [] this->matrix; // delete dynamically allocated 2d array

    for(int j = 0; j<this->dimension*this->dimension; ++j){
        delete [] this->coordElements[j]; // delete nested arrays dynamically allocated
    }
    delete [] this->coordElements; // delete dynamically allocated 2d array
}

void Matrix::buildMatrix(){
    for (int i = 0; i < this->dimension; ++i) {     // rows = outer loop increments y
        for (int j = 0; j < this->dimension; ++j) {  // cols = inner loop increments x
            if(defaultElement(i, j)){
                this->matrix[i][j] = 1; // make default elements 1 for start
            } else{
                this->matrix[i][j] = rand() % 2;
            }
            if(DEBUG)cout << this->matrix[i][j];
            if(this->matrix[i][j]==1){
                this->coordElements[this->numElements][1] = i; // y
                this->coordElements[this->numElements][0] = j; // x
                ++this->numElements;
            }
        }
        if(DEBUG)cout << endl;
    }
}

bool Matrix::defaultElement(int y, int x){
    int c = getCenterCoord();
    if(y == c && x == c) return true;       // yx == centre
    if(ALLOW_DEFAULTS){
        if(y == c || x == c){
            // if(y == DIM/2 && x == DIM/2) return true;       // yx == centre
            if(y == c && x == (c+1) ) return true;   // yx == east of centre
            if(y == (c-1) && x == c) return true;   // yx == north of centre
            if(y == c && x == (c-1) ) return true;   // yx == west of centre
            if(y == (c+1) && x == c ) return true;   // yx == south of centre
        }
    }
    return false;
}

int Matrix::getCenterCoord(){
    return this->dimension/2;
}


bool Matrix::getCoord(int currX, int currY, Direction dir, int* coord){
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
    if(tempX<0 || tempX>=this->dimension || tempY<0 || tempY>=this->dimension){
        coord[0] = currX;
        coord[1] = currY;
        return false;
    } else{
        coord[0] = tempX;
        coord[1] = tempY;
        return true;
    }
}

bool Matrix::checkElement(int xCheck, int yCheck, int compare){
    return (this->matrix[yCheck][xCheck] == compare);
}

string Matrix::directionToString(Direction dir) {
    switch (dir) {
        case EAST:
            return "EAST";
        case NORTH:
            return "NORTH";
        case WEST:
            return "WEST";
        case SOUTH:
            return "SOUTH";
        default:
            cout << " *" << to_string(dir) << "* ";
            return "DIRECTION";
        }
}

void Matrix::print(){
    for (int i = 0; i < this->dimension; ++i) {     // rows = outer loop increments y
        for (int j = 0; j < this->dimension; ++j) {  // columns = inner loop increments x
            if(this->matrix[i][j]==0){
                cout << ".";
            } else{
                cout << this->matrix[i][j];
            }
        }
        cout << endl;
    }
}
