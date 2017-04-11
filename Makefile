o: main.cpp parameters.cpp functions.cpp
	g++ `pkg-config --cflags --libs opencv` -std=c++11 main.cpp parameters.cpp functions.cpp  -o o
