#include "controller/RPMController.h"
#include <Arduino.h>
#include <stdint.h>

RPMController::RPMController(float min, float max, float step)
    : minSpeed(min), maxSpeed(max), stepSize(step), currentSpeed(min), sweepUpTime(0), sweepDownTime(0),
      pauseDuration(0),
      state(SweepState::Stopped), lastUpdate(0), currentInterval(0)
{
}

void RPMController::startSweep(unsigned long riseTime, unsigned long fallTime, unsigned long pause) {
    sweepUpTime = riseTime;
    sweepDownTime = fallTime;
    pauseDuration = pause;
    currentSpeed = minSpeed;
    state = SweepState::Rising;

    const float totalSteps = (maxSpeed - minSpeed) / stepSize;
    currentInterval = riseTime / totalSteps;
    lastUpdate = millis();
}

void RPMController::update() {
    if(!singleShot) return;

    if(state == SweepState::Stopped) return;


    unsigned long now = millis();
    if(now - lastUpdate < currentInterval) return;

    lastUpdate = now;

    switch(state) {
        case SweepState::Rising:
            currentSpeed += stepSize;
            if(currentSpeed >= maxSpeed) {
                currentSpeed = maxSpeed;
                state = SweepState::PausedTop;
                currentInterval = pauseDuration;
            }
            break;

        case SweepState::PausedTop:
            state = SweepState::Falling;
            currentInterval = sweepDownTime / ((maxSpeed - minSpeed) / stepSize);
            break;

        case SweepState::Falling:
            currentSpeed -= stepSize;
            if(currentSpeed <= minSpeed) {
                currentSpeed = minSpeed;
                state = SweepState::PausedBottom;
                currentInterval = pauseDuration;
            }
            break;

        case SweepState::PausedBottom:
            state = SweepState::Rising;
            currentInterval = sweepUpTime / ((maxSpeed - minSpeed) / stepSize);
            break;

        default: break;
    }

    if(state == SweepState::PausedBottom && singleShot) {
        state = SweepState::Stopped;
        singleShot = false;
    }
}

float RPMController::getCurrentSpeed() const {
    return currentSpeed;
}

SweepState RPMController::getState() const {
    return state;
}

uint16_t RPMController::getCANValue() const {
    return static_cast<uint16_t>(currentSpeed * 100);
}

bool RPMController::isRunning() const
{
    return state != SweepState::Stopped;
}

void RPMController::startSingleSweep() {
    startSweep(sweepUpTime, sweepDownTime, pauseDuration);
    singleShot = true;
}
