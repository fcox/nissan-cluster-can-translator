#ifndef SPEED_SENDER_H
#define SPEED_SENDER_H

#include "CANSender.h"

class SpeedSender : public CANSender {
public:
    SpeedSender(MCP_CAN& canRef, float& speedRef);
    void send(unsigned long id, byte* data, unsigned int length) override;
    void send() override;
    void setSpeed(float speed);

private:
    float& currentSpeed;
    const unsigned long SPEED_L_ID = 0x284;
    const unsigned long SPEED_R_ID = 0x285;

};

#endif