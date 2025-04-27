#include "sender/CANSender.h"

CANSender::CANSender(MCP_CAN& canRef, unsigned long intervalMs)
    : can(canRef), interval(intervalMs) {}

void CANSender::update() {

    if(!autoSend)
        return;

    unsigned long now = millis();
    if(now - lastSent >= interval) {
        send();
        lastSent = now;
        rollingCounter = (rollingCounter + 1) % 256;
    }
}

void CANSender::setAutoMode(bool enable){
    autoSend = enable;
}
