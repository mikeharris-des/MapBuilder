#include "Matrix.h"

Matrix::Matrix(int d){
    if(d%2==0){d = d-1;} // odd constraint
    if(d<3){d = 3;} // minimum size

    this->dimension = d;
    this->matrix = new int[d*d];     // 1d bitstring dynamically allocated
    buildMatrix();
}

Matrix::Matrix(const Matrix& m){ // cp ctor

    this->dimension = m.dimension;
    this->matrix = new int[m.dimension*m.dimension]; // 1d bitstring dynamically allocated

    buildMatrix(m);
}

Matrix::~Matrix(){

    delete [] this->matrix; // delete dynamically allocated 1d bitstring
}

// function for building a matrix using random 1s or 0s at a cell
void Matrix::buildMatrix(){
    for (int y = 0; y < this->dimension; ++y) {     // rows = outer loop increments y
        for (int x = 0; x < this->dimension; ++x) {  // cols = inner loop increments x

            if(defaultElement(x, y)){
                this->matrix[ y*this->dimension + x ] = ROOM; // make default elements 1 for start
            } else{
                this->matrix[ y*this->dimension + x ] = rand() % 2; // 0 or 1
            }
            if(DEBUG)cout << this->matrix[ y*this->dimension + x ];
        }
        if(DEBUG)cout << endl;
    }
}

// copy function for building an identical matrix to m
void Matrix::buildMatrix(const Matrix& m){
    for (int y = 0; y < this->dimension; ++y) {     // rows = outer loop increments y
        for (int x = 0; x < this->dimension; ++x) {  // cols = inner loop increments x

            this->matrix[ y*this->dimension + x ] = m.matrix[ y*this->dimension + x ];

            if(DEBUG)cout << this->matrix[ y*this->dimension + x ];
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
    return this->dimension/2;
}

// return true if c cell value == compare
bool Matrix::checkElement(const Coordinate& c, int compare) const{
    if( offBounds(c) ){return false;}
    else{
        return (this->matrix[ c.y*this->dimension + c.x ] == compare);
    }
}

// if the coordinate is out of bounds returns true -> if( offBounds() )
bool Matrix::offBounds(const Coordinate& coord) const{
    return ( coord.x <0 || coord.x >= this->dimension || coord.y < 0 || coord.y >= this->dimension );
}

void Matrix::print() const{
    for (int y = 0; y < this->dimension; ++y) {     // rows = outer loop increments y
        for (int x = 0; x < this->dimension; ++x) {  // cols = inner loop increments x

            if(this->matrix[ y*this->dimension + x ]==0){
                cout << " .";
            } else{
                cout << " " << this->matrix[ y*this->dimension + x ];
            }
        }
        cout << endl;
    }
}
