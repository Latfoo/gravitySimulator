# pragma once
#include "Planet.hpp"

/**
 * @brief Advances the simulation using the explicit (forward) Euler method.
 *
 * @param planets   Vector of planets to update.
 * @param deltaTime Time step in seconds.
 */
void explicitEuler(std::vector<Planet> &planets, float deltaTime); 

/**
 * @brief Advances the simulation using the semi-implicit Euler method.
 *
 * @param planets   Vector of planets to update.
 * @param deltaTime Time step in seconds.
 */
void semiImplicitEuler(std::vector<Planet> &planets, float deltaTime);

/**
 * @brief Advances the simulation using the classical 4th-order Runge–Kutta (RK4) method.
 *
 * @param planets   Vector of planets to update.
 * @param deltaTime Time step in seconds.
 */
void rungeKutta4(std::vector<Planet> &planets, float deltaTime);

/**
 * @brief Advances the simulation using the Leapfrog (Velocity Verlet) method.
 *
 * @param planets   Vector of planets to update.
 * @param deltaTime Time step in seconds.
 */
void leapFrog(std::vector<Planet> &planets, float deltaTime);
