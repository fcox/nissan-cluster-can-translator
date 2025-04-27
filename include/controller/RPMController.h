#ifndef RPMCONTROLLER_H
#define RPMCONTROLLER_H
#include <stdint.h>
#include "enum/SweepState.h"

class RPMController {
public:
    RPMController(float min = 0.0, float max = 8000.0, float step = 10.0);

    void startSweep(unsigned long riseTime, unsigned long fallTime, unsigned long pause);
    void update();
    void startSingleSweep();
    float getCurrentSpeed() const;
    SweepState getState() const;
    uint16_t getCANValue() const;
    bool isRunning() const;

private:
    float minSpeed;
    float maxSpeed;
    float stepSize;
    float currentSpeed;

    unsigned long sweepUpTime;
    unsigned long sweepDownTime;
    unsigned long pauseDuration;

    SweepState state;
    unsigned long lastUpdate;
    unsigned long currentInterval;
    bool singleShot = false;
};
#endif //RPMCONTROLLER_H
