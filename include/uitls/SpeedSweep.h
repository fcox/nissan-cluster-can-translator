#ifndef SPEED_SWEEP_H
#define SPEED_SWEEP_H

#include "enum/SweepState.h"

class SpeedSweep {
private:
    float minVal;
    float maxVal;
    float step;
    float currentVal;

    unsigned long riseDuration;
    unsigned long fallDuration;
    unsigned long pauseDuration;

    SweepState state;
    unsigned long lastUpdate;
    unsigned long interval;

public:
    SpeedSweep(float min, float max, float step);

    void configure(unsigned long riseTime, unsigned long fallTime, unsigned long pause);
    void start();
    void update();
    float getCurrentValue() const;
    bool isRunning() const;
};

#endif