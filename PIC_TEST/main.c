/* 
 * File:   main.c
 * Author: rtotte
 *
 * Created on December 12, 2017, 10:33 PM
 */

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable (PWRT enabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <htc.h>
#include "hardware.h"
#include "typedefs.h"
#include <stdio.h>
#include <stdlib.h>

#define TE_PER      PWM_ON
#define TE          PWM_PERIOD

#define PACKET_LENGTH   21
#define PACKET_BIT_END  (PACKET_LENGTH*8*2)

/** Storage for syncronization counter. */
UINT32_VAL counter;

enum XMIT_STATE
{
    OFF,
    PREAMBLE,
    HEADER,
    DATA,
    GUARD
};

/** Current state of transmitter state machine. */
enum XMIT_STATE p_state = OFF;

/** Storage for encryption key. */
unsigned char key[16];

/**
 * Transmit buffer.
 */
unsigned char dataPacket[PACKET_LENGTH];

/** Current button information. */
unsigned char btn = 0;

UINT16 packetNrOfBits;

static const unsigned char sync_bytes[2] = { 0x2d, 0xd4 };

/** Temporary variables used only in KLQ_Encrypt() function. */
unsigned char CNT1, CNT2, MASK;

/** Shift the dataPacket buffer to the right by 1 bit. */
extern void ShiftBuffer(void);
#pragma regsused ShiftBuffer wreg, status

/** Initialize microcontroller ports. */
void initPorts()
{
    // LED
    LED_OUT = LED_OFF;
    LED_DIR = output_pin;
    
    // Transmitter control
    DATA_OUT = 0;
    DATA_DIR = output_pin;
    CTRL_OUT = 0;
    CTRL_DIR = output_pin;

    // Buttons
    BUTTON_DIR = input_pin;

    // All ports are digital
    ANSELA = 0;

    // port pull-up
    BUTTON_WPU = 1;
    OPTION_REGbits.nWPUEN = 0;
}

/** Initialize PWM module for transmit encoding. */
void initPWM()
{
    // Single output; P1A modulated; P1B assigned as port pins
    CCP1CONbits.P1M = 0b00;

    // PWM mode: P1A active-high
    CCP1CONbits.CCP1M = 0b1100;
    
    // PWM with full period 
    PR2 = TE_PER;
    CCPR1L     = 0;

    // prescaler is 1:4
    T2CONbits.T2CKPS = 0b01;
    T2CONbits.T2CKPS = TMR2_PRESCALER;

    T2CONbits.TMR2ON = 1 ;
}

/** Initialize interrupts */
void initInts()
{
    TMR2IF = 0;
    TMR2IE = 0;
    PEIE = 1;
    
    // BUTTON use pull-downs
    BUTTON_IOC = 1;

    ei();
}

/** Interrupt vector */
static void interrupt int0()
{
    static UINT16 bitnr = 0;

    if (TMR2IF)
    {
        TMR2IF = 0;    
        if (p_state == PREAMBLE)
        {
            if (bitnr & 0x01)
            {
                CCPR1L = 0;
            }
            else
            {
                CCPR1L = TE;
            }
            bitnr++;
            if (bitnr >= 32) 
            {
                p_state = HEADER;
                bitnr = 0;
            }
        }
        else if (p_state == HEADER)
        {
            if (dataPacket[0] & 0x80)
            {
                // it's a one
                CCPR1L = TE;
            }
            else
            {
                CCPR1L = 0;
            }
            // now rotate the data vector
            ShiftBuffer();
            
            bitnr++;            
            if (bitnr >= 16) 
            {
                p_state = DATA;
            }
        }
        else if (p_state == DATA)
        {
            if (dataPacket[0] & 0x80)
            {
                // it's a one
                if (bitnr & 0x01)
                    CCPR1L = 0;
                else
                    CCPR1L = TE;
            }
            else
            {
                // it's a zero
                if (bitnr & 0x01)
                    CCPR1L = TE;
                else
                    CCPR1L = 0;
            }
            if ((bitnr & 0x01) == 1)
                // now rotate the data vector
                ShiftBuffer();
            
            bitnr++;
                
            // check if we sent all bits
            if (bitnr >= packetNrOfBits) 
            { 
                p_state = GUARD ; 
                CCPR1L = 0; 
                bitnr = 0;
            }
        }
        else if (p_state == GUARD)
        {
            CCPR1L = 0;
            bitnr++;
            if (bitnr >= 200) 
            { 
                p_state = OFF ; 
                CCPR1L = 0; 
                bitnr = 0; 
                LED ^= 1;
                TMR2IE = 0;
            }
        }
    }
    if ( IOCIF )
    { // we should never get here
        IOCAF = 0;
        IOCIF = 0;
    }    
}

void sendPacket()
{
    //disable CCP
    CCP1CONbits.CCP1M = 0;

    TX_Init();
    
    packetNrOfBits = PACKET_BIT_END;

    //Start-up time, 2mS
    DATA_OUT = 1;
    __delay_ms(15);
    DATA_OUT = 0;

    //re-enable CCP
    CCP1CONbits.CCP1M = 0b1100;

    p_state = PREAMBLE;

    // start the interrupt
    TMR2IE = 1;
}

/** Prepare interrupt-on-change wake-up and execute sleep. */
void shutDown()
{
    di();

    // enable ints on change
    IOCIE = 1;
    IOCIF = 0;    
    IOCAF = 0;

    SLEEP();

    //disable int on change
    IOCIE = 0;
    IOCIF = 0;
    IOCAF = 0;

    ei();
}


/** Read and populate btn with current switch press seen. */
void readButtons()
{
    // delete all interrupt-on-change events
    IOCAF = 0;

    btn = 0;

    // The remaining buttons are active-low.
    if (BUTTON_IN == 0)
    {
        btn = 1;
    }
}

// CCITT, but it's the 1D0F version of CCITT

#define lo8(x) ((x)&0xff) 
#define hi8(x) (((x)>>8) & 0xFF)

static const UINT16 crctab[256] = {
    0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
    0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
    0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
    0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
    0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
    0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
    0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
    0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
    0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
    0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
    0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
    0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
    0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
    0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
    0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
    0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
    0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
    0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
    0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
    0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
    0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
    0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
    0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
    0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
    0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
    0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
    0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
    0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
    0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
    0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
    0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
    0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};

UINT16 crc_ccitt_1d0f( const unsigned char *in, unsigned char len)
{
  UINT16 crc;
  UINT16 tmp;
  unsigned char a;

  crc = 0x1D0F;

  for (a=0; a<len; a++) {
    tmp     = (crc >> 8) ^ ((UINT16)in[a] & 0xFF);
    crc     = (crc << 8) ^ crctab[tmp];
  }
  return crc;
}

void main ()
{
    /** Button timeout counter */
    UINT16 timeout = 0;
    
    UINT16 crc_tx;

    /** Storage for previous button capture. */
    unsigned char oldBtn = 0;
    unsigned char i;

    static unsigned char cnt = 0;
    
    PCON = 0x03;

    OSCCONbits.SCS = 0b10;
#if (_XTAL_FREQ == 8000000)
    OSCCONbits.IRCF = 0b1110;
#else
    #error unsupported clock frequency
#endif
    OSCCONbits.SPLLEN = 0;

    initPorts();
    initPWM();
    initInts();

    TX_Init();

    for (i = 0; i < PACKET_LENGTH; i++) {
        dataPacket[i] = i;
    }

//-- start timer0
    PSA=0;
    T0CS=0;
    PS2=1;
    PS1=1;
    PS0=1;

    // first read of buttons.
    readButtons();

    // Main application loop
    while (1)
    {
        // flash LEDs
        if ( btn & 0x0F )
        {
            i = 0;
            dataPacket[i++] = sync_bytes[0];
            dataPacket[i++] = sync_bytes[1];
            dataPacket[i++] = PACKET_LENGTH-3-2; // includes addrs, but not len or sync
            dataPacket[i++] = 0x11; // HeaderTo
            dataPacket[i++] = 0x22; // HeaderFrom
            dataPacket[i++] = 0x01; // HeaderId
            dataPacket[i++] = 0x00; // HeaderFlags

            dataPacket[i++] = 0x34;

            for (; i < PACKET_LENGTH-2; i++) {
              dataPacket[i] = cnt++;
            }

            crc_tx = crc_ccitt_1d0f(&(dataPacket[2]), PACKET_LENGTH-2-2);
            dataPacket[i++] = ~hi8(crc_tx);
            dataPacket[i++] = ~lo8(crc_tx);

            sendPacket();

            if (oldBtn == btn ) {
                // same buttons pressed, increment the timer
                // this timer increments after each data packet
                timeout++;
            } else {
                // new button pressed , reset the timer
                timeout = 0;
            }

            //sendPacket();
        }
        oldBtn = btn;
        LED = LED_ON;
        while (p_state > OFF)
        {
            // during a transmission read the button values
            // if a new button was pressed compose a new packet
            readButtons();
        }

        // if timeout ocours, force the button information to zero
        if ( timeout >= 50 )
        {
            timeout = 0;
            btn = 0;
        }

        // no button pressed, data packet sending finished
        if ( ((btn & 0x0F) == 0x00 ) && (p_state == OFF)  )
        {
            LED = LED_OFF;

            do {
                shutDown();
                __delay_ms(1);
                oldBtn = 0;

                readButtons();
            } while( (btn & 0x0F) == 0);
        }
    }    // end of while(1)
} // end of main
