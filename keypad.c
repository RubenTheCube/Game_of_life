#include "keypad.h"
#include "portar.h"
//app_init för keyboard
/*void app_init( void ){
	// starta klockor port D och E
#ifdef USBDM
	* ((unsigned long *) 0x40023830) = 0x18;
#endif
	GPIO_D.moder &= 0x00000000;
	GPIO_D.moder |= 0x55005555;
	GPIO_D.otyper &= 0x00;
	GPIO_D.pupdr &= 0x0000FFFF;
	GPIO_D.pupdr |= 0xAAAA0000;
}*/
unsigned char segCodes[] = {0x3F,0x6,0x5B,0x4F,0x66,0x6D,0x7D,0x7,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
char keyValue[] = {1,2,3,0xA,4,5,6,0xB,7,8,9,0xC,0xE,0,0xF,0xD};

int readColumn( void ){
	char c = GPIO_D.idrHigh;
	if(c & 1) return 0;
	if(c & 2) return 1;
	if(c & 4) return 2;
	if(c & 8) return 3;
	return 0xFF;
}

void activateRow(unsigned int row){
	switch(row){
		case 0: GPIO_D.odrHigh = 0x10;break;
		case 1: GPIO_D.odrHigh = 0x20;break;
		case 2: GPIO_D.odrHigh = 0x40;break;
		case 3: GPIO_D.odrHigh = 0x80;break;
		default: GPIO_D.odrHigh = 0x00;break;
	}
	
}

unsigned char keyb( void ){

	int column = 0;
	for (int row = 0; row < 4;row++){
		activateRow(row);
		column = readColumn();
		if	(column != 0xFF)
			return keyValue[4*row+column];
	}
	return 0xFF;
}


void out7seg(unsigned char keyValue){

	if(keyValue - 16 < 0)
		GPIO_D.odrLow =segCodes[keyValue];
	else
		GPIO_D.odrLow = 0;
}

