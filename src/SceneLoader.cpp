#include "SceneLoader.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

std::vector<Planet> loadScene(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "[SceneLoader] Could not open \"" << path << "\"\n";
        return {};
    }

    nlohmann::json j;
    try {
        file >> j;
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "[SceneLoader] Parse error: " << e.what() << "\n";
        return {};
    }

    std::vector<Planet> planets;

    for (const auto& p : j.at("planets")) {
        Planet planet;
        planet.radius = p.at("radius").get<float>();
        planet.res    = p.at("res").get<int>();
        planet.mass   = p.at("mass").get<float>();

        auto pos = p.at("position");
        planet.position = {pos[0].get<float>(), pos[1].get<float>(), pos[2].get<float>()};

        auto vel = p.at("velocity");
        planet.velocity = {vel[0].get<float>(), vel[1].get<float>(), vel[2].get<float>()};

        planet.acceleration = glm::vec3(0.0f);

        planets.push_back(planet);
    }

    std::cout << "[SceneLoader] Loaded " << planets.size() << " bodies from \"" << path << "\"\n";
    return planets;
}
