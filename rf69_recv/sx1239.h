//
// MICROCHIP SOFTWARE NOTICE AND DISCLAIMER: You may use this software,
// and any derivatives created by any person or entity by or on your
// behalf, exclusively with Microchip's products. Microchip and its
// licensors retain all ownership and intellectual property rights in
// the accompanying software and in all derivatives hereto.
//
// This software and any accompanying information is for suggestion
// only. It does not modify Microchip's standard warranty for its
// products. You agree that you are solely responsible for testing the
// software and determining its suitability. Microchip has no obligation
// to modify, test, certify, or support the software.
//
// THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
// WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO,
// IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
// FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH
// MICROCHIP'S PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN
// ANY APPLICATION.
//
// IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY,
// TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT
// LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT,
// SPECIAL, PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS,
// DAMAGE, FOR COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
// SOFTWARE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
// POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE FULLEST EXTENT
// ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY
// WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF
// ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
//
// MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE
// OF THESE TERMS.
//

/**
 * @file sx1239.h
 */
#ifndef __SX1239_H
#define __SX1239_H

#include <stdint.h>

//SX1239 addresses
#define REGFIFO_ADDRESS         0x00
#define	REGOPMODE_ADDRESS       0x01
#define	REGDATAMODUL_ADDRESS    0x02
#define	REGBITRATEMSB_ADDRESS   0x03
#define	REGBITRATELSB_ADDRESS   0x04
#define	REGFRFMSB_ADDRESS       0x07
#define	REGFRFMID_ADDRESS       0x08
#define	REGFRFLSB_ADDRESS       0x09
#define REGVERSION_ADDRESS      0x10

//Receiver Registers
#define REGLNA_ADDRESS          0x18
#define REGRXBW_ADDRESS			0x19
#define	REGLISTEN1_ADDRESS		0x0D
#define	REGLISTEN2_ADDRESS		0x0E
#define	REGLISTEN3_ADDRESS		0x0F
/** OOK demodulator selection and control in peak mode */
#define REGOOKPEAK_ADDRESS      0x1B
/** Average threshold control of the OOK demodulator */
#define REGOOKAVG_ADDRESS       0x1C
/** Fixed threshold control of the OOK demodulator */
#define REGOOKFIX_ADDRESS       0x1D
//Packet Engine Registers
/** Sync word configuration */
#define REGSYNCCONFIG_ADDRESS   	0x2E
#define REGSYNCVALUE1_ADDRESS		0x2F
#define REGSYNCVALUE2_ADDRESS		0x30
#define	REGPACKETCONFIG1_ADDRESS	0x37
#define	REGPACKETCONFIG2_ADDRESS	0x3D
#define	REGPAYLOADLENGTH_ADDRESS	0x38
//IRQ and pin mapping Registers
#define REGDIOMAPPING1_ADDRESS		0x25
#define REGDIOMAPPING2_ADDRESS		0x26
#define REGRXTIMEOUT1_ADDRESS           0x2A
#define REGRXTIMEOUT2_ADDRESS           0x2B
//Debugging registers
#define	REGIRQFLAGS1_ADDRESS		0x27
#define	REGIRQFLAGS2_ADDRESS		0x28

#define REGRSSITHRESH_ADDRESS           0x29

//Received packet information
#define         MESSAGE_LENGTH  15 	//total received payload
#define		BUTTON_PACKET	0	//Specifies which byte in the received buffer contains the button information
#define		LEARN_BUTTON	0b00000001 //The "learn button" byte

#define         FIFOMAXSIZE   66

// Some easy-to-read bit name defines
/** Enable sync word recognition */
#define SYNCCONFIG_SYNC_ENABLE (1<<7)
/** Disable sync word recognition */
#define SYNCCONFIG_SYNC_DISABLE (0<<7)

// RH_RF69_REG_11_PALEVEL
#define RH_RF69_REG_11_PALEVEL                              0x11
#define RH_RF69_PALEVEL_PA0ON                               0x80
#define RH_RF69_PALEVEL_PA1ON                               0x40
#define RH_RF69_PALEVEL_PA2ON                               0x20
#define RH_RF69_PALEVEL_OUTPUTPOWER                         0x1f

#define RF_FDEVMSB_15000            0x00
#define RF_FDEVLSB_15000            0xF6
#define RF_FDEVMSB_20000            0x01
#define RF_FDEVLSB_20000            0x48
#define RF_FDEVMSB_25000            0x01
#define RF_FDEVLSB_25000            0x9A
#define RF_FDEVMSB_30000            0x01
#define RF_FDEVLSB_30000            0xEC
#define RF_FDEVMSB_35000            0x02
#define RF_FDEVLSB_35000            0x3D
#define RF_FDEVMSB_40000            0x02
#define RF_FDEVLSB_40000            0x8F
#define RF_FDEVMSB_45000            0x02
#define RF_FDEVLSB_45000            0xE1
#define RF_FDEVMSB_50000            0x03
#define RF_FDEVLSB_50000            0x33

#define REG_FDEVMSB       0x05
#define REG_FDEVLSB       0x06

// RegBitRate (bits/sec) example bit rates
#define RF_BITRATEMSB_1200            0x68
#define RF_BITRATELSB_1200            0x2B
#define RF_BITRATEMSB_2400            0x34
#define RF_BITRATELSB_2400            0x15
#define RF_BITRATEMSB_4800            0x1A  // Default
#define RF_BITRATELSB_4800            0x0B  // Default
#define RF_BITRATEMSB_9600            0x0D
#define RF_BITRATELSB_9600            0x05
#define RF_BITRATEMSB_19200           0x06
#define RF_BITRATELSB_19200           0x83
#define RF_BITRATEMSB_38400           0x03
#define RF_BITRATELSB_38400           0x41

#define RF_BITRATEMSB_38323           0x03
#define RF_BITRATELSB_38323           0x43
#define RF_BITRATEMSB_34482           0x03
#define RF_BITRATELSB_34482           0xA0

#define RF_BITRATEMSB_76800           0x01
#define RF_BITRATELSB_76800           0xA1
#define RF_BITRATEMSB_153600          0x00
#define RF_BITRATELSB_153600          0xD0
#define RF_BITRATEMSB_57600           0x02
#define RF_BITRATELSB_57600           0x2C
#define RF_BITRATEMSB_115200          0x01
#define RF_BITRATELSB_115200          0x16
#define RF_BITRATEMSB_12500           0x0A
#define RF_BITRATELSB_12500           0x00
#define RF_BITRATEMSB_20000           0x06
#define RF_BITRATELSB_20000           0x40
#define RF_BITRATEMSB_25000           0x05
#define RF_BITRATELSB_25000           0x00
#define RF_BITRATEMSB_50000           0x02
#define RF_BITRATELSB_50000           0x80
#define RF_BITRATEMSB_100000          0x01
#define RF_BITRATELSB_100000          0x40
#define RF_BITRATEMSB_150000          0x00
#define RF_BITRATELSB_150000          0xD5
#define RF_BITRATEMSB_200000          0x00
#define RF_BITRATELSB_200000          0xA0
#define RF_BITRATEMSB_250000          0x00
#define RF_BITRATELSB_250000          0x80
#define RF_BITRATEMSB_300000          0x00
#define RF_BITRATELSB_300000          0x6B
#define RF_BITRATEMSB_32768           0x03
#define RF_BITRATELSB_32768           0xD1
// custom bit rates
#define RF_BITRATEMSB_55555           0x02
#define RF_BITRATELSB_55555           0x40
#define RF_BITRATEMSB_200KBPS         0x00
#define RF_BITRATELSB_200KBPS         0xa0

#endif

