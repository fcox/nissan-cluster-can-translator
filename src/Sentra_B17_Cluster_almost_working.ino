#include <mcp_can.h>
#include <SPI.h>

const int SPI_CS_PIN = 10;
const float SPEED_FACTOR = 0.245; // 60 km/h * 10.24 = 614
MCP_CAN CAN(SPI_CS_PIN);

// Variables de timing independientes
unsigned long lastECM = 0;
unsigned long lastBCM = 0;
unsigned long lastABS = 0;
unsigned long lastEPS = 0;
unsigned long lastSpeed = 0;
unsigned long lastIPDM = 0;
unsigned long lastNetwork = 0;
unsigned long lastPrndl = 0;
unsigned long lastCluster = 0;

byte rollingCounter = 0;
byte rollingCounter4Bits = 0;

// IDs CAN para Nissan
const unsigned long ECM_ID = 0x180;
const unsigned long ECM2_ID = 0x1F9;
const unsigned long ECM3_ID = 0x19B;
const unsigned long ECM4_ID = 0x160;
const unsigned long BCM_ID = 0x551;
const unsigned long ABSL_ID = 0x354;
const unsigned long ABSR_ID = 0x355;
const unsigned long IPDM_ID = 0x285;
const unsigned long TEMP_ID = 0x3B0;
const unsigned long NETWORK_ID = 0x1F1;
const unsigned long SPEEDL_ID = 0x284;
const unsigned long SPEEDR_ID = 0x285;

const unsigned long EPS_ID = 0x742;
// const unsigned long EPS_ID = 0x2DE;
const unsigned long PRDLE_ID = 0x421; // controls display on dash
const unsigned long CLUSTER_ID = 0x5C5; // I think cluster only sends data or maybe is for override some IDs
const unsigned long BCM1_ID = 0x358;
const unsigned long BCM2_ID = 0x35d;

void setup()
{
    Serial.begin(115200);
    while (CAN_OK != CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ))
    {
        Serial.println("Error al iniciar CAN...");
        delay(100);
    }
    CAN.setMode(MCP_NORMAL);
    Serial.println("CAN iniciado correctamente");
}

void loop()
{
    //handleWarningLights();
    unsigned long now = millis();

    // ECM - Prioridad máxima (20ms)
    if (now - lastECM >= 20)
    {
        sendECM();
        lastECM = now;
        rollingCounter4Bits = (rollingCounter4Bits + 1) % 16;
        rollingCounter = (rollingCounter + 1) % 256;
    }

    if (now - lastNetwork >= 20)
    {
        byte networkData[8] = {0x80, 0x08, 0x07, 0x00, 0x00, 0x00, rollingCounter, 0x00};
        networkData[7] = calculateNissanCRC(networkData, 7);
        CAN.sendMsgBuf(NETWORK_ID, 0, 8, networkData);
        lastNetwork = now;
        rollingCounter = (rollingCounter + 1) % 256;
        rollingCounter4Bits = (rollingCounter4Bits + 1) % 16;
    }

    // PRNDLE (50ms)
    if (now - lastPrndl >= 50)
    {
        sendPRNDL();
        lastPrndl = now;
    }

    // ABS (50ms)
    if (now - lastABS >= 50)
    {
        sendABS();
        lastABS = now;
    }

    // BCM (100ms)
    if (now - lastBCM >= 100)
    {
        sendBCM();
        lastBCM = now;
    }

    // BCM (100ms)
    if (now - lastCluster >= 100)
    {
        sendCluster();
        lastCluster = now;
    }

    // Velocidad (100ms)
    if (now - lastSpeed >= 40)
    {
        sendSpeed();
        lastSpeed = now;
    }

    // EPS (100ms)
    if (now - lastEPS >= 50)
    {
        sendEPS();
        lastEPS = now;
    }

    // IPDM (100ms)
    // if (now - lastIPDM >= 100)
    // {
    //     sendFakeIPDMData();
    //     lastIPDM = now;
    // }
}

//--------------- Módulos Mejorados ---------------
void sendECM()
{
    byte ecmData[8] = { //(180)
        0xBB, 0x80,             // 2000 RPM (0x3E80)  value * 8 = desired rpm
        rollingCounter,
        rollingCounter,         // Contador incremental
        rollingCounter,
        0x00,
        0x00,
        0x00
    };

    byte ecm2Data[8] = { //(1F9)
        0x00,
        0x00,
        rollingCounter,
        0xBB, 0x80,             // 2000 RPM (0x3E80)  value * 8 = desired rpm
        0x00,
        0x00,
        0x00
    };

    byte ecm3Data[8] = { //(19B)
        0xf0,
        0x00,
        rollingCounter,
        0xBB,
        0x00,
        0x00,
        0x00,
        0x00
    };

    byte ecm4Data[8] = { //(160)
        rollingCounter,
        rollingCounter,
        rollingCounter,
        rollingCounter,
        rollingCounter,
        rollingCounter,
        rollingCounter,
        rollingCounter,
    };
    CAN.sendMsgBuf(ECM_ID, 0, 8, ecmData);
    CAN.sendMsgBuf(ECM2_ID, 0, 8, ecm2Data);
    CAN.sendMsgBuf(ECM3_ID, 0, 8, ecm3Data);
    CAN.sendMsgBuf(ECM4_ID, 0, 8, ecm4Data);
}

void sendPRNDL()
{
    byte prndlData[8] = {
        //rollingCounter,
        0x00,                   // Gear selection 0x18 = N, 0x80 = 1, 0x10 = R
        0x41, 0x00, 0x00,
         0x00, 0x00,
         0x00, 0x00,//0x041 turns on O/D OFF when a specific gear is put (i think D)
        // rollingCounter,
        // rollingCounter,rollingCounter,
        // rollingCounter
    };
    CAN.sendMsgBuf(PRDLE_ID, 0, 8, prndlData);
}

