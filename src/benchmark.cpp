#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include "Planet.hpp"
#include "SceneLoader.hpp"
#include "functions.hpp"
#include "constants.hpp"

std::vector<Planet> defaultScene() {
    return {
        {80.f, 50, 5.972e24f, {0.f, 0.f, 0.f},          {0.f, 0.f, 0.0f},    {0.f, 0.f, 0.f}},
        {20.f, 50, 7.348e22f, {-500.f, 0.f, 0.f},        {0.f, 75.f, 50.f},   {0.f, 0.f, 0.f}},
        {20.f, 50, 7.348e22f, {600.f, 0.f, 0.f},         {0.f, -68.f, -30.f}, {0.f, 0.f, 0.f}},
        {10.f, 50, 1e22f,     {0.f, 500.f, 0.f},         {0.f, -60.f, 0.f},   {0.f, 0.f, 0.f}},
        {15.f, 50, 2e22f,     {800.f, -400.f, 200.f},    {-70.f, 55.f, -10.f},{0.f, 0.f, 0.f}},
        {25.f, 50, 1e23f,     {1500.f, 1500.f, -1500.f}, {0.f, 0.f, 0.f},     {0.f, 0.f, 0.f}},
        {5.f,  50, 1e21f,     {-300.f, -300.f, -300.f},  {45.f, 45.f, 45.f},  {0.f, 0.f, 0.f}},
        {5.f,  50, 1e21f,     {300.f, 300.f, 300.f},     {-45.f, -45.f, -45.f},{0.f, 0.f, 0.f}},
    };
}

float totalEnergy(const std::vector<Planet>& planets) {
    float kinetic = 0.0f, potential = 0.0f;
    for (const Planet& p : planets)
        kinetic += 0.5f * p.mass * glm::dot(p.velocity, p.velocity);
    for (size_t i = 0; i < planets.size(); ++i)
        for (size_t j = i + 1; j < planets.size(); ++j) {
            float d = glm::length(planets[j].position - planets[i].position);
            potential -= G * planets[i].mass * planets[j].mass / d;
        }
    return kinetic + potential;
}

int main() {
    const int   N_STEPS = 2500;
    const float DT      = 1.0f / 60.0f;

    std::vector<Planet> initial = loadScene("scene.json");
    if (initial.empty()) {
        std::cout << "Using default hardcoded scene.\n";
        initial = defaultScene();
    }

    const char* names[] = { "explicit_euler", "semi_implicit_euler", "leapfrog", "rk4" };
    std::array<std::vector<float>, 4> errors;

    for (int integ = 0; integ < 4; ++integ) {
        std::vector<Planet> planets = initial;
        float E0 = totalEnergy(planets);

        std::cout << "Running " << names[integ] << "...\n";

        for (int step = 0; step < N_STEPS; ++step) {
            calculateMovement(planets, DT, integ);
            float err = (E0 != 0.0f) ? std::abs((totalEnergy(planets) - E0) / E0) : 0.0f;
            errors[integ].push_back(err);
        }
    }

    std::ofstream csv("docs/energy_errors.csv");
    csv << "time_s";
    for (const char* name : names) csv << "," << name;
    csv << "\n";

    for (int s = 0; s < N_STEPS; ++s) {
        csv << (s + 1) * DT;
        for (int i = 0; i < 4; ++i) csv << "," << errors[i][s];
        csv << "\n";
    }

    std::cout << "Written docs/energy_errors.csv (" << N_STEPS << " steps, dt=" << DT << ")\n";
    return 0;
}
