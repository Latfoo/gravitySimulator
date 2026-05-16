# Changelog

## [v0.9] - 2026-03-02

### Features
- Implemented the possibility to load inital scenes via JSON files
  - Add nlohmann/json (header-only) for JSON parsing
  - Add SceneLoader to parse scene files into Planet vectors
  - Accept scene file path as CLI argument (./GravitySim path/to/scene.json)

### Technical Improvements
- Halved force pair evaluations for improved performance, utilising Newton's third law

## [v0.8] - 2025-08-08

### Features
- Implemented orbit-style interactive 3D camera using OpenGL:
  - Mouse-based rotation (yaw and pitch)
  - Middle-click panning
  - Scroll-wheel zooming
- Camera orbits around a central point (e.g., origin) with pan and zoom constraints.
- Added GUI button to reset the camera to its default view.

### Technical Improvements
- Refactored main render loop for readability


## [v0.7] - 2025-08-06
### Features
- Integrated ImGui and ImPlot for an interactive Simulation HUD.
- Integrated different numerical integrators.
- Added GUI controls for:
  - Pausing/resuming the simulation.
  - Switching between different numerical integrators:
    - Explicit Euler
    - Semi-implicit Euler
    - Leapfrog
    - Runge–Kutta 4
- Implemented relative energy error tracking based on initial total energy.
- Displayed current relative error in the GUI and added plot showing its change over simulation frames.

## [v0.6] - 2025-07-30
### Technical Improvements
- Removed video recording functionality to simplify cross-platform builds.
- Migrated from Makefile to CMake for modern cross-platform build configuration.
- Added platform-specific build instructions for Linux and macOS to the README.

## [v0.5] - 2025-07-28
### Features
- Added optional frame capture functionality for recording simulation frames.
- When `VIDEO` is set to `true`, frames are saved to `video_frames/` and can be converted to `output.mp4` via `make video`.
- This allows exporting simulation runs for sharing.

## [v0.4] - 2025-07-24
### Features
- Completed 3D refactor! (screen edge protection commented out for now).
- Introduced `SimulationState` struct to manage energy data and enable future features.
- Improved collision behavior using real conservation of energy and momentum of perfectly elastic collisions.

## [v0.3] - 2025-07-19
### Enhancements
- Added total energy calculation for future system stability analysis.
- Added energy values to console output.
- Added minimal collision detection (initial version; velocity vector is just reversed).
- General code refactor with function descriptions.
- Renamed main file for clarity.

### Technical Improvements
- Fixed C++ loop to comply with modern standards (`size_t`).
- Created `LICENSE` file.

## [v0.2] - 2025-07-16
### Major Refactor
- Transitioned to using GLM for vector math.
- Implemented basic Newtonian gravity in 2D.
- Enabled scalable multi-planet simulation support.
- Restructured project layout.
- Introduced modular design with separate `.hpp` files.

### Visual & Physics Features
- Created circle struct and circle drawing function.
- Enabled/disabled screen border functionality.

## [v0.1] - 2025-07-16
### Initial Release
- Initialized project with GLFW window.
- Basic 2D simulation setup.
- Applied basic kinematics with fixed acceleration.
- Numerical integration by semi-implicit Euler method.

