#include "Coordinate.h"

ostream& operator<<(ostream& os, const Coordinate& coord){
    os << "(" << setw(2) << coord.x << "," << setw(2) << coord.y << ")";
    return os;
}

string direction::toString( DirectionType dir ){
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

// get the direction from the point of reference of coordinate a, the direction that coordinate b is in
DirectionType direction::getDirection( const Coordinate& a, const Coordinate& b )
{
    // 2 is the cell coordinate difference of 2 nodes in the map (seperated by 1 edge)
    int diffX = (b.x - a.x);
    int diffY = (b.y - a.y);

    DirectionType directionOfB = DIRECTION_COUNT;

   if (diffX > 0 && diffY == 0)
   {
        directionOfB = EAST;
    }
    else if (diffX == 0 && diffY < 0)
    {
        directionOfB = NORTH;
    }
    else if (diffX < 0 && diffY == 0)
    {
        directionOfB = WEST;
    }
    else if (diffX == 0 && diffY > 0)
    {
        directionOfB = SOUTH;
    }

    return directionOfB;
}


// dynamic allocation of coordinate array
CoordinateArray::CoordinateArray(int size){
    this->backArrSize = size;
	this->elements = new Coordinate[size];
	this->numElements = 0;
}

// dynamic allocation of coordinate array, creates new copies of data (static coordinate objects)
CoordinateArray::CoordinateArray(const CoordinateArray& cArr){
    this->backArrSize = cArr.backArrSize;
    this->elements = new Coordinate[cArr.backArrSize];
    this->numElements = cArr.numElements;

    for(int i = 0; i<cArr.numElements; ++i){
        this->elements[i] = Coordinate(cArr.elements[i]);
    }
}


CoordinateArray::~CoordinateArray(){
	delete [] this->elements;
}

// overload [] operator for convenient indexing / random acces to collection object, checks array bounds
Coordinate& CoordinateArray::operator[](int index) const{
    if (index < 0 || index >= this->numElements) {
		cout<<"\n CoordinateArray ERROR [ "<< index << " ] ***** Coordinate Array indexing out of bounds *****"<<endl;
        static Coordinate badCoord(-1,-1);  // static implemented to reference the coordinate globally for the reference return
        return badCoord;
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

// overload -- operator for convenient removing from collection object from the back, handles empty
CoordinateArray& CoordinateArray::operator--()
{
    if (this->numElements > 0)
    {
        --this->numElements;
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
        cout << setw(5) << (i+1) << " : "<< this->elements[i] << endl;
    }
}

// a custom hash function for the CoordinateAdjacencyList hash table
// https://www.geeksforgeeks.org/implement-custom-hash-functions-for-user-defined-types-in-std-unordered_map/
size_t CoordinateHashFunction::operator()(const Coordinate& c) const {
    // combine hashes of x and y integer values using the bitwise XOR
    return hash<int>()(c.x) ^ (hash<int>()(c.y) << 1);
}
