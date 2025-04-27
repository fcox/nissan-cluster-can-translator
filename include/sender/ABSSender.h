#ifndef ABSSENDER_H
#define ABSSENDER_H

#include "CANSender.h"

class ABSSender : public CANSender
{
    const unsigned long ABS_ID = 0x354;
    const unsigned long ABS2_ID = 0x355;

public:
    ABSSender(MCP_CAN& canRef);
    void setABSSpeed(float speed);
    void send(unsigned long id, byte* data, unsigned int length) override;
    void send() override;
    void setABSWarningLight(bool isActive);
    void setHandBrakeWarningLight(bool isActive);


private:
    void setABS2Speed(float speed);
};

#endif //ABSSENDER_H
