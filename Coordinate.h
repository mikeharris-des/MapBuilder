#pragma once
#include "defs.h"

namespace Direction {

    enum Value { EAST, NORTH, WEST, SOUTH, DIRECTION_COUNT };

    string toString(Value dir); // Coordinate.cc for implementation

}

// struct storing two integers corresponding to 2 dimensional coordinates on a map / matrix / grid, PARAMETER ORDERING WILL ALWAYS BE X THEN Y: (x,y)
struct Coordinate{
    int x,y;            //  x,y is assumed zero index ie x: 0,1,2,3,4  for 5x5 matrix

    Coordinate(){
        x=y=0;
    }
    Coordinate(const Coordinate& c){
        this->x = c.x;
        this->y = c.y;
    }
    Coordinate(int sx, int sy){
        this->x = sx;
        this->y = sy;
    }
    void set(int sx, int sy){ // set coordinate to sx,sy cell
        if(sx<0 || sy<0){
            return;
        }
        this->x = sx;
        this->y = sy;
    }

    void set(Direction::Value dir){ // set coordinate to +1 cell in that direction
        switch(dir){
            case Direction::EAST:
                ++this->x;
                break;
            case Direction::NORTH:
                --this->y;
                break;
            case Direction::WEST:
                --this->x;
                break;
            case Direction::SOUTH:
                ++this->y;
                break;
            default:
                cout << " set invalid | LINE " << __LINE__ << endl;
                break;
        }
    }

    friend ostream& operator<<(ostream& os, const Coordinate& coord); // Coordinate.cc for implementation

    bool operator==(const Coordinate& c) const{
        return (c.x == this->x && c.y == this->y);
    }

};

// collection class for coordinates, will dynamicly resize : see Coordinate.cc for implementation
class CoordinateArray {
      public:

          CoordinateArray(int size = 32);               // dynamic allocation of array of coordinates
          CoordinateArray(CoordinateArray* cArr);       // dynamic allocation of array of coordinates, creates new copies of data (static coordinate objects)
          ~CoordinateArray();

          Coordinate& operator[](int i) const;              // overload [] operator for convenient indexing / random acces to collection object, checks array bounds
          CoordinateArray& operator+=(const Coordinate& c); // overload += operator for convenient adding to collection object, handles resizing
          CoordinateArray& operator-=(const Coordinate& c); // overload -= operator for convenient removing from collection object, handles shifting


          int getSize() const;
          bool isFull() const;
          bool contains(const Coordinate& c) const;
          void print() const;

      private:
          int numElements;      // size of elements array
          Coordinate* elements; // dynamic allocated array of T objects (can be pointers)
          int backArrSize;      // number of elements in backing array (will be resized)
};
