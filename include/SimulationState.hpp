#pragma once
#include <array>
#include <vector>

struct IntegratorErrorLog {
    std::vector<float> errors;
    float initialEnergy = 0.0f;
    bool baselineSet = false;

    void clear() {
        errors.clear();
        initialEnergy = 0.0f;
        baselineSet = false;
    }
};

struct SimulationState {
    // Live energy tracking for the current integrator (trimmed rolling window)
    std::vector<float> energyKinetic;
    std::vector<float> energyPotential;
    std::vector<float> energyTotal;
    std::vector<float> energyError;
    float initialTotalEnergy = 0.0f;
    bool  baselineSet = false;

    // Per-integrator accumulated error for log-log comparison
    std::array<IntegratorErrorLog, 4> integratorLogs;

    void resetEnergyData() {
        energyKinetic.clear();
        energyPotential.clear();
        energyTotal.clear();
        energyError.clear();
        initialTotalEnergy = 0.0f;
        baselineSet = false;
        for (auto& log : integratorLogs) log.clear();
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
