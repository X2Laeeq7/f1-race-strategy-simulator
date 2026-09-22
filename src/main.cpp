#include <iostream>
#include <string>
#include <vector>

struct TyreCompound{
    std::string name;
    double paceOffset;
    double linearDeg;
    double cliffLap;
    double cliffSteepness;
    double cliffMagnitude;
};

struct Stint {
    int compoundIndex;
    int laps;
};

struct RaceConfig {
    double trackLengthKm;
    int totalLaps;
    double baseLapTime;
    double fuelStartKg;
    double fuelBurnPerLapKg;
    double fuelPenaltyPerKg;
    double pitLossSeconds;
    double randomSigma;
    std::vector<TyreCompound> compounds;
};

struct LapResult {
    int lapNumber;
    std::string compoundName;
    int tyreAge;
    double fuelKg;
    bool box;
    double lapTime;
};

struct RaceResult{
    std::vector<LapResult> laps;
    double totalTime;
    int pitStops;
    std::string strategyString;
};

int main(){
    TyreCompound soft{"Soft",-0.5,0.03,18.0,0.8,1.5};
    TyreCompound medium{"Medium",0.0,0.02,25.0,0.6,1.0};
    TyreCompound hard{"Hard",0.6,0.01,35.0,0.5,0.8};
    RaceConfig config;
    config.trackLengthKm = 5.0;
    config.totalLaps = 50;
    config.baseLapTime = 82.0;
    config.fuelStartKg = 100.0;
    config.fuelBurnPerLapKg = 1.8;
    config.fuelPenaltyPerKg = 0.03;
    config.pitLossSeconds = 23.0;
    config.randomSigma = 0.15;

    config.compounds.push_back(soft);
    config.compounds.push_back(medium);
    config.compounds.push_back(hard);
    
    for (const auto& c : config.compounds) {
        std::cout << c.name << " cliffLap=" << c.cliffLap <<std::endl;
    }
    return 0;
}