#include "NumericalIntegrators.hpp"
#include "Planet.hpp"
#include "physics.hpp"

void explicitEuler(std::vector<Planet> &planets, float deltaTime) {
    updateAccelerations(planets);

    for (auto& planet : planets) {

        planet.position += planet.velocity * deltaTime;
        planet.velocity += planet.acceleration * deltaTime;

        if (EDGES) checkEdgeProtection(planet);
    }
}


void semiImplicitEuler(std::vector<Planet> &planets, float deltaTime){

    updateAccelerations(planets); 

    for(auto& planet : planets){

        planet.velocity += planet.acceleration * deltaTime;
        planet.position += planet.velocity * deltaTime;

        if(EDGES) checkEdgeProtection(planet);
    }
}


void rungeKutta4(std::vector<Planet>& p, float dt) {
    const size_t n = p.size();

    // k* for position (x) and velocity (v)
    std::vector<glm::vec3> k1x(n), k1v(n), k2x(n), k2v(n), k3x(n), k3v(n), k4x(n), k4v(n);

    // Stage 1 at current state
    updateAccelerations(p);
    for (size_t i = 0; i < n; ++i) {
        k1x[i] = p[i].velocity;
        k1v[i] = p[i].acceleration;
    }

    // Stage 2 at y + dt/2 * k1
    auto tmp = p;
    for (size_t i = 0; i < n; ++i) {
        tmp[i].position = p[i].position + 0.5f * dt * k1x[i];
        tmp[i].velocity = p[i].velocity + 0.5f * dt * k1v[i];
    }
    updateAccelerations(tmp);
    for (size_t i = 0; i < n; ++i) {
        k2x[i] = tmp[i].velocity;
        k2v[i] = tmp[i].acceleration;
    }

    // Stage 3 at y + dt/2 * k2
    tmp = p;
    for (size_t i = 0; i < n; ++i) {
        tmp[i].position = p[i].position + 0.5f * dt * k2x[i];
        tmp[i].velocity = p[i].velocity + 0.5f * dt * k2v[i];
    }
    updateAccelerations(tmp);
    for (size_t i = 0; i < n; ++i) {
        k3x[i] = tmp[i].velocity;
        k3v[i] = tmp[i].acceleration;
    }

    // Stage 4 at y + dt * k3
    tmp = p;
    for (size_t i = 0; i < n; ++i) {
        tmp[i].position = p[i].position + dt * k3x[i];
        tmp[i].velocity = p[i].velocity + dt * k3v[i];
    }
    updateAccelerations(tmp);
    for (size_t i = 0; i < n; ++i) {
        k4x[i] = tmp[i].velocity;
        k4v[i] = tmp[i].acceleration;
    }

    // Combine
    for (size_t i = 0; i < n; ++i) {
        p[i].position += (dt / 6.0f) * (k1x[i] + 2.0f*k2x[i] + 2.0f*k3x[i] + k4x[i]);
        p[i].velocity += (dt / 6.0f) * (k1v[i] + 2.0f*k2v[i] + 2.0f*k3v[i] + k4v[i]);
        if (EDGES) checkEdgeProtection(p[i]);
    }
}


void leapFrog(std::vector<Planet>& planets, float dt) {
    // a(x_t)
    updateAccelerations(planets);

    // half-kick
    for (auto& b : planets)
        b.velocity += 0.5f * b.acceleration * dt;

    // drift
    for (auto& b : planets) {
        b.position += b.velocity * dt;
        if (EDGES) checkEdgeProtection(b);
    }

    // a(x_{t+dt})
    updateAccelerations(planets);

    // half-kick
    for (auto& b : planets)
        b.velocity += 0.5f * b.acceleration * dt;
}
