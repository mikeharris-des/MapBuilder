#include "Coordinate.h"

Coordinate::Coordinate(int x, int y):xCoord(x),yCoord(y){}

void Coordinate::set(int x, int y){
    this->xCoord = x;
    this->yCoord = y;
}

void Coordinate::setInDirection(int tempX, int tempY, Direction dir){
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
            cout << "error setInDirection" << endl;
            break;
    }
    this->xCoord = tempX;
    this->yCoord = tempY;
}

void Coordinate::setInDirection(const Coordinate& coord, Direction dir){
    int inc = 1;
    int tempX = coord.x();
    int tempY = coord.y();
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
            cout << "error setInDirection" << endl;
            break;
    }
    this->xCoord = tempX;
    this->yCoord = tempY;
}

void Coordinate::setInDirection(const Coordinate& coord, Direction dir, int jump){
    int inc = jump;
    int tempX = coord.x();
    int tempY = coord.y();
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
            cout << "error setInDirection" << endl;
            break;
    }
    this->xCoord = tempX;
    this->yCoord = tempY;
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

bool Coordinate::compareY(const Coordinate& coord) const{
    return this->yCoord<coord.y();
}

bool Coordinate::compareX(const Coordinate& coord) const{
    return this->xCoord<coord.x();
}

ostream& operator<<(ostream& os, const Coordinate& coord) {
    os << "(" << coord.x() << "," << coord.y() << ")";
    return os;
}
