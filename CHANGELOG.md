# Changelog

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

