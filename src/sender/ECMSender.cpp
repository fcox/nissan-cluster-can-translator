#include "sender/ECMSender.h"
#include "uitls/GlobalData.h"

ECMSender::ECMSender(MCP_CAN& canRef) : CANSender(canRef, 20) {}

void ECMSender::send() {

    setRPM(vehicleState.rpm);
}

void ECMSender::send(unsigned long id, byte* data, unsigned int length) {


    can.sendMsgBuf(id, 0, length, data);
    // byte ecmData[8] = {0xBB, 0x80, rollingCounter, rollingCounter, rollingCounter, 0x00, 0x00, 0x00};
    // byte ecm2Data[8] = {0x00, 0x00, rollingCounter, 0xBB, 0x80, 0x00, 0x00, 0x00};
    // byte ecm3Data[8] = {0xf0, 0x00, rollingCounter, 0xBB, 0x00, 0x00, 0x00, 0x00};
    // byte ecm4Data[8] = {rollingCounter, rollingCounter, rollingCounter, rollingCounter,
    //                    rollingCounter, rollingCounter, rollingCounter, rollingCounter};
    //
    // can.sendMsgBuf(ECM_ID, 0, 8, ecmData);
    // can.sendMsgBuf(ECM2_ID, 0, 8, ecm2Data);
    // can.sendMsgBuf(ECM3_ID, 0, 8, ecm3Data);
    // can.sendMsgBuf(ECM4_ID, 0, 8, ecm4Data);
}

void ECMSender::setRPM(const float rpm)
{
    unsigned int dataLength = 8;
    const uint16_t value = static_cast<uint16_t>(rpm * 8 );

    byte data[8] = {
        highByte(value),
        lowByte(value),
        0x00,
        0x00,
        rollingCounter,
        0x00,
        0x00,
        0x00
    };

    send(ECM_ID, data, dataLength );
    vehicleState.rpm = rpm;
}


//byte ecmData[8] = { //(180)
    //         rollingCounter,rollingCounter,
    //         //0xBB, 0x80,             // 2000 RPM (0x3E80)  value * 8 = desired rpm
    //         rollingCounter,
    //         rollingCounter,         // Counter
    //         rollingCounter,
    //         0x00,
    //         0x00,
    //         0x00
    //     };
    //
    //     byte ecm2Data[8] = { //(1F9)
    //         0x00,
    //         0x00,
    //         rollingCounter,
    //         0xBB, 0x80,             // 2000 RPM (0x3E80)  value * 8 = desired rpm
    //         0x00,
    //         0x00,
    //         0x00
    //     };
    //
    //     byte ecm3Data[8] = { //(19B)
    //         0xf0,
    //         0x00,
    //         rollingCounter,
    //         0xBB,
    //         0x00,
    //         0x00,
    //         0x00,
    //         0x00
    //     };
    //
    //     byte ecm4Data[8] = { //(160)
    //         rollingCounter,
    //         rollingCounter,
    //         rollingCounter,
    //         rollingCounter,
    //         rollingCounter,
    //         rollingCounter,
    //         rollingCounter,
    //         rollingCounter,
    //     };
    //     CAN.sendMsgBuf(ECM_ID, 0, 8, ecmData);
    //     CAN.sendMsgBuf(ECM2_ID, 0, 8, ecm2Data);
    //     CAN.sendMsgBuf(ECM3_ID, 0, 8, ecm3Data);
    //     CAN.sendMsgBuf(ECM4_ID, 0, 8, ecm4Data);
    // }