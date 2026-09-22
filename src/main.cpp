#include <iostream>
#include <vector>
#include "simulator.hpp"

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
    
    std::vector<Stint> strategy = {{0,15},{2,35}};
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