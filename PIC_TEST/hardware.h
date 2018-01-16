/* 
 * File:   hardware.h
 * Author: rtotte
 *
 * Created on December 12, 2017, 10:28 PM
 */

#ifndef HARDWARE_H
#define	HARDWARE_H

#ifdef	__cplusplus
extern "C" {
#endif

// Determine the transmitter
#define USE_T39A

/** MCU oscillator frequency. Used in PICC __delay macro. */
#define _XTAL_FREQ  8000000

#define CTRL        CTRL_OUT
#define CTRL_OUT    LATAbits.LATA5
#define CTRL_DIR    TRISAbits.TRISA5

#define DATA_OUT    LATAbits.LATA2
#define DATA_DIR    TRISAbits.TRISA2

#define output_pin	0
#define input_pin	1

#define LED_OFF     0
#define LED_ON      1

#define LED         LED_OUT
#define LED_OUT     LATAbits.LATA0
#define LED_DIR     TRISAbits.TRISA0

#define BUTTON_IN      PORTAbits.RA4
#define BUTTON_DIR     TRISAbits.TRISA4
#define BUTTON_WPU     WPUAbits.WPUA4
#define BUTTON_IOC     IOCANbits.IOCAN4

/* Data rate configuration */
#if(_XTAL_FREQ == 8000000ul)
    #define TMR2_PRESCALER      0b01
    #define PWM_ON              100
    #define PWM_PERIOD          (PWM_ON+1)
#else
    #error unsupported system clock frequency
#endif

// Choose the fundamental frequency
#define USE_915MHZ

/* Choose the frequency of the reference oscillator. Typically 24 or
 * 26 MHz. This is used to determine the correct BAND setting for the
 * transmitter configuration. */
#define RF_XTAL 26000000

#include "T39A.h"

// Determine the transmitter configuration settings
// Assume 26 MHz Reference, and Band 1 operation.
// Assume auto-transmit mode with 20 ms sleep delay.
// Assume FSK if not using OOK.
// Assume frequency deviation of 40 kHz.
#define T39A_APP_CONFIG (T39_MODE_AUTO | T39_MOD_FSK | T39_BAND1 | T39_FDEV_40kHz | T39_TX_10dB | T39_TX_20ms | T39_RESERVED_BITS)

#define T39A_FREQ_CONFIG (FREQUENCY_91500)

#ifdef	__cplusplus
}
#endif

#endif	/* HARDWARE_H */

