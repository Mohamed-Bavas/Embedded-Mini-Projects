#include <reg51.h>
sbit rs=P2^0;
sbit rw=P2^1;
sbit en=P2^2;
void delay(){
	unsigned int i,j;
	for(i=0;i<100;i++)
	 for(j=0;j<200;j++);
}
void lcd_cmd(unsigned char cmd){
	P3=cmd;
	rs=0;
	rw=0;
	en=1;
	delay();
	en=0;
}
void lcd_data(unsigned char DATA){
	P3=DATA;
	rs=1;
	rw=0;
	en=1;
	delay();
	en=0;
}
void lcd_string(char *str){
	while(*str){
		lcd_data(*str);
		str++;
	}
}
void lcd_init(){
	lcd_cmd(0x38);
	lcd_cmd(0x0C);
	lcd_cmd(0x06);
	lcd_cmd(0x01);
	delay();
}
void main(){ 
	while(1){
		lcd_init();
		lcd_cmd(0x08);
		lcd_string("Embedded System");
		lcd_cmd(0xC0);
		lcd_string("8056");
	}
}
