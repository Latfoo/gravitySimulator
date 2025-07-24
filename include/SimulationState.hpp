#pragma once
#include <vector>

struct SimulationState {
    std::vector<float> energyKinetic;
    std::vector<float> energyPotential;
    std::vector<float> energyTotal;

    void resetEnergyData() {
        energyKinetic.clear();
        energyPotential.clear();
        energyTotal.clear();
    }

    void trim(size_t maxSamples = 1000) {
        if (energyTotal.size() > maxSamples) {
            energyKinetic.erase(energyKinetic.begin());
            energyPotential.erase(energyPotential.begin());
            energyTotal.erase(energyTotal.begin());
        }
    }
};
