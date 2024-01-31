mapBuilder
C++ application
can compile & execute on LINUX OS, WINDOWS OS, MAC OS

--------------------------------------------------------------------------------------------------------------

TO RUN:
    open console in only directory containing all files

    make executable:
        make

    run program:
        ./test

    LINUX OS & MAC OS
        remove executable and *.o files:
            make clean

    WINDOWS OS
        remove executable and *.o files:
            make wclean

--------------------------------------------------------------------------------------------------------------

CONSOLE PRINTOUT ON EXECUTION:

1s are ROOMS
2s are doors connecting ROOMS
'.' are zeros and are modified when printed for an easy visual of the map layout

@ is the first spawn location for this map taken from the centre coordinate from the original nxn base matrix
G represents second spawn location taken from the last room marked visited ( last element in the map->rooms array 

the map is made square nxn and all non 0 data is translated to top left for purpose of use with a gui window.

Map is randomly generated (time seeded) on each execution

--------------------------------------------------------------------------------------------------------------

ABOUT THE MAP:

to modify the maximum generated map size, in test.cc change 'dimension' value in main() to any desired integer value.


dimension represents the base matrix that is expanded to dimension + (dimension - 1):

[ Matrix ]                                       [ ExpandedMatrix ]      [ removeCommonLoops() ]
                         1 . 1 . 1                  1 2 1 2 1                  1 2 1 . 1
  1 1 1                  . . . . .                  2 . 2 . 2                  2 . 2 . 2
  1 1 1   >> becomes >>  1 . 1 . 1  >> becomes >>   1 2 1 2 1  >> becomes >>   1 . 1 2 1
  1 1 1                  . . . . .                  2 . 2 . 2                  2 . 2 . 2
                         1 . 1 . 1                  1 2 1 2 1                  1 2 1 . 1


MapFoundation will walk through the matrix starting at the centre coordinate and mark all navigable rooms,
or rooms connected with doors reachable from the central room, and remove any isolated room/doors from
the main branching structure.

MapFoundation will then crop the map and square it based on the largest/smallest x & y coordinates of
the main branching structure.

the Map instance uses the MapFoundation instance to hand off the completed map data without
including all member functions/variables unusable (shouldnt be used) after making the finalized map.


change 'dimension' Note:

'dimension = 400' means:
400x400 base matrix rounded to 399x399 (for a valid centre cell) & expanded to 797x797 before cropping

dimension Expanded Becomes:
    dimension + (dimension - 1)

i've tried around 100 attempts at varying dimension = 200,300,400 and have not produced a map larger
than 100x100 after cropping. dimension >= 400 will start to take 2-3 s to execute without any debug.


EXAMPLE CONSOLE PRINTOUT [dimension = 100]:
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
