#ifndef RPM_SWEEP_H
#define RPM_SWEEP_H

#include "controller/SweepController.h"
#include "enum/SweepState.h"

class RpmSweep : public SweepController {
public:
    RpmSweep(float min = 0.0, float max = 8000.0, float step = 10.0);

    void start() override;
    void update() override;
    float getCurrentValue() const override;
    bool isRunning() const override;

    void configure(unsigned long riseTime, unsigned long fallTime, unsigned long pause);

private:
    SweepState state;
    float minVal;
    float maxVal;
    float step;
    float currentVal;

    unsigned long riseDuration;
    unsigned long fallDuration;
    unsigned long pauseDuration;
    unsigned long lastUpdate;
    unsigned long interval;
};

#endif