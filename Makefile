# Makefile for gravitySim

# Compiler
CXX = g++
CXXFLAGS = -std=c++20 -Wall -I./include

# Sources and output
SRC = $(wildcard src/*.cpp)
OUT = app

# Libraries to link
LIBS = -lglfw -lGL -lGLU -ldl -lm

# Build rule
all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT) $(LIBS)
	./$(OUT)

# Clean rule
clean:
	rm -f $(OUT)
