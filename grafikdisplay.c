 #include "grafikdisplay.h"
 #include "portar.h"
 #include "delay.h"
 
 
 #define LCD_ON 0x3F
 #define LCD_OFF 0x3E
 #define LCD_SET_ADD 0x40
 #define LCD_SET_PAGE 0xB8
 #define LCD_DISP_START 0xC0
 #define LCD_BUSY 0x80
 
 #define B_E 0x40
 #define B_SELECT 4
 #define B_RW 2
 #define B_RS 1
 #define B_RST 0x20
 #define B_CS2 0x10
 #define B_CS1 0x8
 

//välj grafik-display och ettställ de bitar som är 1 i x
void graphic_ctrl_bit_set(uint8_t x){
	GPIO_E.odrLow |= ( ~B_SELECT & x );
}

//välj grafik display och nollställ de bitar som är 1 i x
void graphic_ctrl_bit_clear(uint8_t x){
	GPIO_E.odrLow &= ( ~B_SELECT & ~x );
}

//konfigurera cs-signalerna gör så vi alltid addresserar grifikdisplay ochskåp med att släcka B_SELECT 
void select_controller(uint8_t controller){
	switch(controller){
		//slå av cs1 och 2 
		case 0: GPIO_E.odrLow &= ~(B_CS1 | B_CS2 | B_SELECT); break;
		//slå på CS1 och slå av CS2
		case B_CS1: GPIO_E.odrLow |= B_CS1; 
		GPIO_E.odrLow &= ~(B_CS2 | B_SELECT); break;
		//slå av CS1 och slå på CS2 
		case B_CS2: GPIO_E.odrLow &= ~(B_CS1 | B_SELECT); 
		GPIO_E.odrLow |= B_CS2; break;
		//slå på båda
		case B_CS1 | B_CS2: GPIO_E.odrLow |= (B_CS1 | B_CS2);
		GPIO_E.odrLow &= ~B_SELECT; break;
	}
}

void graphic_wait_ready(void){
	uint8_t is_busy = 0;
	graphic_ctrl_bit_clear(B_E);
	GPIO_E.moder = 0x00005555;
	graphic_ctrl_bit_clear(B_RS);
	graphic_ctrl_bit_set(B_RW);
	delay_500ns();
	while(1){
		graphic_ctrl_bit_set(B_E);
		delay_500ns();
		is_busy = GPIO_E.idrHigh & 0x80;
		graphic_ctrl_bit_clear(B_E);
		delay_500ns();
		if(is_busy == 0 )
			break;
	}
	graphic_ctrl_bit_set(B_E);
	GPIO_E.moder = 0x55555555;
}

unsigned char graphic_read(unsigned char controller){
	graphic_ctrl_bit_clear(B_E);
	GPIO_E.moder = 0x00005555;
	graphic_ctrl_bit_set(B_RS | B_RW);
	select_controller(controller);
	delay_500ns();
	graphic_ctrl_bit_set(B_E);
	delay_500ns();
	uint8_t rv = GPIO_E.idrHigh;
	graphic_ctrl_bit_clear(B_E);
	GPIO_E.moder = 0x55555555;
	if(controller & B_CS1){
		select_controller(B_CS1);
		graphic_wait_ready();
	}
	if(controller & B_CS2){
		select_controller(B_CS2);
		graphic_wait_ready();
	}
	return rv;
}
void graphic_write(uint8_t value, uint8_t controller){
	GPIO_E.odrHigh = value;
	select_controller(controller);
	delay_500ns();
	graphic_ctrl_bit_set(B_E);
	delay_500ns();
	graphic_ctrl_bit_clear(B_E);
	
	if(controller & B_CS1){
		select_controller(B_CS1);
		graphic_wait_ready();
	}
	if(controller & B_CS2){
		select_controller(B_CS2);
		graphic_wait_ready();
	}
	GPIO_E.odrHigh = 0;
	graphic_ctrl_bit_set(B_E);
	select_controller(0);
}

void graphic_write_command(uint8_t command, uint8_t controller){
	graphic_ctrl_bit_clear(B_E);
	select_controller(controller);
	graphic_ctrl_bit_clear(B_RS | B_RW);
	graphic_write(command,controller);
}

void graphic_write_data(uint8_t data, uint8_t controller){
	graphic_ctrl_bit_clear(B_E);
	select_controller(controller);
	graphic_ctrl_bit_set(B_RS);
	graphic_ctrl_bit_clear(B_RW);
	graphic_write(data,controller);
}

uint8_t graphic_read_data(uint8_t controller){
	(void) graphic_read(controller);
	return graphic_read(controller);
}

void graphic_initalize(void){
	graphic_ctrl_bit_set(B_E);
	delay_mikro(10);
	graphic_ctrl_bit_clear(B_CS1 | B_CS2 | B_RST | B_E);
	delay_milli(30);
	graphic_ctrl_bit_set(B_RST);
	graphic_write_command(LCD_OFF,B_CS1 | B_CS2);
	graphic_write_command(LCD_ON,B_CS1 | B_CS2);
	graphic_write_command(LCD_DISP_START,B_CS1 | B_CS2);
	graphic_write_command(LCD_SET_ADD,B_CS1 | B_CS2);
	graphic_write_command(LCD_SET_PAGE,B_CS1 | B_CS2);
	select_controller(0);
}

void graphic_clear_screen(void){
	for (int page = 0;page < 8;page++){
		graphic_write_command(LCD_SET_PAGE | page, B_CS1 | B_CS2);
		graphic_write_command(LCD_SET_ADD | 0   , B_CS1 | B_CS2);
		for(int add = 0;add < 64;add++)
			graphic_write_data(0, B_CS1 | B_CS2);
	}
}

void pixel(int x, int y, int set){
	if( (x < 1) || (y < 1) || (x > 128) || (y > 64))return;
	uint8_t mask, controller, c;
	int index = (y-1)/8;
	//mask = 1 << (y-1)%8;
	switch((y-1) %8){
		case 0: mask =1;break;
		case 1: mask =2;break;
		case 2: mask =4;break;
		case 3: mask =8;break;
		case 4: mask =0x10;break;
		case 5: mask =0x20;break;
		case 6: mask =0x40;break;
		case 7: mask =0x80;break;
	}
	if( set == 0)
		mask = ~mask;
	if(x > 64){
		controller = B_CS2;
		x = x - 65;
	}
	else{
		controller = B_CS1;
		x = x - 1;
	}
	graphic_write_command(LCD_SET_ADD | x, controller);
	graphic_write_command(LCD_SET_PAGE | index, controller);
	c = graphic_read_data(controller);
	graphic_write_command(LCD_SET_ADD | x, controller);
	if(set == 1)
		mask |= c;
	else
		mask &=  c;
	graphic_write_data(mask,controller);
	graphic_write_command(LCD_ON,B_CS1 | B_CS2);
}