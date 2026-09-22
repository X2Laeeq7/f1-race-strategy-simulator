#pragma once
#include <sstream>
#include <iomanip>

inline std::string formatTime(double seconds){
    long long totalMs = std::llround(seconds * 1000.0);
    int ms = totalMs % 1000;
    long long totalSecs = totalMs / 1000;
    int secs = totalSecs % 60;
    long long mins = totalSecs / 60;
    std::ostringstream oss;
    oss << mins << ":"
        << std::setw(2) << std::setfill('0') << secs << "."
        << std::setw(3) << std::setfill('0') << ms;
    return oss.str();
};