#include <reg51.h>
sbit D1=P2^0;
sbit D2=P2^1;
sbit D3=P2^2;
sbit D4=P2^3;
sbit D5=P2^4;
sbit D6=P2^5;
unsigned char seg[]={
    0x3F,0x06,0x5B,0x4F,0x66,
    0x6D,0x7D,0x07,0x7F,0x6F
};
volatile unsigned char hr=12;
volatile unsigned char min=0;
volatile unsigned char sec=0;
volatile unsigned char digit=0;
volatile unsigned int ms_count=0;
void timer0_ISR(void) interrupt 1
{
    unsigned char ht,hu,mt,mu,st,su;
    TH0=0xFC;
    TL0=0x18;
    D1=1;
    D2=1;
    D3=1;
    D4=1;
    D5=1;
    D6=1;
    ht=hr/10;
    hu=hr%10;
    mt=min/10;
    mu=min%10;
    st=sec/10;
    su=sec%10;
    switch(digit)
    {
        case 0:
            P3=seg[ht];
            D1=0;
            break;
        case 1:
            P3=seg[hu];
            D2=0;
            break;
        case 2:
            P3=seg[mt];
            D3=0;
            break;
        case 3:
            P3=seg[mu];
            D4=0;
            break;
        case 4:
            P3=seg[st];
            D5=0;
            break;
        case 5:
            P3=seg[su];
            D6=0;
            break;
    }
    digit++;
    if(digit>=6)
        digit=0;
    ms_count++;
    if(ms_count>=1000)
    {
        ms_count=0;
        sec++;
        if(sec>=60)
        {
            sec=0;
            min++;
            if(min>=60)
            {
                min=0;
                hr++;
                if(hr>=24)
                    hr=0;
            }
        }
    }
}
void main()
{
    TMOD=0x01;
    TH0=0xFC;
    TL0=0x18;
    ET0=1;
    EA=1;
    TR0=1;
    while(1);
}
