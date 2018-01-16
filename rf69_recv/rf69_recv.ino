// rf69_server.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing server
// with the RH_RF69 class. RH_RF69 class does not provide for addressing or
// reliability, so you should only use RH_RF69  if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf69_client
// Demonstrates the use of AES encryption, setting the frequency and modem
// configuration.
// Tested on Moteino with RFM69 http://lowpowerlab.com/moteino/
// Tested on miniWireless with RFM69 www.anarduino.com/miniwireless
// Tested on Teensy 3.1 with RF69 on PJRC breakout board

#include <SPI.h>
#include <RH_RF69.h>
#include "sx1239_config.h"

// Feather M0
#define RFM69_CS      8
#define RFM69_INT     3
#define RFM69_RST     4

// Feather Wing
//#define RFM69_CS  10   // "B"
//#define RFM69_RST 11   // "A"
//#define RFM69_INT  6   // "D"

// Teensy
//#define RFM69_CS      10
//#define RFM69_INT     9
//#define RFM69_RST     8

// Singleton instance of the radio driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);

void hexdump(const void *buffer, uint32_t len, uint8_t cols);

void setup() 
{
  Serial.begin(9600);
  if (!rf69.init())
    Serial.println("init failed");

  if (!rf69.setFrequency(915.0))
    Serial.println("setFrequency failed");

  RH_RF69::ModemConfig mdmCfg;
  mdmCfg.reg_02 = SX1239_DATAMODE_PACKET | SX1239_MODULATION_FSK;   ///< RH_RF69_REG_02_DATAMODUL
  mdmCfg.reg_03 = RF_BITRATEMSB_4800;   ///< RH_RF69_REG_03_BITRATEMSB
  mdmCfg.reg_04 = RF_BITRATELSB_4800;   ///< RH_RF69_REG_04_BITRATELSB
  mdmCfg.reg_05 = RF_FDEVMSB_40000;   ///< RH_RF69_REG_05_FDEVMSB
  mdmCfg.reg_06 = RF_FDEVLSB_40000;   ///< RH_RF69_REG_06_FDEVLSB
  mdmCfg.reg_19 = SX1239_RXBW_ANS;   ///< RH_RF69_REG_19_RXBW
  mdmCfg.reg_1a = 0xf4;   ///< RH_RF69_REG_1A_AFCBW
  mdmCfg.reg_37 = RH_RF69_PACKETCONFIG1_PACKETFORMAT_VARIABLE | RH_RF69_PACKETCONFIG1_DCFREE_MANCHESTER
                  | RH_RF69_PACKETCONFIG1_CRC_ON; ///< RH_RF69_REG_37_PACKETCONFIG1
  rf69.setModemRegisters(&mdmCfg);

  rf69.setTxPower(14);
  rf69.setThisAddress(0x11);
}

void loop()
{
  if (rf69.available())
  {
    // Should be a message for us now   
    uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf69.recv(buf, &len))
    {
      Serial.print("got request: ");
      hexdump(buf,len,16);
    }
    else
    {
      Serial.println("recv failed");
    }
  }
}


void hexdump(const void *buffer, uint32_t len, uint8_t cols)
{
   uint32_t i;

   for(i = 0; i < len + ((len % cols) ? (cols - len % cols) : 0); i++)
   {
      /* print hex data */
      if(i < len) {
        int x = ((uint8_t*)buffer)[i] & 0xFF;
        if (x < 16) Serial.print('0');
        Serial.print(x, HEX);
        Serial.print(" ");
      }

      if(i % cols == (uint32_t)(cols - 1)) {
         Serial.println("");
      }
   }
}

