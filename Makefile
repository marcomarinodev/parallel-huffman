CXX = g++
CFLAGS = -O3 -Wall -g -pthread
TARGET = main
MAKEFLAGS = --no-print-directory
ASXS = ./asx/*.cpp

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CXX) $(CFLAGS) $(TARGET).cpp $(ASXS) -o $(TARGET)

run:
	./$(TARGET) $(SIZE)

clean:
	$(RM) $(TARGET)

rebuild:
	make $(MAKEFLAGS) clean && make $(MAKEFLAGS) && make $(MAKEFLAGS) run