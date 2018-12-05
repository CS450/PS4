all:p1.x

p1.x: p1.c
	g++ -std=c++11 p1.cpp -o p1.x

p2.x: p2.cpp
	g++ -std=c++11 p2.cpp -o p3.x

p3.x: p3.cpp
	g++ -std=c++11 p3.cpp -o p3.x
clean: 
	rm -f *.o *~ *.x

