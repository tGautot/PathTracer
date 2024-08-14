
CFLAGS=-g -fopenmp -DSIMPLE_DEBUG
INCLUDES=-Iheaders -Iexternal

clear:
	rm -f build/* && echo "Cleared build dir"

ray:
	g++ $(CFLAGS) $(INCLUDES) -c sources/ray.cpp -o build/ray.o

vec:
	g++ $(CFLAGS) $(INCLUDES) -c sources/vec3.cpp -o build/vec3.o

color:
	g++ $(CFLAGS) $(INCLUDES) -c sources/color.cpp -o build/color.o

interval:
	g++ $(CFLAGS) $(INCLUDES) -c sources/interval.cpp -o build/interval.o 

all: clear vec color interval ray
	g++ $(CFLAGS) $(INCLUDES) main.cpp build/color.o build/vec3.o build/interval.o build/ray.o  -o build/PathTracer