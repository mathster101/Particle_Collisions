COMPILE_FLAGS = -static-libgcc -static-libstdc++ -pedantic -O0
SFML_INCL = "C:/Dev/SFML-2.5.1/include"
SFML_LIBS = -L"C:/Dev/SFML-2.5.1/lib" -lsfml-graphics -lsfml-window -lsfml-system


all: particles main output


particles: src/particles.cpp include/particles.h
	g++ -c src/particles.cpp $(COMPILE_FLAGS) -I$(SFML_INCL) -o objects/particles.o

main: src/main.cpp
	g++ -c src/main.cpp $(COMPILE_FLAGS) -I$(SFML_INCL) -o objects/main.o

output:
	g++ objects/main.o objects/particles.o -o output $(SFML_LIBS)

clean:
	del objects\*.*
