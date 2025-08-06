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

void GuiRender(SimulationState& simstate, float dt_ms, bool& paused, int& integratorChoice, bool& resetRequested) {
    ImGui::Begin("Simulation HUD");
    ImGui::Text("Frame dt: %.3f ms (%.1f FPS)", dt_ms, dt_ms > 0 ? 1000.0f/dt_ms : 0.0f);
    ImGui::Checkbox("Pause physics", &paused);
    
    if (ImGui::Button("Reset Simulation")) {
        resetRequested = true;
    }

    const char* methods[] = { "Explicit Euler", "Semi-implicit Euler", "Leapfrog", "RK4" };
    static int lastIntegratorChoice = integratorChoice; // remember previous choice

    if (ImGui::Combo("Integrator", &integratorChoice, methods, IM_ARRAYSIZE(methods))) {
        if (integratorChoice != lastIntegratorChoice) {
            simstate.resetEnergyData();
            lastIntegratorChoice = integratorChoice;
        }
    }

    // Plot energy data
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

    // Display Relative Energy Error
    if (simstate.energyError.empty()) {
        ImGui::Text("Relative Energy Error: n/a");
    } else {
        float rel = simstate.energyError.back();
        ImGui::Text("Absolute Relative Energy Error: %.3e", rel);
    }

    // Plot absolute relative error on linear scale with fixed X start and "only grow" Y axis
    if (!simstate.energyError.empty()) {
        static double yMax = 0.0;       // start at 0 for linear scale
        const  double yMin = 0.0;       // fixed lower bound

        // Update yMax if new data exceeds it
        for (float v : simstate.energyError) {
            if (v > yMax) yMax = v;
        }

        if (ImPlot::BeginPlot("Absolute Relative Energy Error (linear scale)", ImVec2(-1, 180))) {
            // X starts at 0, grows with frame count
            double xMax = static_cast<double>(simstate.energyError.size());
            ImPlot::SetupAxisLimits(ImAxis_X1, 0, xMax, ImGuiCond_Always);

            // Y stays fixed at min and only grows up
            ImPlot::SetupAxes("Frame", "|Rel. Error|",
                            ImPlotAxisFlags_None, ImPlotAxisFlags_None);
            ImPlot::SetupAxisScale(ImAxis_Y1, ImPlotScale_Linear);
            ImPlot::SetupAxisLimits(ImAxis_Y1, yMin, yMax, ImGuiCond_Always);

            ImPlot::PlotLine("Abs Rel Error", simstate.energyError.data(),
                            (int)simstate.energyError.size());
            ImPlot::EndPlot();
        }
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
