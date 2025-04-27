#include "uitls/SpeedSweep.h"
#include <Arduino.h>
#include "enum/SweepState.h"

SpeedSweep::SpeedSweep(float min, float max, float step)
    : minVal(min), maxVal(max), step(step),
      currentVal(min), riseDuration(0), fallDuration(0), pauseDuration(0), state(SweepState::Stopped),
      lastUpdate(0), interval(0)
{
}

void SpeedSweep::configure(unsigned long riseTime, unsigned long fallTime, unsigned long pause) {
    riseDuration = riseTime;
    fallDuration = fallTime;
    pauseDuration = pause;
}

void SpeedSweep::start() {
    currentVal = minVal;
    state = SweepState::Rising;
    interval = riseDuration / ((maxVal - minVal) / step);
    lastUpdate = millis();
}

void SpeedSweep::update() {
    if(state == SweepState::Stopped) return;

    unsigned long now = millis();
    if(now - lastUpdate < interval) return;

    lastUpdate = now;

    switch(state) {
    case SweepState::Rising:
        currentVal += step;
        if(currentVal >= maxVal) {
            currentVal = maxVal;
            state = SweepState::PausedTop;
            interval = pauseDuration;
        }
        break;

    case SweepState::PausedTop:
        state = SweepState::Falling;
        interval = fallDuration / ((maxVal - minVal) / step);
        break;

    case SweepState::Falling:
        currentVal -= step;
        if(currentVal <= minVal) {
            currentVal = minVal;
            state = SweepState::PausedBottom;
            interval = pauseDuration;
        }
        break;

    case SweepState::PausedBottom:
        state = SweepState::Rising;
        interval = riseDuration / ((maxVal - minVal) / step);
        break;

    default: break;
    }
}

float SpeedSweep::getCurrentValue() const {
    return currentVal;
}

bool SpeedSweep::isRunning() const {
    return state != SweepState::Stopped;
}