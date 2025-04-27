#ifndef BCMSENDER_H
#define BCMSENDER_H

#include "CANSender.h"

class BCMSender : public CANSender
{
    const unsigned long BCM_ID = 0x551;
    const unsigned long BCM1_ID = 0x358;
    const unsigned long BCM2_ID = 0x35D;

public:
    BCMSender(MCP_CAN& canRef);
    void send(unsigned long id, byte* data, unsigned int length) override;
    void send() override;
    void setEngineTemp(uint16_t temp);
    void setASCDLight(bool active);
};

#endif //BCMSENDER_H
