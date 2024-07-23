
clear:
	rm build/*

vec:
	g++ -Iheaders -c sources/vec3.cpp -o build/vec3.o

color:
	g++ -Iheaders -c sources/color.cpp -o build/color.o

interval:
	g++ -Iheaders -c sources/interval.cpp -o build/interval.o

all: clear vec color interval
	g++ -Iheaders main.cpp build/color.o build/vec3.o build/interval.o  -o build/PathTracer