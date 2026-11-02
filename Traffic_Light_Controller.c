#include <reg51.h>
// Road 1 LEDs
sbit R1 = P1^0;
sbit Y1 = P1^1;
sbit G1 = P1^2;
// Road 2 LEDs
sbit R2 = P1^3;
sbit Y2 = P1^4;
sbit G2 = P1^5;
// Delay function
void delay(unsigned int time)
{
    unsigned int i, j;
    for(i = 0; i < time; i++)
    {
        for(j = 0; j < 112; j++);
    }
}
// All signals OFF
void all_off(void)
{
    R1 = 0;
    Y1 = 0;
    G1 = 0;

    R2 = 0;
    Y2 = 0;
    G2 = 0;
}
// Road 1 GREEN, Road 2 RED
void road1_green(void)
{
    all_off();

    G1 = 1;
    R2 = 1;

    delay(500);
}
// Road 1 YELLOW, Road 2 RED
void road1_yellow(void)
{
    all_off();

    Y1 = 1;
    R2 = 1;

    delay(200);
}
// Road 1 RED, Road 2 GREEN
void road2_green(void)
{
    all_off();

    R1 = 1;
    G2 = 1;

    delay(500);
}
// Road 1 RED, Road 2 YELLOW
void road2_yellow(void)
{
    all_off();

    R1 = 1;
    Y2 = 1;

    delay(200);
}
// Main program
void main(void)
{
    P1 = 0x00;

    while(1)
    {
        road1_green();

        road1_yellow();

        road2_green();

        road2_yellow();
    }
}
