#include "Matrix.h"

Matrix::Matrix(int d){
    if(d<3){d = 3;} // minimum size
    this->dimension = d;
    this->matrix = new int*[d];
    for (int i = 0; i < d; ++i) {
        this->matrix[i] = new int[d]; // needs to be pointer array for memory issues of statically allocating fixed size
    }
    this->numElements = 0;
    this->coordElements = new Coordinate[d*d]; // dyn allocd coordinate arr

    buildMatrix();
}

Matrix::~Matrix(){
    for(int i = 0; i< this->dimension; ++i){
        delete [] this->matrix[i]; // delete nested arrays dynamically allocated
    }
    delete [] this->matrix; // delete dynamically allocated 2d array

    delete [] this->coordElements; // delete dynamically allocated 2d array
}

void Matrix::buildMatrix(){
    for (int i = 0; i < this->dimension; ++i) {     // rows = outer loop increments y
        for (int j = 0; j < this->dimension; ++j) {  // cols = inner loop increments x
            if(defaultElement(j, i)){
                this->matrix[i][j] = ROOM; // make default elements 1 for start
            } else{
                this->matrix[i][j] = rand() % 2; // 0 or 1
            }
            if(DEBUG)cout << this->matrix[i][j];
            if(this->matrix[i][j]==ROOM){
                this->coordElements[this->numElements].set(j,i);
                ++this->numElements;
            }
        }
        if(DEBUG)cout << endl;
    }
}

bool Matrix::defaultElement(int x, int y) const{
    int c = getCenterCoord();
    if(y == c && x == c) return true;       // yx == centre
    if(ALLOW_DEFAULTS){
        if(y == c || x == c){
            if(y == c && x == (c+1) ) return true;   // yx == east of centre
            if(y == (c-1) && x == c) return true;   // yx == north of centre
            if(y == c && x == (c-1) ) return true;   // yx == west of centre
            if(y == (c+1) && x == c ) return true;   // yx == south of centre
        }
    }
    return false;
}

// y == x at center coord
int Matrix::getCenterCoord() const{
    return this->dimension/2;
}


bool Matrix::checkElement(const Coordinate& coord, int compare) const{
    return (this->matrix[coord.y()][coord.x()] == compare);
}

void Matrix::print() const{
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
