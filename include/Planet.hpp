#pragma once

#include <glm/glm.hpp>

struct Planet {
    float radius;
    int res;
    float mass;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;

    Planet() = default;
    Planet(float radius, int res, float mass, glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration)
        : radius(radius), res(res), mass(mass), position(position), velocity(velocity), acceleration(acceleration) {}
};
