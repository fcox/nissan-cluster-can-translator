
#ifndef VEHICLEDATAOBSERVER_H
#define VEHICLEDATAOBSERVER_H
#pragma once

class VehicleDataObserver {
public:
    virtual void onSpeedChanged(float newSpeed) = 0;
    virtual void onRpmChanged(float newRpm) = 0;
    virtual ~VehicleDataObserver() = default;
};

#endif //VEHICLEDATAOBSERVER_H
