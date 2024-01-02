#include "Coordinate.h"

Coordinate::Coordinate(int x, int y):xCoord(x),yCoord(y){}

void Coordinate::set(int x, int y){
    this->xCoord = x;
    this->yCoord = y;
}

bool Coordinate::setInDirection(int tempX, int tempY, Direction dir, int bounds){
    int inc = 1;
    switch(dir){
        case EAST:
        tempX = tempX + inc;
            break;
        case NORTH:
        tempY = tempY - inc;
            break;
        case WEST:
        tempX = tempX - inc;
            break;
        case SOUTH:
        tempY = tempY + inc;
            break;
        default:
            tempX = -1;
            tempY = -1;
            cout << "error getCoord" << endl;
            break;
    }
    if(tempX<0 || tempX>=bounds || tempY<0 || tempY>=bounds){
        return false;
    } else{
        this->xCoord = tempX;
        this->yCoord = tempY;
        return true;
    }
}

string Coordinate::directionToString(Direction dir) {
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

void Coordinate::print() const{
    cout << "(" << this->xCoord << "," << this->yCoord << ")";
}

bool Coordinate::operator==(const Coordinate& coord) const{
    return coord.x()==this->xCoord && coord.y()==this->yCoord;
}

ostream& operator<<(ostream& os, const Coordinate& coord) {
    os << "(" << coord.x() << "," << coord.y() << ")";
    return os;
}
