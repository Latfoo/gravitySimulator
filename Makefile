# Makefile for gravitySim

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -Wall -I./include

# Source files and output binary
SRC = $(wildcard src/*.cpp)
OUT = app

# Libraries to link
LIBS = -lglfw -lGL -lGLU -ldl -lm -lpthread

# Default build rule
all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT) $(LIBS)
	./$(OUT)

# Rule to convert frames into video
video:
	ffmpeg -framerate 30 -i video_frames/frame_%04d.ppm -c:v libx264 -pix_fmt yuv420p output.mp4

# Clean rule - removes binary, frames, and final video
clean:
	rm -f $(OUT)
	rm -f video_frames/*.ppm
	rm -f output.mp4
