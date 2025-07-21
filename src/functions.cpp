// functions.cpp

#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <iomanip> 
#include "Planet.hpp"
#include "functions.hpp"


void drawPlanet(Planet &planet){

    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(planet.position.x, planet.position.y);

    for(size_t i = 0; i < planet.res+1; i++){
            float angle = 2.0f * M_PI * static_cast<float>(i)/planet.res;
            float x = planet.position.x + cos(angle) * planet.radius;
            float y = planet.position.y + sin(angle) * planet.radius;
            glVertex2d(x,y);
    }

    glEnd();
}


// Reverses velocity, if one of the screen edges is hit
void checkEdgeProtection(Planet &planet){
    if (planet.position.x < planet.radius || planet.position.x > SCREEN_WIDTH - planet.radius) planet.velocity.x *= -1;
    if (planet.position.y < planet.radius || planet.position.y > SCREEN_HEIGHT - planet.radius) planet.velocity.y *= -1;
}


// Classical Newtonian Physics 
glm::vec2 computeGravitationalForce(const Planet& a, const Planet& b) {
    glm::vec2 r = b.position - a.position;
    float distanceSquared = glm::dot(r, r); 
    float distance = sqrt(distanceSquared);

    if (distance < 1e-5f) return glm::vec2(0.0f);

    glm::vec2 forceDir = glm::normalize(r);
    float forceMagnitude = (G * a.mass * b.mass) / distanceSquared;
    return forceDir * forceMagnitude;
}


void updateAccelerations(std::vector<Planet> &planets) {
    for (auto& planet : planets) {
        glm::vec2 netForce(0.0f);

        for (const auto& other : planets) {
            if (&planet != &other) {
                netForce += computeGravitationalForce(planet, other);
            }
        }

        planet.acceleration = netForce / planet.mass;
    }
}


bool checkCollision(const Planet& a, const Planet& b) {
    float dist = glm::length(b.position - a.position);
    return dist <= (a.radius + b.radius);
}


void checkAllCollisions(std::vector<Planet>& planets) {
    for (size_t i = 0; i < planets.size(); ++i) {
        for (size_t j = i + 1; j < planets.size(); ++j) {
            if (checkCollision(planets[i], planets[j])) {
                planets[i].velocity *= -1.0f;
                planets[j].velocity *= -1.0f;
            }
        }
    }
}


void calculateMovement(std::vector<Planet> &planets, float deltaTime){

    updateAccelerations(planets); 

    for(auto& planet : planets){

        // Euler(semi-implicit)
        planet.velocity += planet.acceleration * deltaTime;
        planet.position += planet.velocity * deltaTime;

        if(EDGES) checkEdgeProtection(planet);
    }
}

void calculateTotalEnergy(const std::vector<Planet>& planets) {
    float totalKinetic = 0.0f;
    float totalPotential = 0.0f;

    // Kinetic energy T
    for (const auto& planet : planets) {
        totalKinetic += 0.5f * planet.mass * glm::dot(planet.velocity, planet.velocity);
    }

    // Potential energy V
    for (size_t i = 0; i < planets.size(); ++i) {
        for (size_t j = i + 1; j < planets.size(); ++j) {
            glm::vec2 r = planets[j].position - planets[i].position;
            float dist = glm::length(r);
            totalPotential -= G * planets[i].mass * planets[j].mass / dist;
        }
    }

    float totalEnergy = totalKinetic + totalPotential;

    std::cout << std::scientific << std::setprecision(2)
              << "Kinetic: " << totalKinetic
              << " Potential: " << totalPotential
              << " Total: " << totalEnergy << std::endl;
}