#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl2.h"
#include "implot/implot.h"
#include "SimulationState.hpp"

void GuiInit(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();
}

void GuiNewFrame() {
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GuiRender(SimulationState& simstate, float dt_ms, bool& paused) {
    ImGui::Begin("Simulation HUD");
    ImGui::Text("Frame dt: %.3f ms (%.1f FPS)", dt_ms, dt_ms > 0 ? 1000.0f/dt_ms : 0.0f);
    ImGui::Checkbox("Pause physics", &paused);

    if (ImPlot::BeginPlot("Energy", ImVec2(-1, 250))) {
        ImPlot::SetupAxes("Frames", "J", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
        if (!simstate.energyKinetic.empty())
            ImPlot::PlotLine("Kinetic", simstate.energyKinetic.data(), (int)simstate.energyKinetic.size());
        if (!simstate.energyPotential.empty())
            ImPlot::PlotLine("Potential", simstate.energyPotential.data(), (int)simstate.energyPotential.size());
        if (!simstate.energyTotal.empty())
            ImPlot::PlotLine("Total", simstate.energyTotal.data(), (int)simstate.energyTotal.size());
        ImPlot::EndPlot();
    }
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

void GuiShutdown() {
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
}
