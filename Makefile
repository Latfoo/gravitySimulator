# Makefile for gravitySim

# Compiler
CXX = g++
CXXFLAGS = -Wall -std=c++20

# Source and output
SRC = gravitySim.cpp
OUT = app

# Libraries to link
LIBS = -lglfw -lGL -ldl -lm

# Build rule
all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT) $(LIBS)
	./$(OUT)

# Clean rule
clean:
	rm -f $(OUT)
