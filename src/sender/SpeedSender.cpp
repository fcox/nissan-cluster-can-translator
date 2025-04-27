#include "sender/SpeedSender.h"
#include "uitls/GlobalData.h"

SpeedSender::SpeedSender(MCP_CAN& canRef, float& speedRef)
    : CANSender(canRef, 40), currentSpeed(speedRef) {}

void SpeedSender::send(unsigned long id, byte* data, unsigned int length) {

    can.sendMsgBuf(SPEED_L_ID, 0, length, data);
    can.sendMsgBuf(SPEED_R_ID, 0, length, data);
}

void SpeedSender::send()
{
    setSpeed(vehicleState.speed);
}

void SpeedSender::setSpeed(const float speed)
{

    const uint16_t canValue = static_cast<uint16_t>(speed * 100 );
    unsigned int dataLength = 8;
    byte speedData[dataLength] = {
        highByte(canValue), lowByte(canValue),
        highByte(canValue), lowByte(canValue),
        highByte(canValue), lowByte(canValue),
        rollingCounter,
        0x00
    };

    send(0, speedData, dataLength);

    vehicleState.speed = speed;
}
