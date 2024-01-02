#include <iostream>
#include <cstdlib>  // for srand
#include <ctime>    // for time
#include <algorithm>
#include <vector>

#define DIM 7 // must be odd to work correctly ->try 3,5,7
#define MAX_ARR (DIM * DIM)
#define DIM2 (DIM + DIM-1)
#define MAX_ARR2 (DIM2 * DIM2)
#define ALLOW_DEFAULTS 1 // 1 to allow default elements at
#define DEBUG1 1
#define DEBUG2 0
#define ROOM 1
#define DOOR 2

using namespace std;
string map[MAX_ARR2];

enum Direction { EAST, NORTH, WEST, SOUTH, DIRECTION_COUNT };

void buildMatrix(int a[DIM][DIM], int coordElements[MAX_ARR][2], int& numElements); // makes initial matrix of 1s & 0s randomly
bool defaultElement(int y, int x); // inserts valid elements (1s) at default coordinates

void expandMatrix(int a[DIM][DIM], int b[DIM2][DIM2]);
void buildConnections(int a[DIM][DIM], int b[DIM2][DIM2], int coordElements[MAX_ARR][2], int numElements);

void getCoord(int currX, int currY, Direction dir, int dim, int* coord);
string directionToString(Direction dir);
bool checkElement(int a[DIM][DIM], int xCheck, int yCheck, int compare);
bool checkElement2(int a[DIM2][DIM2], int xCheck, int yCheck, int compare);
void pBreak();

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    int a[DIM][DIM];
    int numElements = 0;
    int coordElements[MAX_ARR][2];
    pBreak();

    buildMatrix(a,coordElements,numElements);

    pBreak();
    for(int k = 0; k<numElements;++k){
        cout << coordElements[k][0] << ',' << coordElements[k][1] <<endl;
    }
    if(DEBUG1){
        pBreak();
        cout << endl << " half: " << DIM/2 << endl;
        pBreak();
    }
    //a[DIM/2][DIM/2] = 1; // make center 1 always for start
    if(DEBUG2){
        pBreak();
        for (int i = 0; i < DIM; ++i) {     // rows = outer loop increments y
            for (int j = 0; j < DIM; ++j) {  // columns = inner loop increments x
                // a[i][j] = rand() % 2;
                cout << a[i][j];
            }
            cout << endl;
        }
        pBreak();
    }

    int b[DIM2][DIM2];
    expandMatrix(a,b);
    pBreak();
    for (int y = 0; y < DIM2; ++y) {
        for (int x = 0; x < DIM2; ++x) {
            if(b[y][x]==0){
                cout << ".";
            } else{
                cout << b[y][x];
            }

        }
        cout << endl;
    }
    pBreak();

    buildConnections(a,b,coordElements,numElements);

    pBreak();
    for (int y = 0; y < DIM; ++y) {
        for (int x = 0; x < DIM; ++x) {
            if(a[y][x]==0){
                cout << ".";
            } else{
                cout << a[y][x];
            }
        }
        cout << endl;
    }
    pBreak();
    for (int y = 0; y < DIM2; ++y) { // check loops after built connection & delete loops
        for (int x = 0; x < DIM2; ++x) {
            if(b[y][x]==0){

                int result[2];
                int countLoop = 0;
                for (int j = 0; j < DIRECTION_COUNT; ++j) {
                    Direction dir = static_cast<Direction>(j);
                    getCoord(x, y, dir, DIM2, result);
                    if(result[0]==x && result[1]==y){
                        if(DEBUG2)cout << "_______________BH dir :" << directionToString(dir) << endl;
                    } else{
                        if(DEBUG2)cout << "_______________   dir :" << directionToString(dir) << ": " << result[0] << ", " << result[1] << endl;
                        if(DEBUG2)cout << "Value is: " << b[result[1]][result[0]] << " : " << checkElement2(b, result[0], result[1], DOOR) << endl;
                        if(checkElement2(b, result[0], result[1], DOOR)){++countLoop;} // if there is a connection in that direction increment counter
                    }
                }
                if(countLoop==DIRECTION_COUNT){
                    //cout << "L";
                    int coordL[2];
                    Direction dir = static_cast<Direction>(rand()%DIRECTION_COUNT);
                    getCoord(x, y, dir, DIM2, coordL);
                    b[coordL[1]][coordL[0]] = 0;
                } else{
                    //cout << ".";
                }
            } else{
                //cout << b[y][x];
            }
        }
        //cout << endl;
    }
    pBreak();
    for (int y = 0; y < DIM2; ++y) {
        for (int x = 0; x < DIM2; ++x) {
            if(b[y][x]==0){
                cout << ".";
            }else{
                cout << b[y][x];
            }
        }
        cout << endl;
    }
    pBreak();

    /*
    arr to store all rooms, array to store all doors


    start
    dim/2
    check dir order ->
        get coord at dir if coord is not visited
        store

    */

    // start dim/2
    // check dir order ->
        get coord at dir
    // store

    return 0;
}

