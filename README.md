# F1 Race Strategy Simulator 🏎️

A C++ race strategy simulator modelling tyre degradation, fuel burn, pit stops, and lap-time variation.

## Version 1.0

- Lap-by-lap race simulation
- Nonlinear tyre degradation (sigmoid cliff model)
- Fuel load and burn
- Pit stops with time loss
- Random lap-time noise (driver mistakes, wind change etc.)
- Multi-stint strategies

## Example Output

```text
Lap 14/50 | Soft | 14 | 76.6 | 1:24.649
Lap 15/50 | Soft | 15 | 74.8 | 1:43.041
>>> Box Box: Switching to Medium
Lap 16/50 | Medium | 1 | 73 | 1:29.178
Lap 17/50 | Medium | 2 | 71.2 | 1:24.178
```

### Prerequisites

- Windows 10/11
- MinGW / g++

### Compile
Run:
 
```bash
./compile.bat
```

### Run

```bash
./sim.exe
```

## 🔬 How It Works

The simulator runs a race lap-by-lap, computing a lap time for each lap based on four factors:

**Tyre degradation** — each compound has a base linear wear rate and a "cliff" modelled with a sigmoid curve. Degradation is gradual at first, then rises sharply once the tyre passes its cliff lap.

**Fuel load** — the car starts heavy and gets lighter each lap as fuel burns. A heavier car is slower, so lap times naturally improve as the race goes on.

**Pit stops** — a fixed time penalty is added to the lap on which the car pits. The tyre age resets, and the next stint begins on a fresh compound.

**Random variation** — a small amount of noise is added to every lap to simulate driver error, wind, and other variation.

A strategy is defined as a list of stints — for example, `Soft for 15 laps, then Medium for 35 laps`. The simulator runs every lap in order, accumulates the total race time, and reports the result.

## 📁 Project Structure

```text
F1 Race Strategy Simulator/
├── src/
│   ├── main.cpp
│   └── format.hpp
│   └── types.hpp
│   └── tyres.hpp
│   └── simulator.hpp
├── compile.bat
└── README.md
```