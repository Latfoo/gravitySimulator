#pragma once
#include <GLFW/glfw3.h>
struct SimulationState;

/**
 * @brief Initialize the ImGui and ImPlot contexts.
 * 
 * @param window Pointer to the GLFW window where the GUI will be rendered.
 */
void GuiInit(GLFWwindow* window);

/**
 * @brief Start a new ImGui frame.
 */
void GuiNewFrame();

/**
 * @brief Build and render the GUI for the current frame.
 * 
 * @param simstate Reference to the SimulationState.
 * @param dt_ms    Delta time for the last frame in milliseconds.
 * @param paused   Reference to the simulation pause boolean, allowing toggling physics on and off.
 * @param integratorChoice Reference to the integer representing the selected numerical integrator.
 * @param resetRequested Reference to a boolean that indicates if a reset has been requestedv via the GUI.
 */
void GuiRender(SimulationState& simstate, float dt_ms, bool& paused, int& integratorChoice, bool& resetRequested);

/**
 * @brief Clean up ImGui and ImPlot contexts.
 */
void GuiShutdown();
