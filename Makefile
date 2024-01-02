test: test.o Matrix.o ExpandedMatrix.o
	g++ -o test test.o Matrix.o ExpandedMatrix.o

test.o: test.cc
	g++ -c test.cc

Matrix.o: Matrix.cc Matrix.h
	g++ -c Matrix.cc

ExpandedMatrix.o: ExpandedMatrix.cc ExpandedMatrix.h Matrix.h
	g++ -c ExpandedMatrix.cc

clean:
	rm -f *.o test
