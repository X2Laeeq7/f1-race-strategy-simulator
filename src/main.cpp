#include <iostream>
#include <vector>
#include "simulator.hpp"
#include "format.hpp"

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
    config.pitLossInSeconds = 18.0;   
    config.pitLossOutSeconds = 5.0;
    config.randomSigma = 0.10;

    config.compounds.push_back(soft);
    config.compounds.push_back(medium);
    config.compounds.push_back(hard);
    
    std::vector<Stint> strategy = {{0,15},{1,35}};
    RaceResult res = simulateRace(config, strategy, 42);
    std::cout << "Laps | Compound | TyreAge | FuelLoad | Time\n";
    for (const auto& lr : res.laps){
        std::cout << "Lap "<<lr.lapNumber <<"/"<<res.laps.size() <<" | "<<lr.compoundName <<" | "<<lr.tyreAge<<" | "<<lr.fuelKg<<" | "<<formatTime(lr.lapTime) <<"\n";
        if (lr.box){std::cout << ">>> Box Box: Switching to "<<config.compounds[lr.nextCompoundIndex].name<<"\n";}
    }
    std::cout << "Total Time: "<<formatTime(res.totalTime)<<"\n";
    std::cout << "Pit stops: " << res.pitStops << "\n";
    std::cout << "Strategy: " << res.strategyString << "\n";
    return 0;
    
}
