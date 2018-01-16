/* 
 * File:   T39A.h
 * Author: rtotte
 *
 * Created on December 12, 2017, 10:28 PM
 */

#ifndef T39A_H
#define	T39A_H

#ifdef	__cplusplus
extern "C" {
#endif

/** Automatic transmit mode */
#define T39_MODE_AUTO (0)

/** Forced transmit mode */
#define T39_MODE_FORCED (1 << 15)

/** FSK modulation scheme */
#define T39_MOD_FSK (0)

/** OOK modulation scheme */
#define T39_MOD_OOK (1 << 14)

/** Select band 0, for frequencies 310 to 450 MHz */
#define T39_BAND0   (0)

/** Select band 1, for frequencies 860 to 870 MHz and 902 to 928 MHz */
#define T39_BAND1   (1 << 13)

/** Frequency deviation of approximatly 20 kHz. Actual deviation is 19.04 kHz */
#define T39_FDEV_20kHz (6 << 5)

/** Frequency deviation of approximatly 40 kHz. Actual deviation is 41.26 kHz */
#define T39_FDEV_40kHz (13 << 5)

/** Output power 0 dBm */
#define T39_TX_0dB (0)

/** Output power +10 dBm */
#define T39_TX_10dB (1 << 4)

/** 2 ms of inactivity before entering sleep mode. Only used in Automatic mode. */
#define T39_TX_2ms 0

/** 20 ms of inactivity before entering sleep mode. Only used in Automatic mode. */
#define T39_TX_20ms (1 << 3)

/** Bits 2:0 are reserved and should be maintained at their data sheet value. */
#define T39_RESERVED_BITS (0b100)

/** 915 MHz @ Fref = 26 MHz, band = 1 */
#define FREQUENCY_91500 0x46627

/** Initialize the transmitter. */
void TX_Init(void);

/** Issue a soft reset command. */
void T39A_Recover(void);

#ifdef	__cplusplus
}
#endif

#endif	/* T39A_H */

