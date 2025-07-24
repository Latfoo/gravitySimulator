// gravitySim.cpp

#include <GL/glu.h>
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
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GLFW Window", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0, 10000.0);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(0.0, 0.0, 3000.0, // Camera Position
          0.0, 0.0, 0.0, 
          0.0, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 1);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat lightPos[] = { 2500.0f, 5000.0f, 5000.0f, 0.5f }; //Light Position
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
    // --------------------- Simulation -------------------------

    // Initial Conditions for Planets

    std::vector<Planet> planets = {
    {80.f, 50, 5.972e24f, {0.f, 0.f, 0.f}, {0.f, 0.f, 0.0f}, {0.f, 0.f, 0.f}},
    {20.f, 50, 7.348e22f, {-500.f, 0.f, 0.f}, {0.f, 75.f, 50.f}, {0.f, 0.f, 0.f}},
    {20.f, 50, 7.348e22f, {600.f, 0.f, 0.f}, {0.f, -68.f, -30.f}, {0.f, 0.f, 0.f}},  
    };

    // Rendering Loop

    float lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
;

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
