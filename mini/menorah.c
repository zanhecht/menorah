/*
menorah.c

Written by Windell Oskay, http://www.evilmadscientist.com/
Modified by Zan Hecht, http://www.zansstuff.com/

This is free software; please modify it and use it to do cool stuff.

This avr-gcc program for the Atmel ATTiny2313 
runs a very tiny hanukkah menorah that lites from 2 - 9 LED "candles."

 http://www.evilmadscientist.com/2006/how-to-make-high-tech-led-decorations-for-the-holidays/
 
The number of candles displayed is stored in EEPROM and is
incremented each time that the unit is reset.

Version 2.0   Last Modified:  12/1/2008. 
Now extra spiffy, with better LED uniformity, higher refresh rate, and lower power usage. :D
 
Version 3.0   Last Modified: 1/8/2023. 
Candles are lit in correct order (newest to oldest) each night. 
 
*/

#include <avr/io.h>
#include <avr/eeprom.h> 

#define shortdelay(); 			asm("nop\n\t" \ 
								"nop\n\t");

uint16_t eepromWord __attribute__((section(".eeprom")));

int main (void)
	{
	uint8_t PB, PD;		//8-bit unsigned integers
	uint8_t PortBData[9] = {128U,192U,224U,240U,244U,246U,247U,247U,255U};	//uint8_t multiplexed;
	uint8_t PortDData[9] = {0,0,0,0,0,0,0,0,0};

	unsigned int day, position, i;				//16-bit unsigned integers

	//Initialization routine: Clear watchdog timer-- this can prevent several things from going wrong.		
	MCUSR &= 0xF7;		//Clear WDRF Flag
	WDTCSR	= 0x18;		//Set stupid bits so we can clear timer...
	WDTCSR	= 0x00;

	//Data direction register: DDRD
	//Set all ports to output *EXCEPT* PA2 (reset pin)
	//Only IO lines D6 and B0-B7 will be used for output.
		DDRA = 3U;
		DDRB = 255U;	
		DDRD = 127U;

	// Read string number from EEPROM data memory. If good, increment it and save
	// the new value to EEPROM.  This allows us to cycle through the set of strings in flash, 
	// using an new one each time that the unit is reset.  

	if ( eeprom_read_word(&eepromWord) == 0xffff)
		day = 0U;
	else	
		day = (uint8_t) (eeprom_read_word(&eepromWord)) ;
		
	day++;
		
	if (day > 7)
		day = 0U;

	eeprom_write_word(&eepromWord, (uint8_t) day);

	PORTA = 0;
	PORTB = 0;
	PORTD = 0;

	/* Visualize outputs:

				B7 B6 B5 B4 B3 B2 B1 B0 D6		B value	D value

	Day 0	0  0  0  0  0  0  0  0  1		0			64
	Day 0	0  0  0  0  1  0  0  0  1		8			64

	Day 1	0  0  0  0  0  0  0  1  0		1			0
	Day 1	0  0  0  0  0  0  0  1  1		1			64
	Day 1	0  0  0  0  1  0  0  1  1		9			64

	Day 2	0  0  0  0  0  0  1  0  0		2			0
	Day 2	0  0  0  0  0  0  1  1  0		3			0
	Day 2	0  0  0  0  0  0  1  1  1		3			64
	Day 2	0  0  0  0  1  0  1  1  1		11			64

	Day 3	0  0  0  0  0  1  0  0  0		4			0
	Day 3	0  0  0  0  0  1  1  0  0		6			0
	Day 3	0  0  0  0  0  1  1  1  0		7			0
	Day 3	0  0  0  0  0  1  1  1  1		7			64
	Day 3	0  0  0  0  1  1  1  1  1		15			64

	Day 4	0  0  0  1  0  0  0  0  0		16			0
	Day 4	0  0  0  1  0  1  0  0  0		20			0
	Day 4	0  0  0  1  0  1  1  0  0		22			0
	Day 4	0  0  0  1  0  1  1  1  0		23			0
	Day 4	0  0  0  1  0  1  1  1  1		23			64
	Day 4	0  0  0  1  1  1  1  1  1		31			64

	Day 5	0  0  1  0  0  0  0  0  0		32			0
	Day 5	0  0  1  1  0  0  0  0  0		48			0
	Day 5	0  0  1  1  0  1  0  0  0		52			0
	Day 5	0  0  1  1  0  1  1  0  0		54			0
	Day 5	0  0  1  1  0  1  1  1  0		55			0
	Day 5	0  0  1  1  0  1  1  1  1		55			64
	Day 5	0  0  1  1  1  1  1  1  1		63			64

	Day 6	0  1  0  0  0  0  0  0  0		64			0
	Day 6	0  1  1  0  0  0  0  0  0		96			0
	Day 6	0  1  1  1  0  0  0  0  0		112		0
	Day 6	0  1  1  1  0  1  0  0  0		116		0
	Day 6	0  1  1  1  0  1  1  0  0		118		0
	Day 6	0  1  1  1  0  1  1  1  0		119		0
	Day 6	0  1  1  1  0  1  1  1  1		119		64
	Day 6	0  1  1  1  1  1  1  1  1		127		64

	Day 7	1  0  0  0  0  0  0  0  0		128		0
	Day 7	1  1  0  0  0  0  0  0  0		192		0
	Day 7	1  1  1  0  0  0  0  0  0		224		0
	Day 7	1  1  1  1  0  0  0  0  0		240		0
	Day 7	1  1  1  1  0  1  0  0  0		244		0
	Day 7	1  1  1  1  0  1  1  0  0		246		0
	Day 7	1  1  1  1  0  1  1  1  0		247		0
	Day 7	1  1  1  1  0  1  1  1  1		247		64
	Day 7	1  1  1  1  1  1  1  1  1		255		64

	Pin D6 high when Day 1 candle is lit, other D pins are always low. 2^6 = 64, so D = 64U

	PORTB: B3 represents shamash and is always high only in final case.

	*/
	// set first 7 candles (plus shamash) depending on the day
	switch(day) {
		case 0 :
			PortBData[0] = 0;
			PortBData[1] = 8U;
			break;
		case 1 :
			PortBData[0] = 1U;
			PortBData[1] = 1U;
			PortBData[2] = 9U;
			break;
		case 2 :
			PortBData[0] = 2U;
			PortBData[1] = 3U;
			PortBData[2] = 3U;
			PortBData[3] = 11U;
			break;
		case 3 :
			PortBData[0] = 4U;
			PortBData[1] = 6U;
			PortBData[2] = 7U;
			PortBData[3] = 7U;
			PortBData[4] = 15U;
			break;
		case 4 :
			PortBData[0] = 16U;
			PortBData[1] = 20U;
			PortBData[2] = 22U;
			PortBData[3] = 23U;
			PortBData[4] = 23U;
			PortBData[5] = 31U;
			break;
		case 5 : 
			PortBData[0] = 32U;
			PortBData[1] = 48U;
			PortBData[2] = 52U;
			PortBData[3] = 54U;
			PortBData[4] = 55U;
			PortBData[5] = 55U;
			PortBData[6] = 63U;
			break;
		case 6 :
			PortBData[0] = 64U;
			PortBData[1] = 96U;
			PortBData[2] = 112U;
			PortBData[3] = 116U;
			PortBData[4] = 118U;
			PortBData[5] = 119U;
			PortBData[6] = 119U;
			PortBData[7] = 127U;
		// case 7 is the default value described above by
		// uint8_t PortBData[9] = {128U,192U,224U,240U,244U,246U,247U,247U,255U};
	}
	
	// turn on last candle for last two positions
	PortDData[day] = 64U;
	PortDData[day+1] = 64U;

	position = 0;
	for (;;)  // main loop
		{

		PB = PortBData[position];
		PD = PortDData[position];
		
		i = 0;
		while (i < 15000)		// This sets how long it is between changing number of "candles"
			{

			//Multiplexing routine: Each LED is on (1/9) of the time. 
			//  -> Uses much less power.
			
			PORTD = PD & (64U);
			shortdelay();
			PORTD = 0;

			PORTB = PB & (1U);
			shortdelay();
			PORTB = 0;

			PORTB = PB & (2U);
			shortdelay();
			PORTB = 0;

			PORTB = PB & (4U);
			shortdelay();
			PORTB = 0;

			PORTB = PB & (8U);
			shortdelay();
			PORTB = 0;

			PORTB = PB & (16U);
			shortdelay();
			PORTB = 0;

			PORTB = PB & (32U);
			shortdelay();
			PORTB = 0;

			PORTB = PB & (64U);
			shortdelay();
			PORTB = 0;

			PORTB = PB & (128U);
			shortdelay();
			PORTB = 0;

			i++;
			} 
			
		if ( position < (day + 1) )
			position++;
		
		}	//End main loop
		return 0;
	}
