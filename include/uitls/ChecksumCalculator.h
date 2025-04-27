#ifndef CHECKSUM_CALCULATOR_H
#define CHECKSUM_CALCULATOR_H

#include <Arduino.h>

class ChecksumCalculator {
public:
    static byte calculateChecksum(byte* data, byte length);
    static byte calculateNissanCRC(byte* data, byte len);
};

#endif