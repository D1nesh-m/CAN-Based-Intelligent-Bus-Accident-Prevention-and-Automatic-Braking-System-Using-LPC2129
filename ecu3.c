#include <lpc21xx.h>
#include "header.h"

u32 adc_value;
u32 speed;

unsigned int ADC_Conversion(void)
{
    unsigned int ab;

    ADCR |= (1<<24);

    while((ADDR & 0x80000000U) == 0);

    ab = (ADDR >> 6) & 0x3FF;

    ADCR &= ~(7<<24);

    return ab;
}

int main()
{
    CAN2_MSG m1;

    // P0.28 as AD0.1
    PINSEL1 &= ~(3<<24);
    PINSEL1 |= (1<<24);

    // ADC configuration
    ADCR = 0x00200602;

    can2_init();

    while(1)
    {
        adc_value = ADC_Conversion();

        // Convert ADC reading to speed (0-100)
        speed = (adc_value * 100) / 1023;

        m1.id = 0x200;
        m1.rtr = 0;
        m1.dlc = 2;
        m1.byteA = speed;
        m1.byteB = 0;

        can2_tx(m1);

        delay_ms(500);
    }
}
