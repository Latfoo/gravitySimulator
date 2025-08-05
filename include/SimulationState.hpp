#pragma once
#include <vector>

struct SimulationState {
    std::vector<float> energyKinetic;
    std::vector<float> energyPotential;
    std::vector<float> energyTotal;
    std::vector<float> energyError;
    
    float initialTotalEnergy = 0.0f; // reference value for relative energy error
    bool  baselineSet = false; // set once on first sample

    void resetEnergyData() {
        energyKinetic.clear();
        energyPotential.clear();
        energyTotal.clear();
        initialTotalEnergy = 0.0f;
        baselineSet = false;
        energyError.clear();
    }

    void trim(size_t maxSamples = 1000) {
        if (energyTotal.size() > maxSamples) {
            energyKinetic.erase(energyKinetic.begin());
            energyPotential.erase(energyPotential.begin());
            energyTotal.erase(energyTotal.begin());
            energyError.erase(energyError.begin());
        }
    }
};
