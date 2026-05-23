#include <math.h>
#include <vector>
#include <glm/glm.hpp>
#include "Planet.hpp"
#include "physics.hpp"
#include "SimulationState.hpp"
#include "NumericalIntegrators.hpp"


glm::vec3 computeGravitationalForce(const Planet& a, const Planet& b) {
    glm::vec3 r = b.position - a.position;
    float distanceSquared = glm::dot(r, r);
    float distance = sqrt(distanceSquared);

    if (distance < 1e-5f) return glm::vec3(0.0f);

    glm::vec3 forceDir = glm::normalize(r);
    float forceMagnitude = (G * a.mass * b.mass) / distanceSquared;
    return forceDir * forceMagnitude;
}


void updateAccelerations(std::vector<Planet>& planets) {
    const size_t n = planets.size();

    for (size_t i = 0; i < n; ++i)
        planets[i].acceleration = glm::vec3(0.0f);

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            glm::vec3 force = computeGravitationalForce(planets[i], planets[j]);
            planets[i].acceleration += force / planets[i].mass;
            planets[j].acceleration -= force / planets[j].mass;
        }
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

                glm::vec3 n = glm::normalize(a.position - b.position);
                glm::vec3 v_rel = a.velocity - b.velocity;
                float v_rel_n = glm::dot(v_rel, n);

                if (v_rel_n >= 0) continue;

                glm::vec3 impulseVec = (2.0f * glm::dot(a.velocity - b.velocity, n) / (a.mass + b.mass)) * n;
                a.velocity -= b.mass * impulseVec;
                b.velocity += a.mass * impulseVec;
            }
        }
    }
}


void calculateMovement(std::vector<Planet>& planets, float deltaTime, int integratorChoice) {
    switch (integratorChoice) {
        case 0: explicitEuler(planets, deltaTime);      break;
        case 1: semiImplicitEuler(planets, deltaTime);  break;
        case 2: leapFrog(planets, deltaTime);           break;
        case 3: rungeKutta4(planets, deltaTime);        break;
        default: semiImplicitEuler(planets, deltaTime); break;
    }
}


float totalEnergy(const std::vector<Planet>& planets) {
    float kinetic = 0.0f, potential = 0.0f;
    for (const auto& p : planets)
        kinetic += 0.5f * p.mass * glm::dot(p.velocity, p.velocity);
    for (size_t i = 0; i < planets.size(); ++i)
        for (size_t j = i + 1; j < planets.size(); ++j) {
            float d = glm::length(planets[j].position - planets[i].position);
            potential -= G * planets[i].mass * planets[j].mass / d;
        }
    return kinetic + potential;
}

void calculateTotalEnergy(const std::vector<Planet>& planets, SimulationState& simstate) {
    float totalKinetic = 0.0f;
    for (const auto& planet : planets)
        totalKinetic += 0.5f * planet.mass * glm::dot(planet.velocity, planet.velocity);

    float energy = totalEnergy(planets);
    float totalPotential = energy - totalKinetic;

    if (!simstate.baselineSet) {
        simstate.initialTotalEnergy = energy;
        simstate.baselineSet = true;
    }
    float relError = 0.0f;
    if (simstate.initialTotalEnergy != 0.0f)
        relError = abs((energy - simstate.initialTotalEnergy) / simstate.initialTotalEnergy);

    simstate.energyKinetic.push_back(totalKinetic);
    simstate.energyPotential.push_back(totalPotential);
    simstate.energyTotal.push_back(energy);
    simstate.energyError.push_back(relError);
    simstate.trim();
}
