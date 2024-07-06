mapBuilder
C++ application
can compile & execute on LINUX OS, WINDOWS OS, MAC OS

--------------------------------------------------------------------------------------------------------------

TO RUN:
    open console in directory containing Makefile

    make executable w. Makefile:
        make

    run program:
        ./test

    LINUX OS & MAC OS
        remove executable and *.o files:
            make clean

    WINDOWS OS
        remove *.exe executable and *.o files:
            make wclean

--------------------------------------------------------------------------------------------------------------

CONSOLE PRINTOUT ON EXECUTION:

1s are nodes
2s are edges connecting nodes
'.' are zeros and are modified only when printed for an easy visual of the map layout

@ is the primary spawn location for this map
G is the secondary spawn location for this map

Map data is translated to the top left of the squared map for purposes of working with a gui window

Map is randomly generated (in this example time seeded) on each execution using the srand() function

--------------------------------------------------------------------------------------------------------------

ABOUT THE MAP:

to modify the maximum generated map size, in test.cc change 'maxBaseDimension' value in main() to any desired
integer value.

maxBaseDimension represents the base matrix square dimension n x n that is expanded to ( 2n - 1) x ( 2n - 1) :

[ Matrix ]                                       [ ExpandedMatrix ] [ if REMOVE_COMMON_LOOPS constant is set ]
                         1 . 1 . 1                  1 2 1 2 1                     1 2 1 . 1
  1 1 1                  . . . . .                  2 . 2 . 2                     2 . 2 . 2
  1 1 1   >> becomes >>  1 . 1 . 1  >> becomes >>   1 2 1 2 1  >> becomes >>      1 . 1 2 1
  1 1 1                  . . . . .                  2 . 2 . 2                     2 . 2 . 2
                         1 . 1 . 1                  1 2 1 2 1                     1 2 1 . 1


MapFoundation will walk through the matrix starting at the centre coordinate and mark all nodes
connected with edges reachable from the central node, then remove any isolated nodes/edges from
the main branching structure.

MapFoundation will then crop the map and square it based on the largest/smallest x & y coordinates of
the entire main branching structure.

the Map instance uses the MapFoundation instance to build the final map data without
including all member functions/variables unusable after making the finalized map.

when modifying 'maxBaseDimension' Note:

    'maxBaseDimension = 400' means:
        400x400 base matrix rounded to 399x399 (for a valid centre cell) & expanded to 797x797 before cropping

    maxBaseDimension Expanded Becomes:
        ( maxBaseDimension + (maxBaseDimension - 1) ) ^2 matrix

    at varying maxBaseDimension = 200,300,400, it is unlikely to produce a map larger than 100x100 after
    cropping

    maxBaseDimension = 400 will start to take a minimum 5-6 s to execute depending on your system

--------------------------------------------------------------------------------------------------------------


EXAMPLE CONSOLE PRINTOUT [maxBaseDimension = 30]:

-------------------------------------------------

