#include "Coordinate.h"

ostream& operator<<(ostream& os, const Coordinate& coord){
    os << "(" << coord.x << "," << coord.y << ")";
    return os;
}

string Direction::toString(Value dir){
    switch(dir){
        case EAST:
            return "EAST";
        case NORTH:
            return "NORTH";
        case WEST:
            return "WEST";
        case SOUTH:
            return "SOUTH";
        default:
            return "UNKNOWN DIRECTION";
    }
}

// dynamic allocation of coordinate array
CoordinateArray::CoordinateArray(int size){
    this->backArrSize = size;
	this->elements = new Coordinate[size];
	this->numElements = 0;
}

// dynamic allocation of coordinate array, creates new copies of data (static coordinate objects)
CoordinateArray::CoordinateArray(CoordinateArray* cArr){
    this->backArrSize = cArr->backArrSize;
    this->elements = new Coordinate[cArr->backArrSize];
    this->numElements = cArr->numElements;

    for(int i = 0; i<cArr->numElements; ++i){
        this->elements[i] = Coordinate(cArr->elements[i]);
    }
}


CoordinateArray::~CoordinateArray(){
	delete [] this->elements;
}

// overload [] operator for convenient indexing / random acces to collection object, checks array bounds
Coordinate& CoordinateArray::operator[](int index) const{
    if (index < 0 || index >= this->numElements) {
		cerr<<"\nCoordinate Array indexing out of bounds"<<endl;
		exit(1);
	}
	return this->elements[index];
}

// overload += operator for convenient adding to collection object, handles resizing
CoordinateArray& CoordinateArray::operator+=(const Coordinate& c){
	if ( isFull() ){
        this->backArrSize = 2 * this->backArrSize;
        Coordinate* resized = new Coordinate[ this->backArrSize ];

        for(int i = 0; i<this->numElements; ++i){
            resized[i] = this->elements[i];
        }

        delete [] this->elements; // Deallocate the old array of pointers

        this->elements = resized; // assign new
    }
  	this->elements[this->numElements++] = c;
	return *this;
}

// overload -= operator for convenient removing from collection object, handles shifting
CoordinateArray& CoordinateArray::operator-=(const Coordinate& c){
	int i = 0;
	while (i < this->numElements){     // if remove is in middle of array requires locating and shifting
		if (c == this->elements[i]){
			--this->numElements;
			break;
		}
		++i;
	}
	while (i < this->numElements){
		this->elements[i] = this->elements[i + 1];        // shift elements
		++i;
	}
	return *this;
}

int CoordinateArray::getSize() const{
	return this->numElements;
}

bool CoordinateArray::isFull() const {
	return this->numElements >= this->backArrSize;
}


bool CoordinateArray::contains(const Coordinate& c) const{
    for(int i = 0; i < this->numElements; ++i){
        if(c==this->elements[i]){
            return true;
        }
    }
    return false;
}

void CoordinateArray::print() const{
    cout << endl;
    for(int i = 0; i< this->numElements; ++i){
        cout << this->elements[i] << endl;
    }
}

// a custom hash function for the CoordinateAdjacencyList hash table
// https://www.geeksforgeeks.org/implement-custom-hash-functions-for-user-defined-types-in-std-unordered_map/
size_t CoordinateHashFunction::operator()(const Coordinate& c) const {
    // combine hashes of x and y integer values using the bitwise XOR
    return hash<int>()(c.x) ^ (hash<int>()(c.y) << 1);
}
