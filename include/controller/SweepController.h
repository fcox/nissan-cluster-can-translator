#ifndef SWEEP_CONTROLLER_H
#define SWEEP_CONTROLLER_H
#pragma once

class SweepController {
public:
    virtual ~SweepController() = default;
    virtual void start() = 0;
    virtual void update() = 0;
    virtual float getCurrentValue() const = 0;
    virtual bool isRunning() const = 0;
};

#endif