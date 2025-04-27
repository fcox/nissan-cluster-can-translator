#include "sender/BCMSender.h"
#include "uitls/GlobalData.h"

BCMSender::BCMSender(MCP_CAN& canRef) : CANSender(canRef, 100) {}


void BCMSender::setEngineTemp(uint16_t temp)
{
    int dataLength = 8;
    uint16_t value = static_cast<uint16_t>(temp + 40 );
    byte data[dataLength] = {//(551)
        highByte(value),//0x7D,  //Engine temp: (Desired temp + 40).ToHex()
        0x00,         //Average fuel consumption?
        0x00,
        0x00,
        0x00,
        0x00,                   // 0x40 Turns on ASCD cluster light
        0x00,
        0x00
    };

    send(BCM_ID, data, dataLength);
    vehicleState.engineTemp = temp;
}

void BCMSender::setASCDLight(bool active)
{
    constexpr int dataLength = 8;
    const byte value = active ? 0x40 : 0x00;
    byte data[dataLength] = {//(551)
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        value,                   // 0x40 Turns on ASCD cluster light
        0x00,
        0x00
    };

    send(BCM_ID, data, dataLength);
    vehicleState.cruiseLight = active;
}

void BCMSender::send(unsigned long id, byte* data, unsigned int length) {

    // byte bcmData[8] = {//(551)
    //     rollingCounter,//0x7D,  //Engine temp: (Desired temp + 40).ToHex()
    //     rollingCounter,         //Average fuel consumption
    //     0x00,
    //     0x80,
    //     0x01,
    //     0x10,                   // 0x40 Turns on ASCD cluster light
    //     0x00,
    //     rollingCounter
    // };
    //
    // byte bcm1Data[8] = {        //(358)
    //     0x80,
    //     0x00,
    //     0x00,
    //     0x40,
    //     rollingCounter,
    //     0x42, 0x00,
    //     ChecksumCalculator::calculateChecksum(bcmData, 7)
    // };
    //
    // byte bcm2Data[8] = {
    //     //(358)
    //     0x01,
    //     0x00,
    //     0x00,
    //     0x40,
    //     rollingCounter,
    //     0x42, 0x00,
    //     ChecksumCalculator::calculateChecksum(bcmData, 7)
    // };


    can.sendMsgBuf(id, 0, length, data);
    // can.sendMsgBuf(BCM1_ID, 0, 8, bcm1Data);
    // can.sendMsgBuf(BCM2_ID, 0, 8, bcm2Data);
}

void BCMSender::send()
{
    setEngineTemp(vehicleState.engineTemp);
    setASCDLight(vehicleState.cruiseLight);
}
