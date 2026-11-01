#include <reg51.h>
#define LCD P2
sbit RS = P2^0;
sbit EN = P2^1;
sbit ALERT = P2^2;
sbit ADC_RD = P3^7;
sbit ADC_WR = P3^6;
sbit ADC_INTR = P3^2;
unsigned char adc_value;
unsigned int temperature;
// Delay function
void delay(unsigned int time)
{
    unsigned int i, j;

    for(i = 0; i < time; i++)
    {
        for(j = 0; j < 1275; j++);
    }
}
// LCD Enable pulse
void lcd_enable(void)
{
    EN = 1;
    delay(1);
    EN = 0;
}
// Send command to LCD
void lcd_cmd(unsigned char cmd)
{
    RS = 0;
    LCD = (LCD & 0x0F) | (cmd & 0xF0);
    lcd_enable();
    LCD = (LCD & 0x0F) | (cmd << 4);
    lcd_enable();
    delay(2);
}
// Send data to LCD
void lcd_data(unsigned char dat)
{
    RS = 1;
    LCD = (LCD & 0x0F) | (dat & 0xF0);
    lcd_enable();
    LCD = (LCD & 0x0F) | (dat << 4);
    lcd_enable();
    delay(2);
}
// Initialize LCD
void lcd_init(void)
{
    delay(20);
    lcd_cmd(0x02);
    lcd_cmd(0x28);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x01);
}
// Display string
void lcd_string(char *str)
{
    while(*str)
    {
        lcd_data(*str);
        str++;
    }
}
// Display number
void lcd_number(unsigned int num)
{
    if(num >= 100)
        lcd_data((num / 100) + '0');
    if(num >= 10)
        lcd_data(((num / 10) % 10) + '0');
    lcd_data((num % 10) + '0');
}
// Read ADC0804
unsigned char adc_read(void)
{
    unsigned char value;
    ADC_RD = 1;
    ADC_WR = 1;
    // Start conversion
    ADC_WR = 0;
    delay(1);
    ADC_WR = 1;
    // Wait for conversion to complete
    while(ADC_INTR == 1);
    // Read ADC data
    ADC_RD = 0;
    delay(1);
    value = P1;
    ADC_RD = 1;
    return value;
}
void main(void)
{
    P1 = 0xFF;
    ADC_RD = 1;
    ADC_WR = 1;
    ADC_INTR = 1;
    ALERT = 0;
    lcd_init();
    lcd_cmd(0x80);
    lcd_string("Temperature:");
    while(1)
    {
        adc_value = adc_read();
        // With Vref/2 = 1.28V
        // ADC output approximately equals temperature in Celsius
        temperature = adc_value;
        lcd_cmd(0xC0);
        lcd_string("Temp: ");
        lcd_number(temperature);
        lcd_data(0xDF);
        lcd_data('C');
        lcd_string("   ");
        // High temperature alert
        if(temperature >= 40)
        {
            ALERT = 1;
        }
        else
        {
            ALERT = 0;
        }
        delay(100);
    }
}
