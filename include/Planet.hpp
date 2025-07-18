// Planet.hpp

#pragma once

#include <glm/glm.hpp>

struct Planet {
    float radius;
    int res;
    float mass;
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec2 acceleration;

    Planet() = default;
    Planet(float radius, int res, float mass, glm::vec2 position, glm::vec2 velocity, glm::vec2 acceleration)
        : radius(radius), res(res), mass(mass), position(position), velocity(velocity), acceleration(acceleration) {}
};