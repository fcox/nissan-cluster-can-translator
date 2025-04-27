#ifndef VEHICLEDATAMANAGER_H
#define VEHICLEDATAMANAGER_H
#include "VehicleDataObserver.h"
#include "uitls/GlobalData.h"


class VehicleDataManager {
private:
    VehicleState lastState{0, 0};
    const static int MAX_OBSERVERS = 5; //
    VehicleDataObserver* observers[MAX_OBSERVERS];
    int numObservers = 0;

public:
    void checkForChanges(const VehicleState& currentState) {
        if (currentState.speed != lastState.speed) {
            for (int i = 0; i < numObservers; i++) {
                observers[i]->onSpeedChanged(currentState.speed);
            }
            lastState.speed = currentState.speed;
        }
        if (currentState.rpm != lastState.rpm) {
            for (int i = 0; i < numObservers; i++) {
                observers[i]->onRpmChanged(currentState.rpm);
            }
            lastState.rpm = currentState.rpm;
        }
    }

    void registerObserver(VehicleDataObserver* observer) {
        if (numObservers < MAX_OBSERVERS) {
            observers[numObservers++] = observer;
        }
    }
};
#endif //VEHICLEDATAMANAGER_H
