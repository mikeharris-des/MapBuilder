#pragma once
#include "defs.h"

namespace Direction {

    enum Value { EAST, NORTH, WEST, SOUTH, DIRECTION_COUNT };

    string toString(Value dir);

}


struct Coordinate{
    int x,y;

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

    friend ostream& operator<<(ostream& os, const Coordinate& coord);

    bool operator==(const Coordinate& c) const{
        return (c.x == this->x && c.y == this->y);
    }

};
