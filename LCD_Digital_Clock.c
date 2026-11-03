#include <reg51.h>
#define LCD_PORT P2
// LCD connections
sbit RS = P2^0;
sbit EN = P2^1;
// Push buttons
sbit HOUR_BTN = P3^0;
sbit MIN_BTN  = P3^1;
// Clock variables
volatile unsigned int ms_count = 0;
volatile unsigned char hours = 12;
volatile unsigned char minutes = 0;
volatile unsigned char seconds = 0;
// Delay function
void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for(i = 0; i < ms; i++)
    {
        for(j = 0; j < 112; j++);
    }
}
// LCD enable pulse
void lcd_enable(void)
{
    EN = 1;
    delay_ms(1);
    EN = 0;
}
// Send 4 bits to LCD
void lcd_nibble(unsigned char value)
{
    LCD_PORT = (LCD_PORT & 0x0F) | (value & 0xF0);
    lcd_enable();
}
// LCD command
void lcd_cmd(unsigned char cmd)
{
    RS = 0;
    lcd_nibble(cmd);
    lcd_nibble(cmd << 4);
    delay_ms(2);
}
// LCD data
void lcd_data(unsigned char dat)
{
    RS = 1;
    lcd_nibble(dat);
    lcd_nibble(dat << 4);
    delay_ms(2);
}
// LCD initialization
void lcd_init(void)
{
    delay_ms(20);
    RS = 0;
    EN = 0;
    lcd_nibble(0x30);
    delay_ms(5);
    lcd_nibble(0x30);
    delay_ms(1);
    lcd_nibble(0x30);
    delay_ms(1);
    lcd_nibble(0x20);
    lcd_cmd(0x28);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x01);
}
// Display character string
void lcd_string(char *str)
{
    while(*str)
    {
        lcd_data(*str);
        str++;
    }
}
// Display two digit number
void lcd_number(unsigned char num)
{
    lcd_data((num / 10) + '0');
    lcd_data((num % 10) + '0');
}
// Timer0 initialization
void timer0_init(void)
{
    TMOD = 0x01;
    // Timer0 reload value for approximately 1 ms
    TH0 = 0xFC;
    TL0 = 0x66;
    ET0 = 1;
    EA = 1;
    TR0 = 1;
}
// Timer0 interrupt
void timer0_ISR(void) interrupt 1
{
    TH0 = 0xFC;
    TL0 = 0x66;
    ms_count++;
    if(ms_count >= 1000)
    {
        ms_count = 0;
        seconds++;
        if(seconds >= 60)
        {
            seconds = 0;
            minutes++;
            if(minutes >= 60)
            {
                minutes = 0;
                hours++;
                if(hours >= 24)
                {
                    hours = 0;
                }
            }
        }
    }
}
// Display clock
void display_time(void)
{
    lcd_cmd(0x80);
    lcd_string("DIGITAL CLOCK");
    lcd_cmd(0xC0);
    lcd_number(hours);
    lcd_data(':');
    lcd_number(minutes);
    lcd_data(':');
    lcd_number(seconds);
    lcd_string("   ");
}
// Set hour
void set_hour(void)
{
    EA = 0;
    hours++;
    if(hours >= 24)
    {
        hours = 0;
    }
    EA = 1;
}
// Set minute
void set_minute(void)
{
    EA = 0;
    minutes++;
    if(minutes >= 60)
    {
        minutes = 0;
    }
    EA = 1;
}
// Main function
void main(void)
{
    HOUR_BTN = 1;
    MIN_BTN = 1;
    lcd_init();
    timer0_init();
    while(1)
    {
        display_time();
        // Hour button
        if(HOUR_BTN == 0)
        {
            delay_ms(20);
            if(HOUR_BTN == 0)
            {
                set_hour();
                while(HOUR_BTN == 0);
            }
        }
        // Minute button
        if(MIN_BTN == 0)
        {
            delay_ms(20);
            if(MIN_BTN == 0)
            {
                set_minute();
                while(MIN_BTN == 0);
            }
        }
    }
}
