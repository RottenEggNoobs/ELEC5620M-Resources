/*
 * DE1SoC_SevenSeg.c
 *
 *  Created on: 12 Feb 2021
 *      Author: Harry Clegg
 *      		You!
 */

#include "DE1SoC_SevenSeg.h"

//Hex Display addresses in a char(byte) container per display defined
volatile unsigned char *sevenseg_base_lo_ptr = (unsigned char *)0xFF200020;
volatile unsigned char *sevenseg_base_hi_ptr = (unsigned char *)0xFF200030;

// There are four HEX displays attached to the low (first) address.
#define SEVENSEG_N_DISPLAYS_LO 4

// There are two HEX displays attached to the high (second) address.
#define SEVENSEG_N_DISPLAYS_HI 2

void DE1SoC_SevenSeg_Write(unsigned int display, unsigned char value) {
    // Select between the two addresses so that the higher level functions
    // have a seamless interface.
    if (display < SEVENSEG_N_DISPLAYS_LO) {
        // If we are targeting a low address, use byte addressing to access
        // directly.
        sevenseg_base_lo_ptr[display] = value;
    } else {
        // If we are targeting a high address, shift down so byte addressing
        // works.
        display = display - SEVENSEG_N_DISPLAYS_LO;
        sevenseg_base_hi_ptr[display] = value;
    }
}

void DE1SoC_SevenSeg_SetSingle(unsigned int display, unsigned int value) {
    // Function turns a real value 0-F into the correctly encoded
    // bits to enable the correct segments on the seven segment display to
    // illuminate. 
	switch (value) {
		case 0x00:
			DE1SoC_SevenSeg_Write(display, 0x3F);
		break;
		
		case 0x1:
			DE1SoC_SevenSeg_Write(display, 0x06);
		break;
		
		case 0x2:
			DE1SoC_SevenSeg_Write(display, 0x5B);
		break;
		
		case 0x3:
			DE1SoC_SevenSeg_Write(display, 0x4F);
		break;
		
		case 0x4:
			DE1SoC_SevenSeg_Write(display, 0x66);
		break;
		
		case 0x5:
			DE1SoC_SevenSeg_Write(display, 0x6D);
		break;
		
		case 0x6:
			DE1SoC_SevenSeg_Write(display, 0x7D);
		break;
		
		case 0x7:
			DE1SoC_SevenSeg_Write(display, 0x07);
		break;
		
		case 0x8:
			DE1SoC_SevenSeg_Write(display, 0x7F);
		break;
		
		case 0x9:
			DE1SoC_SevenSeg_Write(display, 0x67);
		break;
		
		case 0xA:
			DE1SoC_SevenSeg_Write(display, 0x77);
		break;
		
		case 0xB:
			DE1SoC_SevenSeg_Write(display, 0x7C);
		break;
		
		case 0xC:
			DE1SoC_SevenSeg_Write(display, 0x39);
		break;
		
		case 0xD:
			DE1SoC_SevenSeg_Write(display, 0x5E);
		break;
		
		case 0xE:
			DE1SoC_SevenSeg_Write(display, 0x79);
		break;
		
		case 0xF:
			DE1SoC_SevenSeg_Write(display, 0x71);
		break;
		
		default:
			//when value exceeds 15 will loop in segments of 16 until value is in range
			//one line loop - Ultra optimized but disabled according to specification
			//DE1SoC_SevenSeg_SetSingle(display, (value - 16));
			DE1SoC_SevenSeg_Write(display, 0x40);
		break;
	}
}

void DE1SoC_SevenSeg_SetDoubleHex(unsigned int display, unsigned int value) {
    // This function will show the first digit of a HEXADECIMAL number on
    // the specified 'display', and the second digit on the display to
    // the left of the specified display.
	if (value < 0xFF) { //Limit Set
		DE1SoC_SevenSeg_SetSingle(display, (value & 0x0F));
		DE1SoC_SevenSeg_SetSingle( (display + 1) , (value >> 4) );
	} else {
		DE1SoC_SevenSeg_SetSingle(display, 16);
		DE1SoC_SevenSeg_SetSingle( (display + 1) , 16);
	}

    /** Some examples:
     *
     *    input | output | HEX(N+1) | HEX(N)
     *    ----- | ------ | -------- | ------
     *        5 |     05 |        0 |      5
     *       30 |     1E |        1 |      E
     *     0x60 |     60 |        6 |      0
     */
}

void DE1SoC_SevenSeg_SetDoubleDec(unsigned int display, unsigned int value) {
    
    // This function will show the first digit of a DECIMAL number on
    // the specified 'display', and the second digit on the display to
    // the left of the specified display.
	DE1SoC_SevenSeg_SetSingle(display, (value%10) );
	value = (value - (value%10)) / 10;
	DE1SoC_SevenSeg_SetSingle(display + 1 , value);

    /** Some examples:
     *
     *	  input | output | HEX(N+1) | HEX(N)
     *    ----- | ------ | -------- | ------
     *        5 |     05 |        0 |      5
     *       30 |     30 |        3 |      0
     *     0x5A |     90 |        9 |      0
     */
}

void DE1SoC_SevenSeg_SetSixDec(unsigned int value) {
	DE1SoC_SevenSeg_SetDoubleDec(0, value % 100); 
	DE1SoC_SevenSeg_SetDoubleDec(2, (value % 10000) / 100);
	DE1SoC_SevenSeg_SetDoubleDec(4, value / 10000);
}

void DE1SoC_SevenSeg_SetTimeFast(unsigned int value) {
	DE1SoC_SevenSeg_SetDoubleDec(0, value % 100);  //hundreth seconds
	DE1SoC_SevenSeg_SetDoubleDec(2, (value / 100) % 60); //seconds
	DE1SoC_SevenSeg_SetDoubleDec(4, (value / 6000) % 100); //minutes
}

void DE1SoC_SevenSeg_SetTime(unsigned int value) {
	DE1SoC_SevenSeg_SetDoubleDec(0, value % 60); // seconds
	DE1SoC_SevenSeg_SetDoubleDec(2, (value / 60) % 60); //minutes
	DE1SoC_SevenSeg_SetDoubleDec(4, (value / 3600) % 100); // hours
}
