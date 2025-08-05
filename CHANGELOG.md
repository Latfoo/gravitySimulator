# Changelog

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
- Introduced `SimulationState` struct to manage energy data and enable future features.
- Improved collision behavior using real conservation of energy and momentum of perfectly elastic collisions.
- Completed 3D refactor (screen edge protection commented out).

## [v0.3] - 2025-07-19
### Enhancements
- Added total energy calculation for system stability analysis.
- Added energy values to console output.
- Added minimal collision detection (initial version; velocity vector is just reversed).
- General code refactor with function descriptions.
- Renamed main file for clarity.

### Technical Improvements
- Removed `.vscode` folder from version control.
- Fixed C++ loop to comply with modern standards (`size_t`).
- Created `LICENSE` file.

## [v0.2] - 2025-07-16
### Major Refactor
- Transitioned to using GLM for vector math.
- Implemented basic Newtonian gravity.
- Enabled scalable multi-planet simulation support.
- Restructured project layout.
- Introduced modular design with separate `.hpp` files.

### Visual & Physics Features
- Created circle struct and drawing function.
- Enabled/disabled screen border functionality.
- Applied basic kinematics with fixed acceleration.

## [v0.1] - 2025-07-16
### Initial Release
- Initialized project with GLFW window.
- Basic 2D simulation setup.
- Numerical integration by semi-implicit Euler method.

