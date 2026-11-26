
	#include <lpc21xx.h>
#include "header.h"
#include "lcd_header.h"
#include "can2_driver.c"
unsigned int led_state = 0;
unsigned int distance = 0;
unsigned int speed = 0;
unsigned int raw_speed = 0;
unsigned int brake_status = 0;
unsigned int buzzer_state = 0;
#define IN2 (1U << 9)
#define PWM_PIN (1U << 7)

#define PWM_NORMAL  950
#define PWM_WARNING 450
#define PWM_STOP     0
 void pwm_init(void)
{
    // P0.7 as PWM2
    PINSEL0 &= ~(3U << 14);
    PINSEL0 |=  (2U << 14);

    PWMPR = 59;

    PWMMR0 = 1000;
    PWMMR2 = 0;

    PWMMCR = (1U << 1);

    PWMPCR = (1U << 10);

    PWMLER = (1U << 0) | (1U << 2);

    PWMTCR = (1U << 0) | (1U << 3);
}
void motor_init(void)
{
    // P0.9 as GPIO
    PINSEL0 &= ~(3U << 18);

    IODIR0 |= IN2;

    IOCLR0 = IN2;

    pwm_init();
}
void pwm_duty(unsigned int duty)
{
    if(duty > 1000)
        duty = 1000;

    PWMMR2 = duty;
    PWMLER = (1U << 2);
}
void motor_forward(void)
{
    IOCLR0 = IN2;
}

void motor_brake(void)
{
    pwm_duty(0);
    IOCLR0 = IN2;
}


void led_init(void)
{
    PINSEL0 &= ~(3U << 20);  // P0.10 GPIO
    IODIR0 |= STATUS_LED;    // Output
    IOCLR0 = STATUS_LED;     // Initially OFF
}
void led_on(void)
{
    
	IOCLR0 = STATUS_LED;
}

void led_off(void)
{
    IOSET0 = STATUS_LED;
}
void led_blink(void)
{
    if(led_state == 0)
    {
        IOCLR0 = STATUS_LED;
        led_state = 1;
    }
    else
    {
        IOSET0 = STATUS_LED;
        led_state = 0;
    }

    delay_ms(250);
}
void buzzer_init(void)
{
    PINSEL1 &= ~(3U << 12);  // P0.22 GPIO
    IODIR0 |= BUZZER;
    IOCLR0 = BUZZER;
}

void buzzer_off(void)
{
    IOSET0 = BUZZER;
    buzzer_state = 0;
}

void buzzer_warning(void)
{
    if(buzzer_state == 0)
    {
        IOCLR0 = BUZZER;
        buzzer_state = 1;
    }
    else
    {
        IOSET0 = BUZZER;
        buzzer_state = 0;
    }

    delay_ms(250);
}

void buzzer_braking(void)
{
    IOCLR0 = BUZZER;
    buzzer_state = 1;
}
void display_number(unsigned int num)
{
    LCD_DATA((num / 100) + '0');
    LCD_DATA(((num / 10) % 10) + '0');
    LCD_DATA((num % 10) + '0');
}
   void calculate_speed(void)
{
    if(distance > 100)
    {
        speed = raw_speed;
    }
    else if(distance > 50)
    {
        speed = (raw_speed * (45 + ((distance - 51) * 55) / 49)) / 100;
    }
    else
    {
        speed = 0;
    }
}
void display_values(void)
{
    LCD_COMMAND(0x80);
    LCD_STR("D:   cm         ");

    LCD_COMMAND(0x82);
    display_number(distance);

    LCD_COMMAND(0xC0);
    LCD_STR("S:   km/h       ");

    LCD_COMMAND(0xC2);
    display_number(speed);
}



void motor_speed_control(void)
{
    unsigned int duty;

    if(speed == 0)
    {
        pwm_duty(0);
    }
    else
    {
        duty = 450 + ((speed - 45) * 500) / 55;

        if(speed < 45)
            duty = 450;

        if(duty > 950)
            duty = 950;

        pwm_duty(duty);
    }
}

  void automatic_braking(void)
{
    if(distance > 100)
    {
        brake_status = 0;

        motor_forward();
        motor_speed_control();

        led_off();
        led_state = 0;

        buzzer_off();

        uart0_tx_string("NORMAL: SAFE DISTANCE\r\n");
    }
    else if(distance > 50)
    {
        brake_status = 1;

        motor_forward();
        motor_speed_control();

        led_blink();
        buzzer_warning();

        uart0_tx_string("WARNING: OBSTACLE NEAR\r\n");
    }
    else
    {
        brake_status = 2;

        motor_brake();

        led_on();
        led_state = 1;

        buzzer_braking();

        uart0_tx_string("BRAKING: OBSTACLE TOO CLOSE\r\n");
    }
}
int main()
{
    CAN2_MSG m1;

    can2_init();
    LCD_INITIALIZATION();
    uart0_init(115200);
	led_init();
	buzzer_init();
    motor_init();

    distance = 0;
    speed = 0;

    LCD_COMMAND(0x80);
    LCD_STR("CAN BUS SYSTEM  ");
    LCD_COMMAND(0xC0);
    LCD_STR("INITIALIZING... ");

    delay_ms(1000);

    display_values();

   while(1)
{
    can2_rx(&m1);

    if(m1.rtr == 0)
    {
        if(m1.id == 0x100)
        {
            distance = m1.byteA & 0xFF;
        }

        else if(m1.id == 0x200)
        {
            raw_speed = m1.byteA & 0xFF;
        }

        calculate_speed();

        display_values();
        automatic_braking();
    }
}
}


