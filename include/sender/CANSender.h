#ifndef CAN_SENDER_H
#define CAN_SENDER_H

#include <mcp_can.h>

class CANSender {
protected:
    MCP_CAN& can;
    unsigned long lastSent = 0;
    unsigned long interval;
    byte rollingCounter = 0;
    bool autoSend = true;

public:
    virtual ~CANSender() = default;
    CANSender(MCP_CAN& canRef, unsigned long intervalMs);

    virtual void send(unsigned long id, byte* data, unsigned int length) = 0;
    virtual void send() = 0;

    void update();
    void setAutoMode(bool enable);
};

#endif