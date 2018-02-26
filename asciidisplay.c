#include "asciidisplay.h"
#include "types.h"
#include "portar.h"
/*
*/
enum {B_RS = 1, B_RW = 2, B_SELECT = 4, B_E = 0x40};
//addressera ASCII-display och ettställ de bitar som är 1 i 0
void ascii_bit_set(uint8_t x){
	GPIO_E.odrLow |= (x | B_SELECT);
}

//addressera ASCII-display och nollställ de bitar som är 1 i x
void ascii_bit_clear(uint8_t x){
	GPIO_E.odrLow &= ~x;
	GPIO_E.odrLow |= B_SELECT;
}

void ascii_write_controller(uint8_t a_byte){
	ascii_bit_set(B_E);
	GPIO_E.odrHigh = a_byte;
	delay_250ns();
	ascii_bit_clear(B_E);
}

void ascii_write_cmd(uint8_t command){
	ascii_bit_clear(B_RS);
	ascii_bit_clear(B_RW);
	ascii_write_controller(command);
}

void ascii_write_data(uint8_t data){
	ascii_bit_set(B_RS);
	ascii_bit_clear(B_RW);
	ascii_write_controller(data);
}

uint8_t ascii_read_controller(void){
	ascii_bit_set(B_E);
	delay_250ns();
	delay_250ns();
	uint8_t rv = GPIO_E.idrHigh;
	ascii_bit_clear(B_E);
	delay_250ns();
	return rv;
}

uint8_t ascii_read_status(void){
	GPIO_E.moder &= 0x0000FFFF;
	ascii_bit_clear(B_RS);
	ascii_bit_set(B_RW);
	uint8_t rv = ascii_read_controller();
	GPIO_E.moder |= 0x55550000;
	return rv;
}

uint8_t ascii_read_data(void){
	GPIO_E.moder &= 0x0000FFFF;
	ascii_bit_set(B_RS);
	ascii_bit_set(B_RW);
	uint8_t rv = ascii_read_controller();
	GPIO_E.moder |= 0x55550000;
	return rv;
}

void ascii_init(void){
	while(ascii_read_status() & 0x80)
		;
	delay_mikro(8);
	ascii_write_cmd(0x3C);
	delay_mikro(39);
	
	while(ascii_read_status() & 0x80)
		;
	delay_mikro(8);
	ascii_write_cmd(0x0F);
	delay_mikro(39);
	
	while(ascii_read_status() & 0x80)
		;
	delay_mikro(8);
	ascii_write_cmd(0x0E);
	delay_mikro(39);
	
	while(ascii_read_status() & 0x80)
		;
	delay_mikro(8);
	ascii_write_cmd(0x04);
	delay_mikro(39);
}

void ascii_gotoxy (int x, int y){
	x--;
	if(y == 2)
		x += 0x40;
	ascii_write_cmd(0x80 | x);
}

void ascii_write_char(uint8_t c){
	while(ascii_read_status() & 0x80)
		;
	delay_mikro(8);
	ascii_write_data(c);
	delay_mikro(43);
}