ALL ADJACENCY DATA
    1 : (14,18) = (16,18) (14,16)
    2 : (16,16) = (14,16)
    3 : (16,18) = (14,18)
    4 : (20,16) = (20,14)
    5 : (14,16) = (16,16) (14,14) (14,18)
    6 : (20,14) = (20,12) (20,16)
    7 : (20,12) = (22,12) (20,10) (20,14)
    8 : (18,12) = (18,10)
    9 : (18,10) = (20,10) (18, 8) (16,10) (18,12)
   10 : (16,10) = (18,10) (16, 8)
   11 : (18, 8) = (18,10)
   12 : (14, 6) = (16, 6) (12, 6) (14, 8)
   13 : (12, 6) = (14, 6)
   14 : (20,10) = (18,10) (20,12)
   15 : (16, 8) = (14, 8) (16,10)
   16 : (16, 4) = (18, 4) (16, 6)
   17 : (12,10) = (12, 8)
   18 : ( 8, 8) = (10, 8) ( 8,10)
   19 : ( 4,14) = ( 4,12) ( 2,14)
   20 : (18, 4) = (18, 2) (16, 4)
   21 : (10, 8) = (12, 8) ( 8, 8)
   22 : ( 2,12) = ( 4,12)
   23 : ( 2,14) = ( 4,14) ( 0,14)
   24 : (14, 8) = (16, 8) (14, 6) (12, 8)
   25 : ( 6,12) = ( 8,12) ( 4,12)
   26 : (22,12) = (20,12)
   27 : ( 2, 6) = ( 2, 8)
   28 : (14,14) = (14,12) (12,14) (14,16)
   29 : (10,12) = (10,14)
   30 : ( 2, 8) = ( 4, 8) ( 2, 6)
   31 : ( 0,14) = ( 2,14)
   32 : (16, 6) = (16, 4) (14, 6)
   33 : (12, 8) = (14, 8) (10, 8) (12,10)
   34 : ( 8,10) = ( 8, 8) ( 8,12)
   35 : ( 4,12) = ( 6,12) ( 4,10) ( 2,12) ( 4,14)
   36 : (16, 2) = (18, 2) (16, 0)
   37 : ( 8,14) = (10,14) ( 8,12)
   38 : ( 4, 8) = ( 2, 8) ( 4,10)
   39 : ( 0,10) = ( 2,10)
   40 : (16, 0) = (16, 2)
   41 : (12,14) = (14,14) (10,14)
   42 : ( 8,12) = ( 8,10) ( 6,12) ( 8,14)
   43 : ( 4,10) = ( 4, 8) ( 2,10) ( 4,12)
   44 : (18, 2) = (16, 2) (18, 4)
   45 : (14,12) = (14,14)
   46 : (10,14) = (12,14) (10,12) ( 8,14)
   47 : ( 2,10) = ( 4,10) ( 0,10)

-------------------------------------------------

   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2

0  . . . . . . . . . . . . . . . . 1 . . . . . .
1  . . . . . . . . . . . . . . . . 2 . . . . . .
2  . . . . . . . . . . . . . . . . 1 2 1 . . . .
3  . . . . . . . . . . . . . . . . . . 2 . . . .
4  . . . . . . . . . . . . . . . . 1 2 1 . . . .
5  . . . . . . . . . . . . . . . . 2 . . . . . .
6  . . 1 . . . . . . . . . 1 2 1 2 1 . . . . . .
7  . . 2 . . . . . . . . . . . 2 . . . . . . . .
8  . . 1 2 1 . . . 1 2 1 2 1 2 1 2 1 . 1 . . . .
9  . . . . 2 . . . 2 . . . 2 . . . 2 . 2 . . . .
0  1 2 @ 2 1 . . . 1 . . . 1 . . . 1 2 1 2 1 . .
1  . . . . 2 . . . 2 . . . . . . . . . 2 . 2 . .
2  . . 1 2 1 2 1 2 1 . 1 . . . 1 . . . 1 . 1 2 1
3  . . . . 2 . . . 2 . 2 . . . 2 . . . . . 2 . .
4  1 2 1 2 1 . . . 1 2 1 2 1 2 1 . . . . . 1 . .
5  . . . . . . . . . . . . . . 2 . . . . . 2 . .
6  . . . . . . . . . . . . . . 1 2 1 . . . 1 . .
7  . . . . . . . . . . . . . . 2 . . . . . . . .
8  . . . . . . . . . . . . . . 1 2 G . . . . . .
9  . . . . . . . . . . . . . . . . . . . . . . .
0  . . . . . . . . . . . . . . . . . . . . . . .
1  . . . . . . . . . . . . . . . . . . . . . . .
2  . . . . . . . . . . . . . . . . . . . . . . .

-------------------------------------------------
   ENTITY SPAWN1 START: @ ( 2,10)
   ENTITY SPAWN2 START: G (16,18)

-------------------------------------------------
