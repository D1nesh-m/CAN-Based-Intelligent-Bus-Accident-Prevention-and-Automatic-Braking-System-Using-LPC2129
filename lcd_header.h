#ifndef __LCD_HEADER_H__


#define __LCD_HEADER_H__




#include <lpc21xx.h>


#include "delay.h"




#define LCD_D  (0x0F << 14)


#define RS     (1 << 12)


#define E      (1 << 13)




void LCD_INITIALIZATION(void);


void LCD_COMMAND(unsigned char);


void LCD_DATA(unsigned char);


void LCD_STR(unsigned char *);	   

void LCD_INITIALIZATION(void)
{
    PINSEL0 &= ~(0xFFU << 24);
    PINSEL1 &= ~(0x0F);

    IODIR0 |= LCD_D | RS | E;

    delay_ms(20);

    // LCD initialization for 4-bit mode
    IOCLR0 = LCD_D;
    IOSET0 = (0x03 << 14);
    IOCLR0 = RS;
    IOSET0 = E;
    delay_ms(5);
    IOCLR0 = E;

    delay_ms(1);

    IOCLR0 = LCD_D;
    IOSET0 = (0x03 << 14);
    IOSET0 = E;
    delay_ms(1);
    IOCLR0 = E;

    delay_ms(1);

    IOCLR0 = LCD_D;
    IOSET0 = (0x03 << 14);
    IOSET0 = E;
    delay_ms(1);
    IOCLR0 = E;

    delay_ms(1);

    IOCLR0 = LCD_D;
    IOSET0 = (0x02 << 14);
    IOSET0 = E;
    delay_ms(1);
    IOCLR0 = E;

    // Now LCD is in 4-bit mode
    LCD_COMMAND(0x28);
    LCD_COMMAND(0x0C);
    LCD_COMMAND(0x01);
    LCD_COMMAND(0x02);
}




void LCD_COMMAND(unsigned char cmd)


{


    IOCLR0 = LCD_D;




    IOSET0 = (cmd & 0xF0) << 10;




    IOCLR0 = RS;


    IOSET0 = E;




    delay_ms(2);




    IOCLR0 = E;




    IOCLR0 = LCD_D;




    IOSET0 = (cmd & 0x0F) << 14;




    IOCLR0 = RS;


    IOSET0 = E;




    delay_ms(2);




    IOCLR0 = E;


}




void LCD_DATA(unsigned char data)


{


    IOCLR0 = LCD_D;




    IOSET0 = (data & 0xF0) << 10;




    IOSET0 = RS;


    IOSET0 = E;




    delay_ms(2);




    IOCLR0 = E;




    IOCLR0 = LCD_D;




    IOSET0 = (data & 0x0F) << 14;

    IOSET0 = RS;
    IOSET0 = E;

    delay_ms(2);

    IOCLR0 = E;
}

void LCD_STR(unsigned char *s)
{
    while(*s)

    {
        LCD_DATA(*s++);
    }
}




#endif
