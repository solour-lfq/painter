o: main.cpp parameters.cpp functions.cpp
	g++ `pkg-config --cflags --libs opencv` main.cpp parameters.cpp functions.cpp  -o o
