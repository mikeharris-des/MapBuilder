mapBuilder
C++ application
can compile & execute on LINUX OS, WINDOWS OS, MAC OS

--------------------------------------------------------------------------------------------------------------

RUN PROGRAM INSTRUCTIONS:

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
    '.' are zeros and are modified only in output stream for an easy visual of the map layout

    @ is the primary unique node coordinate for this map
    # is any unique node coordinate or 'Select Node' for this map (a map can be set to have more than 2)

    Map data is translated to the top left of the squared map for purposes of working with a specific gui window

    Map is randomly generated (in this example time seeded) on each execution using the srand() function

--------------------------------------------------------------------------------------------------------------



EXAMPLE CONSOLE PRINTOUT with maxBaseDimensionOfMatrix = 30, numSelectNodes = 2 :

    -------------------------------------

    ALL ADJACENCY DATA

        1 : ( 4,16) = ( 6,16) ( 4,14)
        2 : ( 6,16) = ( 8,16) ( 4,16)
        3 : ( 8,16) = (10,16) ( 6,16)
        4 : (10,16) = (10,14) ( 8,16)
        5 : ( 0, 8) = ( 2, 8)
        6 : ( 4,10) = ( 6,10) ( 2,10)
        7 : (12,14) = (10,14)
        8 : ( 6, 8) = ( 6,10)
        9 : ( 2,10) = ( 4,10) ( 2, 8) ( 2,12)
       10 : (10,14) = (12,14) (10,12) (10,16)
       11 : ( 6,10) = ( 6, 8) ( 4,10) ( 6,12)
       12 : ( 2, 8) = ( 0, 8) ( 2,10)
       13 : (10,12) = (10,10) (10,14)
       14 : ( 6,12) = ( 6,10)
       15 : ( 2,14) = ( 4,14) ( 2,12)
       16 : (14, 8) = (14,10)
       17 : (10,10) = (12,10) (10, 8) (10,12)
       18 : ( 8, 0) = ( 8, 2)
       19 : ( 8, 2) = ( 8, 0) ( 6, 2) ( 8, 4)
       20 : ( 4,14) = ( 2,14) ( 4,16)
       21 : (12,10) = (14,10) (12, 8) (10,10)
       22 : ( 6, 2) = ( 8, 2)
       23 : (10, 4) = (12, 4) (10, 6)
       24 : ( 2,12) = ( 2,10) ( 2,14)
       25 : (14,10) = (14, 8) (12,10)
       26 : (10, 8) = (10, 6) (10,10)
       27 : ( 8, 6) = (10, 6) ( 8, 4)
       28 : (12, 4) = (10, 4) (12, 6)
       29 : ( 8, 4) = ( 8, 2) ( 8, 6)
       30 : (12, 6) = (12, 4) (12, 8)
       31 : (10, 6) = (10, 4) ( 8, 6) (10, 8)
       32 : (12, 8) = (12, 6) (12,10)

    -------------------------------------

       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6

    0  . . . . . . . . 1 . . . . . . . .
    1  . . . . . . . . 2 . . . . . . . .
    2  . . . . . . 1 2 1 . . . . . . . .
    3  . . . . . . . . 2 . . . . . . . .
    4  . . . . . . . . 1 . 1 2 1 . . . .
    5  . . . . . . . . 2 . 2 . 2 . . . .
    6  . . . . . . . . 1 2 1 . 1 . . . .
    7  . . . . . . . . . . 2 . 2 . . . .
    8  # 2 1 . . . 1 . . . 1 . @ . 1 . .
    9  . . 2 . . . 2 . . . 2 . 2 . 2 . .
    0  . . 1 2 1 2 1 . . . 1 2 1 2 1 . .
    1  . . 2 . . . 2 . . . 2 . . . . . .
    2  . . 1 . . . 1 . . . 1 . . . . . .
    3  . . 2 . . . . . . . 2 . . . . . .
    4  . . 1 2 1 . . . . . 1 2 1 . . . .
    5  . . . . 2 . . . . . 2 . . . . . .
    6  . . . . 1 2 1 2 1 2 1 . . . . . .

    -------------------------------------

       SELECT NODE [ 1 ] START: @ (12, 8)
       SELECT NODE [ 2 ] START: # ( 0, 8)

    -------------------------------------



--------------------------------------------------------------------------------------------------------------

ABOUT MAP GENERATION:

    to modify the maximum generated map size, in test.cc change 'maxBaseDimensionOfMatrix' value in main() to any
    desired integer value.

    maxBaseDimensionOfMatrix represents the base matrix square dimension n x n that is expanded to
    ( 2n - 1) x ( 2n - 1) :

        [ Matrix ]                                    [ ExpandedMatrix ] [ if REMOVE_COMMON_LOOPS is set]
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

    the Map instance uses the MapFoundation instance to build all final map data with abstraction from
    the member functions/variables used in the generation process

    when modifying 'maxBaseDimensionOfMatrix' :

        'maxBaseDimensionOfMatrix = 400' means:
            400x400 base matrix rounded to 399x399 (for a valid centre cell) & expanded to 797x797 before
            cropping

        OR maxBaseDimensionOfMatrix Expanded becomes:
            ( maxBaseDimensionOfMatrix + (maxBaseDimensionOfMatrix - 1) )^2 for the full matrix dimension

        varying maxBaseDimensionOfMatrix to have a value of 200,300,400, will likely to produce a map of size 100x100
        after cropping

        maxBaseDimensionOfMatrix = 400 will start to take a minimum 5-6 s to execute depending on your system

--------------------------------------------------------------------------------------------------------------
