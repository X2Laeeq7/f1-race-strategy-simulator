#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <random>


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
    double fuelKg;
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

double tyreTimeLoss(const TyreCompound& c, int tyreAge){
    double x = c.cliffSteepness * (tyreAge - c.cliffLap);
    double sigmoid_x = 1.0/(1.0 + std::exp(-x));
    return c.linearDeg * tyreAge + c.cliffMagnitude * sigmoid_x;
};

double computeLapTime(
    const TyreCompound& c,
    int tyreAge,
    double fuelKg, 
    bool box,
    double pitLossSeconds,
    double baseLapTime,
    double fuelPenaltyPerKg,
    std::mt19937& rng,
    std::normal_distribution<double>& noise
){
    double tyreLoss = tyreTimeLoss(c,tyreAge);
    double fuelLoss = fuelKg * fuelPenaltyPerKg;
    double pitLoss = box ? pitLossSeconds : 0.0;
    double n = noise(rng);
    return baseLapTime + c.paceOffset + tyreLoss + fuelLoss + pitLoss + n;
};

int main(){
    TyreCompound soft{"Soft",-0.5,0.03,15.0,0.8,3.0};
    TyreCompound medium{"Medium",0.0,0.02,25.0,0.6,1.0};
    TyreCompound hard{"Hard",0.6,0.01,35.0,0.5,0.8};
    RaceConfig config;
    config.trackLengthKm = 5.0;
    config.totalLaps = 50;
    config.baseLapTime = 82.0;
    config.fuelKg = 100.0;
    config.fuelBurnPerLapKg = 1.8;
    config.fuelPenaltyPerKg = 0.03;
    config.pitLossSeconds = 23.0;
    config.randomSigma = 0.10;

    config.compounds.push_back(soft);
    config.compounds.push_back(medium);
    config.compounds.push_back(hard);
    
    std::mt19937 rng(42);
    std::normal_distribution<double> noise(0.0, config.randomSigma);
    for (int lap=0; lap < 40; ++lap){
        double t = computeLapTime(config.compounds[0],lap,config.fuelKg,false,config.pitLossSeconds,config.baseLapTime,config.fuelPenaltyPerKg,rng,noise);
        std::cout << "LAP "<<lap+1<<": "<<t<<std::endl;
        config.fuelKg -= config.fuelBurnPerLapKg;
    };
    return 0;
}