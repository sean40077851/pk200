#ifndef _GOODIX_H_
#define _GOODIX_H_

#include <Arduino.h>
#include "GoodixStructs.h"
#include "GoodixFw.h"

#define GOODIX_OK   0

// 0x28/0x29 (0x14 7bit)
#define GOODIX_I2C_ADDR_28  0x14
// 0xBA/0xBB (0x5D 7bit)
#define GOODIX_I2C_ADDR_BA  0x5D

#define GOODIX_CONTACT_SIZE   8
#define GOODIX_MAX_CONTACTS   5

/* Register defines */
#define GT_REG_CFG  0x8047
#define GT_REG_DATA 0x8140

// Write only registers
#define GOODIX_REG_COMMAND        0x8040

// Read/write registers
// The version number of the configuration file
#define GOODIX_REG_CONFIG_DATA  0x8047

#define GOODIX_REG_CONFIG_MIDDLE	0x80A2
#define GOODIX_REG_CONFIG_END		0x80FE

// ReadOnly registers (device and coordinates info)
// Product ID (LSB 4 bytes, GT9110: 0x06 0x00 0x00 0x09)
#define GOODIX_REG_ID           0x8140

#define GOODIX_READ_COORD_ADDR  0x814E


class Goodix {
  public:
    uint8_t i2cAddr;
    struct GTConfig config;
    struct GTInfo info;
    struct GTPoint points[GOODIX_MAX_CONTACTS]; //processed points

    bool begin(uint8_t interruptPin, uint8_t resetPin, uint8_t addr=GOODIX_I2C_ADDR_BA);

    void setHandler(void (*handler)(int8_t, GTPoint*));
    void loop();
    void fwResolution(uint16_t maxX, uint16_t maxY);

    uint8_t configCheck(bool isLilyPi);
    uint8_t productID(char *buf);
    
    GTConfig* readConfig();
    GTInfo* readInfo();



  //--- Private routines ---
  private:
    uint8_t intPin, rstPin;
    
    bool write(uint16_t reg, uint8_t value);
    bool writeBytes(uint16_t reg, uint8_t *data, int nbytes);
    bool readBytes(uint16_t reg, uint8_t *data, int nbytes);
    bool reset();

    uint8_t calcChecksum(uint8_t* buf, uint8_t len);
    uint8_t readChecksum();
    uint8_t test();
    
    int16_t readInput(uint8_t *data);
    
    void (*touchHandler)(int8_t, GTPoint*);
    void configUpdate();
    void onIRQ();

    // --- I2C helper ---
    void i2cStart(uint16_t reg);
};

#endif
