#pragma once
#include <GLFW/glfw3.h>
struct SimulationState;

void GuiInit(GLFWwindow* window);
void GuiNewFrame();
void GuiRender(SimulationState& simstate, float dt_ms, bool& paused);
void GuiShutdown();
