# Real-Time Gravity Simulation (C++)

This is a personal project where I'm learning to build a real-time simulation of celestial bodies using C++. It began as a simple 2D circle bouncing under gravity, modeled with basic kinematics. Since then, the simulation has evolved significantly: from kinematics to full Newtonian kinetics, and from 2D to 3D.

It now models gravitational interactions and perfectly elastic collisions between spherical objects, visualized in 3D using legacy OpenGL. A minimal GUI has been added to experiment with and observe the relative error of different numerical integrators. The GUI also includes controls for interacting with and managing the simulation at runtime.

This is my first attempt at building a full C++ application outside of university or academic exercises. Starting simple and steadily adding features and accuracy as I learn.

## Simulation Preview

### Video

[![Short Demo Video](https://Latfoo.github.io/gravitySimulator/preview.gif)](https://Latfoo.github.io/gravitySimulator/demo_video.mp4)
> ⚠️ This is a GIF preview. Hence some stuttering or quality loss. Click the GIF to see a short demo video including GUI!

### GUI Preview
![GUI Preview](https://Latfoo.github.io/gravitySimulator/gui_demo.png)


## Notes

- The code is work in progress
- Currently only tested and built in Linux Ubuntu
- [`CHANGELOG.md`](./CHANGELOG.md) documents current features and development steps
- Built with C++, OpenGL, GLFW, Imgui, Implot

## Further Goals

- Make it interactive --> enable manual configuration of initial conditions via GUI Window
- Improve physical accuracy and structure over time
- Make it cross-platform compatible
- Learn by building, not just reading

## Build

### Necessary Packages

These are the required packages for building and running the simulation:

- [Core OpenGL (libGL)](https://mesa3d.org/) — Mesa's implementation of the OpenGL specification
- [OpenGL Utility Library (GLU)](https://www.opengl.org/resources/libraries/glut/) — A set of utilities for OpenGL
- [GLFW Library](https://www.glfw.org/) — Window creation, OpenGL context, and input handling
- [CMake](https://cmake.org/) — Cross-platform build system generator used to configure and build the project



#### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install libgl1-mesa-dev libglu1-mesa-dev libglfw3-dev cmake
```
If `apt` is not available (e.g. on Arch or Fedora), use your distribution’s package manager instead (like `pacman` or `dnf`).

#### MacOS

To install required dependencies on macOS, you can use [Homebrew](https://brew.sh/), the macOS package manager.
Once Homebrew is installed, install GLFW:
```bash
brew install glfw cmake
```
OpenGL is already included with macOS as a system framework (OpenGL.framework), so no additional OpenGL installation is required.

### Build Instructions (Linux and MacOS only):

Build the project:
```bash
# Create and enter the build directory
mkdir build && cd build

# Configure the build system (only to be repeated after major changes like new dependencies or files)
cmake ..

# Compile the project 
cmake --build .
```
Run the simulation:
```bash
./GravitySim
```

## Author
Latfoo

