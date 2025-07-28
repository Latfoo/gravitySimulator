#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include <fstream>
#include "Planet.hpp"
#include "functions.hpp"
#include "SimulationState.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


void drawPlanet(Planet &planet) {
    glPushMatrix();
    glTranslated(planet.position.x, planet.position.y, planet.position.z);

    const int layers = planet.res;
    const int slices = planet.res;
    const float radius = planet.radius;

    for (int i = 0; i < layers; ++i) {
        float phi1 = M_PI * i / layers;
        float phi2 = M_PI * (i + 1) / layers;

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; ++j) {
            float theta = 2.0f * M_PI * j / slices;

            float x1 = radius * sin(phi1) * cos(theta);
            float y1 = radius * cos(phi1);
            float z1 = radius * sin(phi1) * sin(theta);

            float x2 = radius * sin(phi2) * cos(theta);
            float y2 = radius * cos(phi2);
            float z2 = radius * sin(phi2) * sin(theta);

            glNormal3f(x1 / radius, y1 / radius, z1 / radius);
            glVertex3f(x1, y1, z1);

            glNormal3f(x2 / radius, y2 / radius, z2 / radius);
            glVertex3f(x2, y2, z2);
        }
        glEnd();
    }

    glPopMatrix();
}



// Reverses velocity, if one of the screen edges is hit (currently non-functional in 3D)
/*void checkEdgeProtection(Planet &planet){
    if (planet.position.x < planet.radius || planet.position.x > SCREEN_WIDTH - planet.radius) planet.velocity.x *= -1;
    if (planet.position.y < planet.radius || planet.position.y > SCREEN_HEIGHT - planet.radius) planet.velocity.y *= -1;
    if (planet.position.z < -500.0 || planet.position.z > 500.0) planet.velocity.z *= -1;
}*/


// Classical Newtonian Physics 
glm::vec3 computeGravitationalForce(const Planet& a, const Planet& b) {
    glm::vec3 r = b.position - a.position;
    float distanceSquared = glm::dot(r, r); 
    float distance = sqrt(distanceSquared);

    if (distance < 1e-5f) return glm::vec3(0.0f);

    glm::vec3 forceDir = glm::normalize(r);
    float forceMagnitude = (G * a.mass * b.mass) / distanceSquared;
    return forceDir * forceMagnitude;
}


void updateAccelerations(std::vector<Planet> &planets) {
    for (auto& planet : planets) {
        glm::vec3 netForce(0.0f);

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

                Planet& a = planets[i];
                Planet& b = planets[j];

                glm::vec3 n = glm::normalize(a.position - b.position); // Normalized collision vector
                glm::vec3 v_rel = a.velocity - b.velocity;
                float v_rel_n = glm::dot(v_rel, n); // Projection of rel. velocity on the normal vector

                if (v_rel_n >= 0) continue;

                // Perfectly elastic collision: exchange of momentum along normal vector
                glm::vec3 impulseVec = (2.0f * glm::dot(a.velocity - b.velocity, n) / (a.mass + b.mass)) * n;

                a.velocity -= b.mass * impulseVec;
                b.velocity += a.mass * impulseVec;
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

void calculateTotalEnergy(const std::vector<Planet>& planets, SimulationState &simstate) {
    float totalKinetic = 0.0f;
    float totalPotential = 0.0f;

    // Kinetic energy T
    for (const auto& planet : planets) {
        totalKinetic += 0.5f * planet.mass * glm::dot(planet.velocity, planet.velocity);
    }

    // Potential energy V
    for (size_t i = 0; i < planets.size(); ++i) {
        for (size_t j = i + 1; j < planets.size(); ++j) {
            glm::vec3 r = planets[j].position - planets[i].position;
            float dist = glm::length(r);
            totalPotential -= G * planets[i].mass * planets[j].mass / dist;
        }
    }

    float totalEnergy = totalKinetic + totalPotential;

    // Store Data for Graphical Interface
    simstate.energyKinetic.push_back(totalKinetic);
    simstate.energyPotential.push_back(totalPotential);
    simstate.energyTotal.push_back(totalEnergy);
    simstate.trim(); // trims buffer vectors if they get too large

    // Console output
    std::cout << std::scientific << std::setprecision(2)
              << "Kinetic: " << totalKinetic
              << " Potential: " << totalPotential
              << " Total: " << totalEnergy << std::endl;
}


void capture_frame(int frame_number, const std::string& folder, const std::string& prefix) {
    using namespace std;

    std::filesystem::create_directories(folder);

    int width, height;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);

    std::vector<unsigned char> pixels(width * height * 3);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

    // Flip image vertically for correct orientation
    for (int y = 0; y < height / 2; ++y) {
        int top = y * width * 3;
        int bottom = (height - 1 - y) * width * 3;
        for (int x = 0; x < width * 3; ++x) {
            std::swap(pixels[top + x], pixels[bottom + x]);
        }
    }

    std::ostringstream filename;
    filename << folder << "/" << prefix << "_" << std::setw(4) << std::setfill('0') << frame_number << ".ppm";

    std::ofstream out(filename.str(), std::ios::binary);
    if (!out) {
        std::cerr << "Failed to open file: " << filename.str() << std::endl;
        return;
    }

    out << "P6\n" << width << " " << height << "\n255\n";
    out.write(reinterpret_cast<char*>(pixels.data()), pixels.size());
}