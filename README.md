# PIC_RFM69
Send packet from PIC12LF1840T39A to RFM69HCW

This code sends a packet from the PIC to the HopeRF module as used on the Adafruit Feather M0.

It sends it in a way that the RFM69 can handle natively, including CRC check.

With this example it should be easy to send from the very inexpensive and powerful PIC chip to the ubiquitous HopeRF RFM69 module.


PIC Code is based on the Microchip Keeloq examples.

## Important Changes from the original example code

### rf69_recv/rf69_recv.ino
A configuration that is compatible is loaded in setup()
After that, the Radiohead library can be used as usual.

### PIC_TEST/main.c
- The bit order sent over the air is reversed.
- Manchester encoding is used
- The preamble and sync bits are NOT manchester encoded
- The CRC used by the RFM69 is generated
- It generates a packet that is compatible with the Radiohead library

## The Hardware

PIC Board: 
https://oshpark.com/shared_projects/H4lJ3ypO

![Board Image](https://github.com/CryptoThings/PIC_RFM69/blob/master/IMG_5769.jpg)