void sendBCM()
{
    byte lala = (0x10 | (rollingCounter4Bits & 0x0F));

    byte bcmData[8] = {//(551)
        rollingCounter,//0x7D,  //Engine temp: (Desired temp + 40).ToHex()
        rollingCounter,         //Average fuel consumption
        0x00,
        0x80,
        0x01,
        0x10,                   // 0x40 Turns on ASCD
        0x00,
        rollingCounter
    };

    byte bcm1Data[8] = {        //(358)
        0x80,
        0x00,
        0x00,
        0x40,
        rollingCounter,
        0x42, 0x00,
        calculateChecksum(bcmData, 7)
    };

    byte bcm2Data[8] = {
        //(358)
        0x01,
        0x00,
        0x00,
        0x40,
        rollingCounter,
        0x42, 0x00,
        calculateChecksum(bcmData, 7)
    };

    CAN.sendMsgBuf(BCM_ID, 0, 8, bcmData);
    CAN.sendMsgBuf(BCM1_ID, 0, 8, bcm1Data);
    CAN.sendMsgBuf(BCM2_ID, 0, 8, bcm2Data);
}

void sendABS()
{
    byte lala = (0x10 | (rollingCounter4Bits & 0x0F));

    byte absData[8] = {
        0x0F, 0xA0, //Absolute speed (desired speed * 1000).ToHex()
        0x00,
        0x00,
        0x00, // 0x10 = ABS light on. 0x02 = Handbrake light on. 0x12 = both on
        0x00,
        0x08,
        0x00
    };

    byte absData2[8] = {
        0x0F, 0xA0, //absolute speed (desired speed * 1000).ToHex()
        0x0F, 0xA0, //absolute speed (desired speed * 1000).ToHex()
        0x00,
        0xff,
        0x00,
        0x00
    };
    CAN.sendMsgBuf(ABSL_ID, 0, 8, absData);
    CAN.sendMsgBuf(ABSR_ID, 0, 8, absData2);
    Serial.println(lala, HEX);
}

void sendCluster()
{
    byte clusterData[8] = {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        // 0x00,
        // 0x00,
        // 0x50,
        // 0x10
        // 0x00, 0x00,
        // calculateChecksum(epsData, 7)
    };
    CAN.sendMsgBuf(CLUSTER_ID, 0, 8, clusterData);
}

void sendEPS()
{
    byte lala = (0x00 | (rollingCounter4Bits & 0x0F));
    byte epsData[5] = {
        rollingCounter,
        rollingCounter,
        rollingCounter,
        rollingCounter,
        rollingCounter
        // 0x00, 0x00,
        // calculateChecksum(epsData, 7)
    };

    CAN.sendMsgBuf(EPS_ID, 0, 5, epsData);
}

void sendSpeed()
{
    byte speedData[8] = {
        0x0F, 0xA0, //FR(284) RR(285) wheel speed (desired speed * 1000).ToHex()
        0x0F, 0xA0, //FL(284) RL(285) wheel speed (desired speed * 1000).ToHex()
        0x0F, 0xA0, // Absolute speed (desired speed * 1000).ToHex()
        rollingCounter,
        0x00
        //calculateChecksum(speedData, 7)
    };
    CAN.sendMsgBuf(SPEEDL_ID, 0, 8, speedData);
    CAN.sendMsgBuf(SPEEDR_ID, 0, 8, speedData);
}

void sendFakeIPDMData()
{
    byte ipdmData[8] = {
        // Byte 0: Luces
        (true << 5) | // Faros encendidos
        (false << 1) | // Intermitente izquierdo
        (false << 0), // Intermitente derecho

        // Byte 1: Limpiaparabrisas
        0x00, // Modo off

        // Byte 2: Ventilador
        (3 << 4), // Velocidad 3

        // Byte 3: Voltaje
        0x9A, // 13.8V (0x9A = 154 → 154*0.1 + 10 = 25.4V? Revisar fórmula)

        // Bytes 4-5
        0x00, 0x00,

        // Checksum y contador
        rollingCounter,
        calculateChecksum(ipdmData, 7)
    };
    CAN.sendMsgBuf(IPDM_ID, 0, 8, ipdmData);
}

byte calculateChecksum(byte* data, byte length)
{
    byte checksum = 0xFF;
    for (byte i = 0; i < length; i++)
    {
        checksum ^= data[i];
        checksum = (checksum << 1) | (checksum >> 7); // Rotación típica Nissan
    }
    return checksum;
}

// CRC-8 Nissan (Polinomio 0x8D)
byte calculateNissanCRC(byte* data, byte len)
{
    byte crc = 0xFF;
    for (byte i = 0; i < len; i++)
    {
        crc ^= data[i];
        for (byte j = 0; j < 8; j++)
        {
            if (crc & 0x80) crc = (crc << 1) ^ 0x8D;
            else crc <<= 1;
        }
    }
    return crc;
}

void handleWarningLights()
{
    static unsigned long lastABSMsg = 0;
    if (millis() - lastABSMsg >= 100)
    {
        // Mensaje ABS (0x1D4) - Byte 1: 0x00 = ABS OK, 0x01 = Falla
        byte absData[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        absData[1] = 0x00; // Cambiar a 0x01 para testigo ABS
        // CAN.sendMsgBuf(ABSL_ID, 0, 8, absData);
        //CAN.sendMsgBuf(ABSR_ID, 0, 8, absData);
        lastABSMsg = millis();
    }
}