void buildMatrix(int a[DIM][DIM], int coordElements[MAX_ARR][2], int& numElements){
    for (int i = 0; i < DIM; ++i) {     // rows = outer loop increments y
        for (int j = 0; j < DIM; ++j) {  // cols = inner loop increments x
            if(defaultElement(i, j)){
                a[i][j] = 1; // make center 1 always for start
            } else{
                a[i][j] = rand() % 2;
            }
            cout << a[i][j];
            if(a[i][j]==1){
                coordElements[numElements][1] = i; // y
                coordElements[numElements][0] = j; // x
                ++numElements;
            }
        }
        cout << endl;
    }
}

bool defaultElement(int y, int x){
    if(y == DIM/2 && x == DIM/2) return true;       // yx == centre
    if(ALLOW_DEFAULTS){
        if(y == DIM/2 || x == DIM/2){
            // if(y == DIM/2 && x == DIM/2) return true;       // yx == centre
            if(y == DIM/2 && x == (DIM/2+1) ) return true;   // yx == east of centre
            if(y == (DIM/2-1) && x == DIM/2) return true;   // yx == north of centre
            if(y == DIM/2 && x == (DIM/2-1) ) return true;   // yx == west of centre
            if(y == (DIM/2+1) && x == DIM/2 ) return true;   // yx == south of centre
        }
    }
    return false;
}

void expandMatrix(int a[DIM][DIM], int b[DIM2][DIM2]){
    int yC = 0; // row counter reader for reading og matrix
    int xC = 0; // column counter reader for reading og matrix
    for (int y = 0; y < DIM2; ++y) {
        if (y % 2 == 0) {  // if even row -0 index
            for (int x = 0; x < DIM2; ++x) {
                if (x % 2 == 0) {   // if even column -0 index
                    b[y][x] = a[yC][xC];
                    ++xC; // increment column index
                } else { // if odd column -0 index
                    b[y][x] = 0; // default
                }
            }
            ++yC;   // row counter reader for reading og matrix (reads every other row)
        } else { // if odd row -0 index
            for (int x = 0; x < DIM2; ++x) {
                b[y][x] = 0; // default
            }
        }
        xC = 0; // reset column counter reader for og matrix (reads every other column only)
    }
    cout << "Matrix Expanded" << endl;
}

void buildConnections(int a[DIM][DIM], int b[DIM2][DIM2], int coordElements[MAX_ARR][2], int numElements){
    int result[2];
    int result2[2];
    for(int k = 0; k<numElements;++k){
        if(DEBUG2)pBreak();
        if(DEBUG2)cout << coordElements[k][0] << ',' << coordElements[k][1] <<endl;
        int x = coordElements[k][0];
        int y = coordElements[k][1];
        for (int j = 0; j < DIRECTION_COUNT; ++j) {
            Direction dir = static_cast<Direction>(j);
            getCoord(x, y, dir, DIM, result);
            if(result[0]==x && result[1]==y){
                if(DEBUG2)cout << "_______________BH dir :" << directionToString(dir) << endl;
            } else{
                if(DEBUG2)cout << "_______________   dir :" << directionToString(dir) << ": " << result[0] << ", " << result[1] << endl;
                if(DEBUG2)cout << "Value is: " << a[result[1]][result[0]] << " : " << checkElement(a, result[0], result[1], ROOM) << endl;
                if(checkElement(a, result[0], result[1], ROOM)){ // if there is a 1 in that direction make a 2 inbetween

                    getCoord(x*2, y*2, dir, DIM2, result2);
                    if(result2[0]==(x*2) && result2[1]==(y*2)){
                        cout << endl << "BIG ERRR bC " << __LINE__ << endl << endl;
                    }
                    if(DEBUG2)cout << "                   BH dir :" << directionToString(dir) << ": " << result2[0] << ", " << result2[1] << endl;
                    if(DEBUG2)cout << "       Value is: " << b[result2[1]][result2[0]] << " : " << checkElement2(b, result2[0], result2[1], ROOM) << endl;
                    b[result2[1]][result2[0]] = 2;
                    if(DEBUG2)cout << "       Value is: " << b[result2[1]][result2[0]] << " : " << checkElement2(b, result2[0], result2[1], ROOM) << endl;
                }
            }
        }
    }
}


