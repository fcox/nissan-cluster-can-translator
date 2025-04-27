
#include "sender/ABSSender.h"
#include "uitls/GlobalData.h"

ABSSender::ABSSender(MCP_CAN& canRef) : CANSender(canRef, 50) {}

void ABSSender::send()
{
    setABSSpeed(vehicleState.speed);
    setABS2Speed(vehicleState.speed);
    setABSWarningLight(vehicleState.absLight);
    setHandBrakeWarningLight(vehicleState.handBrakeLight);
}

void ABSSender::send(unsigned long id, byte* data, unsigned int length) {

    // byte absData[8] = {
    //     rollingCounter,rollingCounter,
    //     //0x0F, 0xA0, //Absolute speed (desired speed * 1000).ToHex()
    //     0x00,
    //     0x00,
    //     0x00, // 0x10 = ABS light on. 0x02 = Handbrake light on. 0x12 = both on
    //     0x00,
    //     0x08,
    //     0x00
    // };
    //
    // byte absData2[8] = {
    //     rollingCounter,rollingCounter,
    //     rollingCounter,rollingCounter,
    //     // 0x0F, 0xA0, //absolute speed (desired speed * 1000).ToHex()
    //     // 0x0F, 0xA0, //absolute speed (desired speed * 1000).ToHex()
    //     0x00,
    //     0xff,
    //     0x00,
    //     0x00
    // };

    can.sendMsgBuf(ABS_ID, 0, length, data);
}

void ABSSender::setABSSpeed(const float speed)
{
    const uint16_t canValue = static_cast<uint16_t>(speed * 100 );
    unsigned int dataLength = 8;
    byte data[8] = {
        highByte(canValue), lowByte(canValue),
        0x00,0x00,
        0x00,0x00,
        0x00, 0x00
    };

    send(ABS_ID, data, dataLength);

    vehicleState.speed = speed;

    setABS2Speed(speed);

}

void ABSSender::setABS2Speed(const float speed)
{
    const uint16_t canValue = static_cast<uint16_t>(speed * 100 );
    unsigned int dataLength = 8;
    byte data[8] = {
        highByte(canValue), lowByte(canValue),
        highByte(canValue), lowByte(canValue),
        0x00, 0x00,
        0x00, 0x00
    };

    send(ABS2_ID, data, dataLength);

    vehicleState.speed = speed;
}

void ABSSender::setABSWarningLight(bool isActive)
{
    byte value = (isActive ? 0x10 : 0x00) | (vehicleState.handBrakeLight ? 0x02 : 0x00);

    constexpr int dataLength = 8;
    byte data[8] = {
        0x00,
        0x00,
        0x00,
        0x00,
        value, // 0x10 = ABS light on. 0x02 = Handbrake light on. 0x12 = both on
        0x00,
        0x08,
        0x00
    };
    send(ABS_ID, data, dataLength);

    vehicleState.absLight = isActive;
}

void ABSSender::setHandBrakeWarningLight(bool isActive)
{
    byte value = (vehicleState.absLight ? 0x10 : 0x00) | (isActive ? 0x02 : 0x00);

    constexpr int dataLength = 8;
    byte data[8] = {
        0x00,
        0x00,
        0x00,
        0x00,
        value, // 0x10 = ABS light on. 0x02 = Handbrake light on. 0x12 = both on
        0x00,
        0x08,
        0x00
    };
    send(ABS_ID, data, dataLength);

    vehicleState.handBrakeLight = isActive;
}