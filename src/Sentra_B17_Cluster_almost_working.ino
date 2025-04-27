#include <mcp_can.h>

#include "controller/RPMController.h"
#include "controller/SpeedController.h"
#include "enum/SystemState.h"
#include "observer/VehicleDataManager.h"
#include "sender/ABSSender.h"
#include "sender/BCMSender.h"
#include "sender/ECMSender.h"
#include "sender/SpeedSender.h"

const int SPI_CS_PIN = 10;
MCP_CAN CAN(SPI_CS_PIN);

// Instances
VehicleDataManager dataManager;

SpeedController speedController(0, 240, 240);
RPMController rpmController(0, 8000, 8000);
ECMSender ecmSender(CAN);
BCMSender bcmSender(CAN);
ABSSender absSender(CAN);
SpeedSender speedSender(CAN );


SystemState currentState = SystemState::Off;
bool lastIgnitionState = false;
CANSender* senders[] = {
    &ecmSender,
    &bcmSender,
    &absSender
};

class CanSenderObserver : public VehicleDataObserver {
public:
    void onSpeedChanged(float newSpeed) override {
        uint16_t canValue = static_cast<uint16_t>(newSpeed);
        speedSender.setSpeed(canValue);
    }

    void onRpmChanged(float newRpm) override {
        uint16_t canValue = static_cast<uint16_t>(newRpm);
        ecmSender.setRPM(canValue);
    }
};
CanSenderObserver canObserver;

void setup() {
    Serial.begin(115200);
    while (CAN_OK != CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ)) {
        Serial.println("CAN Init Fail");
        delay(100);
    }
    CAN.setMode(MCP_NORMAL);
    Serial.println("CAN Init OK!");
    dataManager.registerObserver(&canObserver);

}

bool checkIgnition() {
    uint8_t IGNITION_PIN = 4;
    //Ignition logic here...
    //In the meantime, I'm simulating the ignition with a pin read
    return digitalRead(IGNITION_PIN) == HIGH;
}

void handleSystemState() {
    bool ignition = checkIgnition();

    if(ignition && !lastIgnitionState) {
        currentState = SystemState::IgnitionOn;
    }
    lastIgnitionState = ignition;
}

void loop() {
    handleSystemState();
    VehicleState currentVehicleState = {speedController.getCurrentSpeed(), rpmController.getCurrentSpeed()}; // Obtén datos reales
    dataManager.checkForChanges(currentVehicleState);
    switch(currentState) {
    case SystemState::IgnitionOn:
        speedController.startSingleSweep();
        rpmController.startSingleSweep();
        currentState = SystemState::SweepInProgress;
        break;

    case SystemState::SweepInProgress:
        speedController.update();
        rpmController.update();

        if(!speedController.isRunning() && !rpmController.isRunning()) {
            currentState = SystemState::SweepComplete;
        }
        break;

    case SystemState::SweepComplete:
        //start receiving and sending CAN messages.

        for (auto sender : senders) {
            sender->setAutoMode(true);
            sender->update();
        }
        break;
    default: ;
    }

    // static unsigned long lastSpeedSend = 0;
    // speedController.update();
    //
    // // Debug
    // static unsigned long lastPrint = 0;
    // if(millis() - lastPrint > 100) {
    //     Serial.print("Speed: ");
    //     Serial.print(speedController.getCurrentSpeed());
    //     Serial.print(" km/h");
    //     Serial.print(" | RPM: ");
    //     Serial.print(rpmController.getCurrentSpeed());
    //     Serial.print(" | State: ");
    //     switch(speedController.getState()) {
    //     case SweepState::Rising:
    //         Serial.println("Rising");
    //         break;
    //     case SweepState::Falling:
    //         Serial.println("Falling");
    //         break;
    //     case SweepState::PausedTop:
    //         Serial.println("Pause (Max)");
    //         break;
    //     case SweepState::PausedBottom:
    //         Serial.println("Pause (Min)");
    //         break;
    //     default:
    //         Serial.println("Stopped");
    //         break;
    //     }
    //     lastPrint = millis();
    // }
}