#ifndef GLOBAL_DATA_H
#define GLOBAL_DATA_H
#include "enum/IgnitionState.h"

struct VehicleState {
    float speed;  // km/h
    float rpm;
    float engineTemp;   // Celsius
    IgnitionSwitchState ignitionSwitchState;
    bool absLight;
    bool handBrakeLight;
    bool epsLight;
    bool cruiseLight;
    float cruiseSpeed;
};

// Declaración externa para acceso global controlado
extern VehicleState vehicleState;

#endif