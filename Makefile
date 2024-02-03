test: test.o Coordinate.o Matrix.o MapFoundation.o Map.o
	g++ -o test test.o Coordinate.o Matrix.o MapFoundation.o Map.o

test.o: test.cc
	g++ -c test.cc

Map.o: Map.cc MapFoundation.h Matrix.h Coordinate.h
	g++ -c Map.cc

MapFoundation.o: MapFoundation.cc MapFoundation.h Matrix.h Coordinate.h
	g++ -c MapFoundation.cc

Matrix.o: Matrix.cc Matrix.h Coordinate.h
	g++ -c Matrix.cc

Coordinate.o: Coordinate.cc Coordinate.h
	g++ -c Coordinate.cc

clean:
	rm -f *.o test

wclean:
	del -f *.o test.exe
