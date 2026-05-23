#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <string>
#include "Planet.hpp"
#include "constants.hpp"
#include "SimulationState.hpp"


/**
 * @brief Draws a filled circle representing a planet using OpenGL.
 * @param planet The planet object to draw.
 */
void drawPlanet(Planet &planet);

/**
 * @brief Reverses the velocity of the planet if it hits the screen edges.
 * @param planet The planet to check and possibly update velocity.
 */
void checkEdgeProtection(Planet &planet);

/**
 * @brief Calculates the gravitational force exerted on planet a by planet b.
 * @param a The first planet.
 * @param b The second planet.
 * @return The gravitational force vector applied on planet a due to planet b.
 */
glm::vec3 computeGravitationalForce(const Planet& a, const Planet& b);  // ← changed from vec2

/**
 * @brief Updates accelerations of all planets based on gravitational forces.
 * @param planets Vector of planets whose accelerations will be updated.
 */
void updateAccelerations(std::vector<Planet> &planets);

/**
 * @brief Checks if two planets are colliding based on their positions and radii.
 * @param a The first planet.
 * @param b The second planet.
 * @return True if planets collide, false otherwise.
 */
bool checkCollision(const Planet& a, const Planet& b);

/**
 * @brief Checks for collisions between all pairs of planets and reverses their velocities if they collide.
 * @param planets Vector of planets to check collisions for.
 */
void checkAllCollisions(std::vector<Planet>& planets);

/**
 * @brief Calculates new positions and velocities of planets based on accelerations and time elapsed.
 * @param planets Vector of planets to update.
 * @param deltaTime Time elapsed since last update.
 * @param integratorChoice Choice of numerical integrator: 0 = Semi-Implicit Euler, 1 = Leapfrog, 2 = Runge-Kutta 4.
 */
void calculateMovement(std::vector<Planet>& planets, float deltaTime, int integratorChoice);

/**
 * @brief Returns the total mechanical energy (kinetic + potential) of the system.
 */
float totalEnergy(const std::vector<Planet>& planets);

/**
 * @brief Calculates and logs the system's total kinetic, potential, and total energy.
 * @param planets   Vector of Planet objects representing the current simulation state.
 * @param simstate  Reference to a SimulationState object where energy values will be stored.
 */
void calculateTotalEnergy(const std::vector<Planet>& planets, SimulationState& simstate);