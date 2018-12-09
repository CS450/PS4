all:p1.x p2.x p3.x

p1.x: p1.cpp
	g++ -std=c++11 -D PROB1 p1.cpp -o p1.x

p2.x: p1.cpp
	g++ -std=c++11 p1.cpp -o p2.x

p3.x: p3.c
	gcc p3.c -o p3.x
clean: 
	rm -f *.o *~ *.x

