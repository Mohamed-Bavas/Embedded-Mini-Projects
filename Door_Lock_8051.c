#include <reg51.h>
#include <string.h>
#define LCD_DATA P2
sbit RS   = P3^0;
sbit RW   = P3^1;
sbit EN   = P3^2;
sbit BUZZ = P3^6;
sbit LOCK = P3^7;
#define PASS_LEN     4
#define MAX_ATTEMPTS 3
code char PASSWORD[PASS_LEN + 1] = "1234";
code char KEYS[4][4] = {
    {'7', '8', '9', '/'},
    {'4', '5', '6', '*'},
    {'1', '2', '3', '-'},
    {'C', '0', '=', '+'}
};
void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 123; j++);
}
void lcd_cmd(unsigned char cmd)
{
    LCD_DATA = cmd;
    RS = 0; RW = 0;
    EN = 1; delay_ms(1); EN = 0;
    delay_ms(2);
}
void lcd_data(unsigned char dat)
{
    LCD_DATA = dat;
    RS = 1; RW = 0;
    EN = 1; delay_ms(1); EN = 0;
    delay_ms(2);
}
void lcd_init(void)
{
    delay_ms(20);
    lcd_cmd(0x38);   
    lcd_cmd(0x0C);   
    lcd_cmd(0x06);  
    lcd_cmd(0x01);   
}
void lcd_str(char *s)
{
    while (*s) lcd_data(*s++);
}
void lcd_goto(unsigned char row, unsigned char col)
{
    lcd_cmd((row == 0 ? 0x80 : 0xC0) + col);
}
void lcd_clear_line(unsigned char row)
{
    unsigned char i;
    lcd_goto(row, 0);
    for (i = 0; i < 16; i++) lcd_data(' ');
    lcd_goto(row, 0);
}
char get_key(void)
{
    unsigned char r, c, col;
    while (1)
    {
        for (r = 0; r < 4; r++)
        {
            P1 = 0xF0 | (~(1 << r) & 0x0F);   
            col = (P1 >> 4) & 0x0F;
            if (col != 0x0F)                  
            {
                delay_ms(20);                 
                col = (P1 >> 4) & 0x0F;
                if (col == 0x0F) continue;
                for (c = 0; c < 4; c++)
                    if (!(col & (1 << c))) break;
                while (((P1 >> 4) & 0x0F) != 0x0F); 
                delay_ms(20);
                return KEYS[r][c];
            }
        }
    }
}
void beep(unsigned int ms)
{
    BUZZ = 1; delay_ms(ms); BUZZ = 0;
}
void unlock_door(void)
{
    unsigned char s;
    lcd_cmd(0x01);
    lcd_str("Access Granted");
    lcd_goto(1, 0);
    lcd_str("Door Open");
    LOCK = 1;
    beep(200);
    for (s = 0; s < 5; s++) delay_ms(1000);  
    LOCK = 0;
}
void lockout()
{
    unsigned char s;
    lcd_cmd(0x01);
    lcd_str("Too Many Tries!");
    lcd_goto(1, 0);
    lcd_str("Locked 10 sec");
    for (s = 0; s < 10; s++)
    {
        beep(300);
        delay_ms(700);
    }
}
void main()
{
    char entered[PASS_LEN + 1];
    unsigned char idx, attempts = 0;
    char key;
    LOCK = 0; BUZZ = 0;
    P1 = 0xFF;
    lcd_init();
    while (1)
    {
        lcd_cmd(0x01);
        lcd_str("Enter Password:");
        lcd_goto(1, 0);
        idx = 0;
        while (1)
        {
            key = get_key();
            if (key >= '0' && key <= '9' && idx < PASS_LEN)
            {
                entered[idx++] = key;
                lcd_data('*');
            }
            else if (key == 'C')           
            {
                idx = 0;
                lcd_clear_line(1);
            }
            else if (key == '=')
            {
                break;
            }
        }
        entered[idx] = '\0';
        if (idx == PASS_LEN && strcmp(entered, PASSWORD) == 0)
        {
            attempts = 0;
            unlock_door();
        }
        else
        {
            attempts++;
            lcd_cmd(0x01);
            lcd_str("Wrong Password");
            beep(500);
            delay_ms(1000);
            if (attempts >= MAX_ATTEMPTS)
            {
                attempts = 0;
                lockout();
            }
        }
    }
}