/*
 * Ultrasonic_to_measure_distance_project.c
 *
 * Created: 05/05/2024 03:03:28 
 *  Author: Hazem Ragab Elsayed
 */ 


#include <avr/io.h>
#define F_CPU 8000000UL
#include<util/delay.h>
#include<avr/interrupt.h>
#include"LCD.h"
#include"DIO.h"
#include"std_macros.h"
volatile unsigned short a,b,distance,high;


int main(void)
{
	LCD_vInit();
	DIO_vsetPINDir('D',7,1);
    while(1)
    {
		TCCR1A = 0;
		
		SET_BIT(TIFR,ICF1);
		DIO_write('D',7,1);
		_delay_us(30);
		DIO_write('D',7,0);
		_delay_us(200);
		TCCR1B = 0xc2;
		while((TIFR & 1<<ICF1) == 0);
		a = ICR1;
		TCCR1B = 0x82;
		SET_BIT(TIFR,ICF1);
		while((TIFR & 1<<ICF1) == 0);
		b = ICR1;
		TCNT1 = 0;
		TCCR1B = 0;
		if(b>a)
		{
			high = (b-a) ;
				
			// it was distance = high * 34500/(2*1000000) for simulation in protues but it is adjusted for Hardware;
			distance = high * 37900/(2*1000000);
			_delay_ms(400);
			if(distance < 10)
			{
				LCD_vSend_string("Distance :");
				LCD_vSend_char((distance%10)+48);
				LCD_vSend_string("cm");
				LCD_vSend_string("  ");
				
			}
			else if(distance<100 && distance < 80)
			{
				LCD_vSend_string("Distance :");
				LCD_vSend_char((distance/10)+48);
				LCD_vSend_char((distance%10)+48);
				LCD_vSend_string("cm");
				LCD_vSend_string(" ");
			}
			else if(distance >= 80)
			{
				LCD_clearscreen();
				LCD_vSend_string("No object");
			}
			/*else if (distance <1000)
			{
				LCD_vSend_char((distance/100)+48);
				LCD_vSend_char((distance %100/10)+48);
				LCD_vSend_char((distance%100%10)+48);
				LCD_vSend_string("cm");
			}*/
			else
			{
				
			}
			
			LCD_movecursor(1,1);
		}
		else
		{
			LCD_clearscreen();
			LCD_vSend_string("wrong");
		}		

    }
}
