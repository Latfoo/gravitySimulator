// gravitySim.cpp

#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "Planet.hpp"
#include "functions.hpp"
#include "constants.hpp"
#include "SimulationState.hpp"
#include "Gui.hpp"
#include "camera_controls.hpp"


int main() {

    // ==================== GLFW and OpenGL Setup ========================

    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GLFW Window", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    CameraControls camera;
    camera.installCallbacks(window);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0, 10000.0);
    glMatrixMode(GL_MODELVIEW);
    
    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 1);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat lightPos[] = { 2500.0f, 5000.0f, 5000.0f, 0.5f }; // Initial Light Position
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Gui Initialization
    GuiInit(window);
    
    // ======================= Simulation ==========================

    // ------------Initial Conditions ----------------

    std::vector<Planet> planets = {
    // stationary Planet at the origin
    {80.f, 50, 5.972e24f, {0.f, 0.f, 0.f}, {0.f, 0.f, 0.0f}, {0.f, 0.f, 0.f}},

    // Moon-like object approaching from the left
    {20.f, 50, 7.348e22f, {-500.f, 0.f, 0.f}, {0.f, 75.f, 50.f}, {0.f, 0.f, 0.f}},

    // Another moon-like object coming from the right
    {20.f, 50, 7.348e22f, {600.f, 0.f, 0.f}, {0.f, -68.f, -30.f}, {0.f, 0.f, 0.f}},  

    // Small planet falling straight down toward Earth
    {10.f, 50, 1e22f, {0.f, 500.f, 0.f}, {0.f, -60.f, 0.f}, {0.f, 0.f, 0.f}},

    // Diagonal moving body from the lower right
    {15.f, 50, 2e22f, {800.f, -400.f, 200.f}, {-70.f, 55.f, -10.f}, {0.f, 0.f, 0.f}},

    // Distant massive body - stationary and far from center
    {25.f, 50, 1e23f, {1500.f, 1500.f, -1500.f}, {0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}},

    // Two small bodies heading directly toward each other through the center
    {5.f, 50, 1e21f, {-300.f, -300.f, -300.f}, {45.f, 45.f, 45.f}, {0.f, 0.f, 0.f}},
    {5.f, 50, 1e21f, {300.f, 300.f, 300.f}, {-45.f, -45.f, -45.f}, {0.f, 0.f, 0.f}},
    };

    std::vector<Planet> initialPlanets = planets; // backup for reset functionality

    SimulationState simstate;

    float lastTime = glfwGetTime();
    int frame_count = 0;
    bool paused = false;
    int integratorChoice = 1; // default to Leapfrog
    bool resetRequested = false;
    bool cameraResetRequested = false;

    
    // ---------------- Rendering Loop -------------------

    while (!glfwWindowShouldClose(window)) {
        // Time Update
        float now = glfwGetTime();
        float deltaTime = now - lastTime;
        lastTime = now;

        // Clear Buffers and apply Camera View
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        camera.applyView();

        // Lightning
        GLfloat lightPos[] = { 2500.0f, 5000.0f, 5000.0f, 1.0f };
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

        // Pause Simulation
        if (!paused) {
            calculateMovement(planets, deltaTime, integratorChoice);
            checkAllCollisions(planets);
            calculateTotalEnergy(planets, simstate);
        }

        // Rendering Planets
        for (auto& planet : planets){
            drawPlanet(planet);
        }

        // GUI
        GuiNewFrame();
        GuiRender(simstate, deltaTime * 1000.0f, paused, integratorChoice, resetRequested, cameraResetRequested);

        if (resetRequested) {
            planets = initialPlanets;
            simstate.resetEnergyData();
            lastTime = glfwGetTime();
            paused = false;
            resetRequested = false;
        }

        if (cameraResetRequested) {
            camera.reset();
            cameraResetRequested = false;
        }

        // Swap Buffers and Poll Events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }



    // --------------------- Terminate Window -------------------------

    glfwDestroyWindow(window);
    GuiShutdown();
    glfwTerminate();
    return 0;
}