void getCoord(int currX, int currY, Direction dir, int dim, int* coord){
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
    if(tempX<0 || tempX>=dim || tempY<0 || tempY>=dim){
        coord[0] = currX;
        coord[1] = currY;
    } else{
        coord[0] = tempX;
        coord[1] = tempY;
    }
}

string directionToString(Direction dir) {
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

bool checkElement(int a[DIM][DIM], int xCheck, int yCheck, int compare){
    return (a[yCheck][xCheck] == compare);
}

bool checkElement2(int b[DIM2][DIM2], int xCheck, int yCheck, int compare){
    return (b[yCheck][xCheck] == compare);
}

void pBreak(){
    cout << endl << "-------------------------------------------------------" << endl;
}

/*

ARRAY COORDINATEs
pBreak();
cout << a[0][0] << a[0][1] << a[0][2] << endl;
cout << a[1][0] << a[1][1] << a[1][2] << endl;
cout << a[2][0] << a[2][1] << a[2][2] << endl;
pBreak();
// FROM 1,1 = centre
cout << a[1][2] << endl; // east
cout << a[0][1] << endl; // north
cout << a[1][0] << endl; // west
cout << a[2][1] << endl; // south
pBreak();


WORKING EXPANDOR
int b[DIM2][DIM2];  // expand the matrix by DIM - 1 in each axis
int xC = 0; // row counter reader for reading og matrix
int yC = 0; // column counter reader for reading og matrix
for (int x = 0; x < DIM2; ++x) {
    if (x % 2 == 0) {  // if even row -0 index
        for (int y = 0; y < DIM2; ++y) {
            if (y % 2 == 0) {   // if even column -0 index
                b[x][y] = a[xC][yC];
                ++yC;
            } else { // if odd column -0 index
                b[x][y] = 0; // default
            }
            cout << b[x][y];
        }
        ++xC;   // row counter reader for reading og matrix (reads every other row)
    } else { // if odd row -0 index
        for (int y = 0; y < DIM2; ++y) {
            b[x][y] = 0;
            cout << b[x][y];
        }
    }
    cout << endl;
    yC = 0; // reset column counter reader for og matrix (reads every other column only)
}

need to know all rooms and path before generation -> last event

path vertical
|
|
|
|
=
...|...
...|...
...|...
...|...

path horizontal
-------
=
.......
.......
-------
.......

map *7 for top left coordinate
..o..
..|..
o-o-o
..|..
..o..

010
111
010

010
111
010

00100
00200
12121
00200
00100

1234567
._____. 1
|.....| 2
|.....| 3
|_____| 4

_______
/_/ | |
[_|D--D
HHH|

1234567.234567.

               _______
              /_/ |  |
              [_|D---D
              HHH|H
                 |
                 |
                 |
 _____         __|__         _____
|     |       |     |       |     |
|     ---------     ---------     |
|_____|       |__-__|       |_____|
                 |
                 |
                 |
                 |
               __|__
              |     |
              |     |
              |_____|

SWNE = 0000


1234
ENWS = 0000

EMPTY - no entities

convert room -> read each in from file? cant do this every time

DEADEND
boolean array
E 1000
 _____
|     |
|     -
|_____|

N 0100
 __|__
|     |
|     |
|_____|

W 0010
 _____
|     |
-     |
|_____|


S 0001
 _____
|     |
|     |
|__-__|

VAN
S 0001
 _______
/_/ |  |
[_|D---D
   |



JUNCTION_S

EN..
1100
EN
 __|__
|     |
|     -
|_____|

E.W.
1010
EW
 _____
|     |
-     -
|_____|

E..S
1001
ES
 _____
|     |
|     -
|__-__|

.NW.
0110
NW
 __|__
|     |
-     |
|_____|

0101
.N.S
NS
 __|__
|     |
|     |
|__-__|


0011
WS
..WS
 _____
|     |
-     |
|__-__|



JUNCTION_L

ENW.
1110
 __|__
|     |
-     -
|_____|

EN.S
1101
 __|__
|     |
|     -
|__-__|

E.WS
1011
 _____
|     |
-     -
|__-__|

.NWS
0111
 __|__
|     |
-     |
|__-__|


HALLWAY
ENWS
1111

 __|__
|     |
-     -
|__-__|

FOYER
 __|__
|     |
-     -
|__-__|

*/
