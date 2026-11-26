#include <lpc21xx.h>
#include "header.h"

#define TRIG (1<<4)
#define ECHO (1<<5)

unsigned int distance_measure(void)
{
    unsigned int time, d;

    T0TCR = 0x02;
    T0PR = 59;       // Assuming PCLK = 60 MHz
    T0TC = 0;
    T0PC = 0;
    T0TCR = 0x01;

    IOCLR0 = TRIG;
    while(T0TC < 2);

    IOSET0 = TRIG;
    while(T0TC < 12);

    IOCLR0 = TRIG;

    T0TC = 0;
    T0PC = 0;

    while((IOPIN0 & ECHO) == 0)
    {
        if(T0TC > 30000)
        {
            T0TCR = 0x02;
            return 0xFFFF;
        }
    }

    T0TC = 0;
    T0PC = 0;

    while((IOPIN0 & ECHO) != 0)
    {
        if(T0TC > 30000)
        {
            T0TCR = 0x02;
            return 0xFFFF;
        }
    }

    time = T0TC;

    d = time / 58;

    T0TCR = 0x02;

    return d;
}
int main(void)
{
    CAN2_MSG m1;
    unsigned int distance;

    PINSEL0 &= ~(0xFF << 8);

    IODIR0 |= TRIG;
    IODIR0 &= ~ECHO;

    can2_init();
    uart0_init(115200);

    uart0_tx_string("ECU1 DISTANCE SENSOR STARTED\r\n");

    while(1)
    {
        distance = distance_measure();

        m1.id = 0x100;
        m1.rtr = 0;
        m1.dlc = 1;

        m1.byteA = distance;

        can2_tx(m1);

        delay_ms(500);
    }
}
