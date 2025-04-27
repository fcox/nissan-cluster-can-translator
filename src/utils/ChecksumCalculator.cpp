#include "uitls/ChecksumCalculator.h"


byte ChecksumCalculator::calculateChecksum(byte* data, byte length) {
    byte checksum = 0xFF;
    for (byte i = 0; i < length; i++) {
        checksum ^= data[i];
        checksum = (checksum << 1) | (checksum >> 7);
    }
    return checksum;
}

byte ChecksumCalculator::calculateNissanCRC(byte* data, byte len) {
    byte crc = 0xFF;
    for (byte i = 0; i < len; i++) {
        crc ^= data[i];
        for (byte j = 0; j < 8; j++) {
            if (crc & 0x80) crc = (crc << 1) ^ 0x8D;
            else crc <<= 1;
        }
    }
    return crc;
}