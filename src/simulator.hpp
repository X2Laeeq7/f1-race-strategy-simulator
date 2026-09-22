#pragma once
#include "tyres.hpp"
#include <random>

inline double computeLapTime(
    const TyreCompound& c,
    int tyreAge,
    double fuelKg, 
    bool box,
    double pitLoss,
    double baseLapTime,
    double fuelPenaltyPerKg,
    std::mt19937& rng,
    std::normal_distribution<double>& noise
){
    double tyreLoss = tyreTimeLoss(c,tyreAge);
    double fuelLoss = fuelKg * fuelPenaltyPerKg;
    double n = noise(rng);
    return baseLapTime + c.paceOffset + tyreLoss + fuelLoss + pitLoss + n;
};

inline RaceResult simulateRace(const RaceConfig& config, const std::vector<Stint>& strategy, unsigned seed){
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
            bool isFirstLapOfStint = (i == 0);
            bool box = isLastLapOfStint && !isLastStint;
            bool outLap = isFirstLapOfStint && s > 0;
            double pitPenalty = 0.0;
            if (box) pitPenalty = config.pitLossInSeconds;
            else if (outLap) pitPenalty = config.pitLossOutSeconds;
            double t = computeLapTime(config.compounds[compoundIdx],tyreAge,fuel,box,pitPenalty,config.baseLapTime,config.fuelPenaltyPerKg,rng,noise);
            result.totalTime += t;

            LapResult lr;
            lr.lapNumber = lapNumber;
            lr.compoundName = config.compounds[compoundIdx].name;
            lr.tyreAge = tyreAge + 1;
            lr.fuelKg = fuel;
            lr.box = box;
            lr.lapTime = t;
            if (box) lr.nextCompoundIndex = strategy[s + 1].compoundIndex;
            result.laps.push_back(lr);
            result.pitStops++;

            tyreAge++;

            fuel -= config.fuelBurnPerLapKg;
            if (fuel < 0.0){fuel = 0.0;}
        }
    }
    return result;
};