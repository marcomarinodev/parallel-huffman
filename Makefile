CXX = g++
CFLAGS = -Wall -g -pthread
TARGET = main
MAKEFLAGS = --no-print-directory
ASX = asx

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CXX) $(CFLAGS) $(TARGET).cpp ./$(ASX)/$(ASX)1.cpp -o $(TARGET)

run:
	./$(TARGET)

clean:
	$(RM) $(TARGET)

rebuild:
	make $(MAKEFLAGS) clean && make $(MAKEFLAGS) && make $(MAKEFLAGS) run