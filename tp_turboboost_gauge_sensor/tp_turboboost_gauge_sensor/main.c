/*
 * TP_HUD_PRESSURE_GUAGE_ti85ADC.c
 *
 * Created: 30.10.2022 13:31:06
 * Author : ss201040303
 */ 

/* Description : 
 * This is the Code for TP HUD MAP Gauge ADC Converter.
 * Following Pin define (as PDID/SOIC):
 * PINOUT		PIN
 * VCC(5v)		1
 * PA7(CS)		6
 * PA6(MOSI/DI)	7
 * PA5(MISO/DO)	8
 * PA4(USCK)	9
 * PA1(SESNOR)	12
 * GND			14
 */

#define CLK		4	//!< USI clock I/O pin.
#define DI		6	//!< USI data input pin.
#define DO		5	//!< USI data output pin.
#define CS		7


#include <avr/io.h>
#include <avr/interrupt.h>


int adcBufferH = 0xFF;
int adcBudderL = 0xFF;

int main(void)
{
    /* Replace with your application code */
	//I/O Configuration
	DDRA |= (1 << DO);
	PORTA |= (1 << CLK)|(1 << DI)|(1 << CS);
	
	//USI (SPI)
	USICR = 0x18;  	//USIWM0 -> Bit 4, USICS1 -> Bit3
	
	//Pin Change Interrupt
	GIMSK |= (1 << PCIE0);
	PCMSK0 |= (1 << PCINT7);
	
	//ADC Config.
	DIDR0 |= (1 << ADC1D);
	ADCSRA = (1 << ADEN)|(1 << ADSC)|(1 << ADATE);
	
	//MUX Select PA1
	ADMUX &= ~(0x3F);
	ADMUX |= 0x01;
	
	//Enable Interrupt
	sei();
	
	
    while (1) 
    {
		if( ( (ADCSRA & (1 << ADIF)) >>  4) == 1){
			adcBufferH = ADCH;
			adcBudderL = ADCL;
		}
    }
}

ISR(PCINT0_vect){
	

	if( (PINA & 0x80)== 0 ){
		USICR |= (1<<USIOIE);
		USISR = 1<<USIOIF;      // Clear Overflow bit
		USIDR = adcBufferH;
		
		}else{
		USICR &= ~(1<<USIOIE);
	}
	

}

ISR(USI_OVF_vect)
{
	USIDR = adcBudderL;
	USISR = 1<<USIOIF;
}