all: result

result: unit_test.o vector2d.o
	g++ unit_test.o vector2d.o -o result

vector2d.o: vector2d.cpp
	g++ vector2d.cpp -c

main.o: unit_test.cpp
	g++ unit_test.cpp -c

clean:
	rm *.o
	rm result
