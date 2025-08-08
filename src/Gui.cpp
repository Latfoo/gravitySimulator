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

void GuiRender(SimulationState& simstate, float dt_ms, bool& paused, int& integratorChoice, bool& resetRequested, bool& cameraResetRequested) {
    ImGui::Begin("Simulation HUD");
    ImGui::Text("Frame dt: %.3f ms (%.1f FPS)", dt_ms, dt_ms > 0 ? 1000.0f/dt_ms : 0.0f);
    ImGui::Checkbox("Pause physics", &paused);
    
    if (ImGui::Button("Reset Simulation")) {
        resetRequested = true;
    }
    if (ImGui::Button("Reset Camera")) {
        cameraResetRequested = true;
    }

    const char* methods[] = { "Explicit Euler", "Semi-implicit Euler", "Leapfrog", "RK4" };
    static int lastIntegratorChoice = integratorChoice; // remember previous choice

    if (ImGui::Combo("Integrator", &integratorChoice, methods, IM_ARRAYSIZE(methods))) {
        if (integratorChoice != lastIntegratorChoice) {
            simstate.resetEnergyData();
            lastIntegratorChoice = integratorChoice;
        }
    }

    ImGui::Separator();

    // ENERGY COLLAPSER
    if (ImGui::CollapsingHeader("Energy Tracking")) {

        // Plot energy data
        if (ImPlot::BeginPlot("Energy", ImVec2(-1, 250))) {
            ImPlot::SetupAxes("Frame", "J", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
            if (!simstate.energyKinetic.empty())
                ImPlot::PlotLine("Kinetic", simstate.energyKinetic.data(), (int)simstate.energyKinetic.size());
            if (!simstate.energyPotential.empty())
                ImPlot::PlotLine("Potential", simstate.energyPotential.data(), (int)simstate.energyPotential.size());
            if (!simstate.energyTotal.empty())
                ImPlot::PlotLine("Total", simstate.energyTotal.data(), (int)simstate.energyTotal.size());
            ImPlot::EndPlot();
        }

        ImGui::Separator();

        // Display Relative Energy Error
        if (simstate.energyError.empty()) {
            ImGui::Text("Relative Energy Error: n/a");
        } else {
            float rel = simstate.energyError.back();
            ImGui::Text("Relative Energy Error (Magnitude): %.3e", rel);
        }

        // Plot absolute relative error on linear scale with fixed X start and "only grow" Y axis
        if (!simstate.energyError.empty()) {
            static double yMax = 0.0;       // start at 0 for linear scale
            const  double yMin = 0.0;       // fixed lower bound

            // Update yMax if new data exceeds it
            for (float v : simstate.energyError) {
                if (v > yMax) yMax = v;
            }

            if (ImPlot::BeginPlot("Linear scale", ImVec2(-1, 180))) {
                // X starts at 0, grows with frame count
                double xMax = static_cast<double>(simstate.energyError.size());
                ImPlot::SetupAxisLimits(ImAxis_X1, 0, xMax, ImGuiCond_Always);

                // Y stays fixed at min and only grows up
                ImPlot::SetupAxes("Frame", "|Rel. Error|",
                                ImPlotAxisFlags_None, ImPlotAxisFlags_None);
                ImPlot::SetupAxisScale(ImAxis_Y1, ImPlotScale_Linear);
                ImPlot::SetupAxisLimits(ImAxis_Y1, yMin, yMax, ImGuiCond_Always);

                ImPlot::PlotLine("Rel. Error", simstate.energyError.data(),
                                (int)simstate.energyError.size());
                ImPlot::EndPlot();
            }
        }

        // log scale
        if (!simstate.energyError.empty()) {
            static double yMaxLog = 0.0;
            const  double yMinLog = 1e-12;   // must be > 0 for log scale

            for (float v : simstate.energyError) {
                if (v > yMaxLog) yMaxLog = v;
            }

            // Ensure yMaxLog is reasonable
            if (yMaxLog < yMinLog) yMaxLog = yMinLog * 10.0;

            if (ImPlot::BeginPlot("Log scale", ImVec2(-1, 180))) {
                double xMax = static_cast<double>(simstate.energyError.size());
                ImPlot::SetupAxisLimits(ImAxis_X1, 0, xMax, ImGuiCond_Always);

                ImPlot::SetupAxes("Frame", "|Rel. Error|",
                                ImPlotAxisFlags_None, ImPlotAxisFlags_None);
                ImPlot::SetupAxisScale(ImAxis_Y1, ImPlotScale_Log10);
                ImPlot::SetupAxisLimits(ImAxis_Y1, yMinLog, yMaxLog, ImGuiCond_Always);

                ImPlot::PlotLine("Rel. Error", simstate.energyError.data(),
                                (int)simstate.energyError.size(), ImPlotItemFlags_NoLegend);
                ImPlot::EndPlot();
            }
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
