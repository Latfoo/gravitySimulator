# Real-Time Gravity Simulation (C++)

This is a personal project where I'm learning to build a real-time simulation of celestial bodies using C++. It began as a simple 2D circle bouncing under gravity, modeled with basic kinematics. Since then, the simulation has evolved significantly: from kinematics to full Newtonian kinetics, and from 2D to 3D.

It now models gravitational interactions and perfectly elastic collisions between spherical objects, visualized in 3D using OpenGL. A GUI has been added to experiment with and observe the relative error of different numerical integrators. The GUI also includes controls for interacting with and managing the simulation at runtime.

## Simulation Preview

### Video

[![Short Demo Video](https://Latfoo.github.io/gravitySimulator/preview.gif)](https://Latfoo.github.io/gravitySimulator/demo_video.mp4)
> ⚠️ This is a GIF preview. Hence some stuttering or quality loss. Click the GIF to see a short demo video including GUI!

### GUI Preview
![GUI Preview](https://Latfoo.github.io/gravitySimulator/gui_demo.png)


## Numerical Integrators

The simulation supports four numerical integrators for advancing the physics each frame:

| Integrator | Order | Symplectic |
|---|---|---|
| Explicit Euler | 1st | No |
| Semi-implicit Euler | 1st | Yes |
| Leapfrog | 2nd | Yes |
| RK4 | 4th | No |

Symplectic integrators are structure-preserving methods for Hamiltonian
systems that exactly preserve the symplectic 2-form of phase space. By
backward error analysis, they can be interpreted as exactly solving a nearby
"shadow" Hamiltonian whose value stays close to that of the original system.
As a consequence, the energy error remains bounded and oscillates rather than
drifting, over exponentially long time intervals in the step size.
Non-symplectic methods carry no such guarantee. Explicit Euler, a low-order
non-symplectic scheme, shows unbounded secular growth in energy for typical
oscillatory systems. A high-order non-symplectic method such as RK4 can still
achieve negligible drift on practical timescales, though without any long-time
conservation theorem. [1]

[1] Hairer, E., Lubich, C., & Wanner, G. (2006). *Geometric Numerical
    Integration: Structure-Preserving Algorithms for Ordinary Differential
    Equations* (2nd ed.). Springer Series in Computational Mathematics,
    vol. 31. Chapters I and IX.
    
### Energy Conservation Benchmark

The plot below shows the relative energy error `|ΔE/E₀|` over simulated time for each integrator, applied to the same N-body initial conditions:

![Energy Error Plot](https://Latfoo.github.io/gravitySimulator/energy_errors.png)

## Notes

- The code is work in progress
- Currently only tested and built in Linux Ubuntu
- [`CHANGELOG.md`](./CHANGELOG.md) documents current features and development steps
- Built with C++, OpenGL, GLFW, Imgui, Implot

## Scalability

The simulation uses a direct N-body approach where every pair of bodies interacts gravitationally, resulting in O(N²) force evaluations per frame. This is the main bottleneck when scaling to larger body counts.

### Current optimizations
- **Newton's third law**: Each pair is evaluated once instead of twice, halving the number of force computations.

### On multithreading (OpenMP)

Parallelizing the force loop across CPU cores is the obvious next step, but it comes with a tradeoff. The Newton's third law optimization requires both bodies in a pair to be updated simultaneously, which introduces write conflicts between threads. The clean alternative is to give each body its own independent loop over all others: Safe to parallelize, but at the cost of redundant (2x) force evaluations.

Whether this tradeoff is worthwhile depends on the number of bodies and available cores:

| Bodies   | Parallel worth it?                |
|----------|-----------------------------------|
| < 100    | No — thread overhead dominates    |
| ~100–200 | Marginal — roughly break-even     |
| 500+     | Yes — clear speedup              |
| 1,000+   | Essential                         |

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
# With default scene file
./GravitySim ../scene.json

# Or with a custom scene file (e.g. from an API or script)
./GravitySim /path/to/custom_scene.json
```
If no argument is given and no `scene.json` is found, the simulation falls back to a built-in default scene.

### Benchmark

To regenerate the energy error plot, build the `Benchmark` target and run the Python plotting script. The benchmark requires `pandas` and `matplotlib` which can be installed via `pip3 install pandas matplotlib`.

```bash
cmake --build build --target Benchmark
./build/Benchmark
python3 tools/plot_errors.py
```

## Author
Latfoo
