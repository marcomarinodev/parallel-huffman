CXX = g++
CFLAGS = -O3 -Wall -g -pthread
TARGET = main
MAKEFLAGS = --no-print-directory
ASXS = ./asx/*.cpp

output: ./libs/main.o ./libs/asx1.o ./libs/asx2.o ./libs/utils.o 
	$(CXX) $(CFLAGS) ./libs/main.o ./libs/asx1.o ./libs/asx2.o ./libs/utils.o -o output

./libs/main.o: main.cpp
	$(CXX) $(CFLAGS) -c main.cpp -o ./libs/main.o

./libs/asx1.o: ./asx/asx1.cpp ./asx/asx1.hpp
	$(CXX) $(CFLAGS) -c ./asx/asx1.cpp -o ./libs/asx1.o

./libs/asx2.o: ./asx/asx2.cpp ./asx/asx2.hpp
	$(CXX) $(CFLAGS) -c ./asx/asx2.cpp -o ./libs/asx2.o

./libs/utils.o: ./asx/utils.cpp ./asx/utils.hpp
	$(CXX) $(CFLAGS) -c ./asx/utils.cpp -o ./libs/utils.o

clean:
	rm ./libs/*.o output