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

int main(){return 0;}