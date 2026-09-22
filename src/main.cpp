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

RaceResult simulateRace(const RaceConfig& config, const std::vector<Stint>& strategy, unsigned seed){
    RaceResult result;
    result.totalTime = 0.0;
    result.pitStops = 0;
    int totalLaps = 0;
    for (const auto& s : strategy)totalLaps += s.laps;
    if (totalLaps != config.totalLaps){
        std::cerr << "Warning: strategy laps (" << totalLaps << ") != race laps (" << config.totalLaps << ")\n";
        return result;
    }
    std::mt19937 rng(seed);
    std::normal_distribution<double> noise(0.0, config.randomSigma);
    int lapNumber = 0;
    double fuel = config.fuelKg;
    
    for (size_t s = 0; s < strategy.size();++s){
        const Stint& stint = strategy[s];
        int compoundIdx = stint.compoundIndex;
        int tyreAge = 0;
        bool isLastStint = ( s == strategy.size()-1);
        if (s > 0) result.strategyString += " -> ";
        result.strategyString += config.compounds[compoundIdx].name;
        for (int i=0;i < stint.laps;++i){
            lapNumber++;
            bool isLastLapOfStint = (i == stint.laps -1);
            bool box = isLastLapOfStint && !isLastStint;
            double t = computeLapTime(config.compounds[compoundIdx],tyreAge,fuel,box,config.pitLossSeconds,config.baseLapTime,config.fuelPenaltyPerKg,rng,noise);
            result.totalTime += t;

            LapResult lr;
            lr.lapNumber = lapNumber;
            lr.compoundName = config.compounds[compoundIdx].name;
            lr.tyreAge = tyreAge + 1;
            lr.fuelKg = fuel;
            lr.box = box;
            lr.lapTime = t;
            result.laps.push_back(lr);

            if (box)result.pitStops++;
            else tyreAge++;

            fuel -= config.fuelBurnPerLapKg;
            if (fuel < 0.0){fuel = 0.0;}
        }
    }
    return result;
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
    
    std::vector<Stint> strategy = {{0,15},{0,15},{1,35}};
    RaceResult res = simulateRace(config, strategy, 42);
    for (const auto& lr : res.laps){
        std::cout << "Lap "<<lr.lapNumber << " | "<<lr.compoundName <<" | "<<lr.tyreAge<<" | "<<lr.fuelKg<<" | "<<lr.lapTime <<std::endl;
    }
    std::cout << "Total: "<<res.totalTime<<std::endl;
    double sum = 0.0;
    for (const auto& lr : res.laps) sum += lr.lapTime;
    std::cout << "Sum of laps: " << sum << "\n";
    std::cout << "Total:       " << res.totalTime << "\n";
    std::cout << "Pit stops: " << res.pitStops << "\n";
    std::cout << "Strategy: " << res.strategyString << "\n";
    return 0;
    
}