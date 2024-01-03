test: test.o Matrix.o ExpandedMatrix.o Coordinate.o MapFoundation.o
	g++ -o test test.o Matrix.o ExpandedMatrix.o Coordinate.o MapFoundation.o

test.o: test.cc
	g++ -c test.cc

MapFoundation.o: MapFoundation.cc MapFoundation.h ExpandedMatrix.h Coordinate.h
	g++ -c MapFoundation.cc

Matrix.o: Matrix.cc Matrix.h Coordinate.h
	g++ -c Matrix.cc

ExpandedMatrix.o: ExpandedMatrix.cc ExpandedMatrix.h Matrix.h Coordinate.h
	g++ -c ExpandedMatrix.cc

Coordinate.o: Coordinate.cc Coordinate.h
	g++ -c Coordinate.cc

clean:
	rm -f *.o test
