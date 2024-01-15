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
