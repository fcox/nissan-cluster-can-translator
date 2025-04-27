#ifndef ECM_SENDER_H
#define ECM_SENDER_H

#include "CANSender.h"

class ECMSender : public CANSender {
    const unsigned long ECM_ID = 0x180;
    const unsigned long ECM2_ID = 0x1F9;
    const unsigned long ECM3_ID = 0x19B;
    const unsigned long ECM4_ID = 0x160;

public:
    ECMSender(MCP_CAN& canRef);
    void send() override;
    void send(unsigned long id, byte* data, unsigned int length) override;
    void setRPM(float rpm);
};

#endif