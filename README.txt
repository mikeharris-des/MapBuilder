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


EXAMPLE CONSOLE PRINTOUT [maxBaseDimension = 100]:
-------------------------------------------------------
 . . . . . . . . . . . . . . . . . . . . . . . . . . . . 1 . . . . . . . . . . . . . .
 . . . . . . . . . . . . . . . . . . . . . . . . . . . . 2 . . . . . . . . . . . . . .
 . . . . . . . . . . . . . . . . . . . . . . . . . . . . 1 . . . . . 1 . . . . . . . .
 . . . . . . . . . . . . . . . . . . . . . . . . . . . . 2 . . . . . 2 . . . . . . . .
 . . . . . . . . . . . . . . . . . . . . . . . . . . . . 1 2 1 . 1 2 1 . . . . . . . .
 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 2 . 2 . . . . . . . . . .
 . . . . . . . . . . . . . . . . . . . . . . . . 1 . . . . . 1 2 1 . . . . . . . . . .
 . . . . . . . . . . . . . . . . . . . . . . . . 2 . . . . . 2 . . . . . . . . . . . .
 . . . . . . . . . . . . . . . . . . . . . . . . 1 . . . . . 1 2 1 2 1 2 1 . . . . . .
 . . . . . . . . . . . . . . . . . . . . . . . . 2 . . . . . 2 . . . . . 2 . . . . . .
 . . . . . . . . . . . . . . . . . . . . . . . . 1 . . . 1 2 1 . . . . . 1 . . . . . .
 . . . . . . . . . . . . . . . . . . . . . . . . 2 . . . 2 . . . . . . . . . . . . . .
 . . . . . . . . . . . . . . . . . . . . . . . . 1 2 1 . 1 2 1 2 1 . . . . . . . . . .
 . . . . . . . . . . . . . . . . . . . . . . . . . . 2 . 2 . . . . . . . . . . . . . .
 . . . . . . . . . . . . . . . . . . . . . . . . . . 1 . 1 . . . . . . . . . . . . . .
 . . . . . . . . . . . . . . . . . . . . . . . . . . 2 . 2 . . . . . . . . . . . . . .
 . . . . . . . . . . . . . . . . . . . . . . . . 1 2 1 2 1 . . . 1 2 1 2 1 . . . . . .
 . . . . . . . . . . . . . . . . . . . . . . . . 2 . . . . . . . 2 . . . 2 . . . . . .
 . . . . . . . . . . . . . . . . . . . . . . 1 2 1 2 1 . . . . . 1 . . . 1 . . . . . .
 . . . . . . . . . . . . . . . . . . . . . . . . . . 2 . . . . . . . . . 2 . . . . . .
 . . . . . . . . . . . . . . . . . . . . . . . . 1 2 1 2 1 2 1 . . . 1 2 1 2 1 . . . .
 . . . . . . . . . . . . . . . . . . . . . . . . . . . . 2 . . . . . . . 2 . . . . . .
 . . . . . . . . . . . . . . . . . . . . . . 1 . . . . . 1 . . . . . . . 1 . . . 1 . 1
 . . . . . . . . . . . . . . . . . . . . . . 2 . . . . . 2 . . . . . . . 2 . . . 2 . 2
 . . . . . . . . . . . . . . . . . . . . . . 1 2 1 2 1 2 1 . 1 2 1 2 1 2 1 2 1 2 1 2 1
 . . . . . . . . . . . . . . . . . . . . . . . . 2 . . . 2 . 2 . . . . . 2 . . . . . .
 . . 1 . . . . . . . . . . . . . . . . . . . . . 1 . . . 1 2 1 . . . . . 1 . . . . . .
 . . 2 . . . . . . . . . . . . . . . . . . . . . 2 . . . . . . . . . . . . . . . . . .
 1 2 1 . . . 1 . . . . . . . . . . . . . 1 . 1 2 @ 2 1 . . . . . . . . . . . . . . . .
 . . 2 . . . 2 . . . . . . . . . . . . . 2 . 2 . 2 . . . . . . . . . . . . . . . . . .
 1 2 1 2 1 2 1 2 1 2 1 2 1 2 1 . . . 1 2 1 2 1 . 1 . . . . . . . . . . . . . . . . . .
 . . 2 . . . 2 . . . 2 . . . 2 . . . . . . . 2 . . . . . . . . . . . . . . . . . . . .
 . . 1 . . . 1 2 1 2 1 . . . 1 2 1 2 1 2 1 2 1 . . . . . . . . . . . . . . . . . . . .
 . . 2 . . . . . 2 . . . . . . . 2 . . . . . 2 . . . . . . . . . . . . . . . . . . . .
 . . 1 2 1 . . . 1 . . . 1 . . . 1 . . . . . 1 2 1 2 1 . 1 . . . . . . . . . . . . . .
 . . 2 . . . . . 2 . . . 2 . . . 2 . . . . . . . 2 . 2 . 2 . . . . . . . . . . . . . .
 1 2 1 . . . . . 1 2 1 2 1 2 1 2 1 . . . . . . . 1 . 1 2 1 . . . . . . . . . . . . . .
 2 . . . . . . . 2 . . . . . . . . . . . . . . . 2 . . . 2 . . . . . . . . . . . . . .
 G . . . . . . . 1 . . . . . . . . . . . . . 1 2 1 . . . 1 . . . . . . . . . . . . . .
 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

-------------------------------------------------------
   SPAWN1 START @ (24,28)
   SPAWN2 START G (0,38)

-------------------------------------------------------
