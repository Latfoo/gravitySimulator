// gravitySim.cpp

#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "Planet.hpp"
#include "functions.hpp"
#include "constants.hpp"

int main() {

    // --------------------- GLFW and OpenGL Setup -------------------------

    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GLFW Window", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, -1, 1);  // orthographic projection on the screen
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0, 0, 0, 1);

    
    // --------------------- Simulation -------------------------

    // Initial Conditions for Planets

    std::vector<Planet> planets = {
        {50.f, 100, 5.972e24f, {SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f}, {0.f, 0.f}, {0.f, 0.f}},
        {20.f, 100, 7.348e22f, {SCREEN_WIDTH / 4.f, SCREEN_HEIGHT / 1.5f}, {50.f, 70.f}, {0.f, 0.f}},
        {20.f, 100, 7.348e22f, {SCREEN_WIDTH / 1.1f, SCREEN_HEIGHT / 1.2f}, {-10.f, -30.f}, {0.f, 0.f}}
    };

    // Rendering Loop

    float lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        calculateMovement(planets, deltaTime);
        checkAllCollisions(planets);
        calculateTotalEnergy(planets);
        for(auto& planet : planets){
            drawPlanet(planet);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // --------------------- Terminate Window -------------------------

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
