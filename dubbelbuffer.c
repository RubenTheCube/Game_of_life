#include "grafikdisplay.h"

uint8_t frame_buffer0[1024], frame_buffer1[1024];
uint8_t *front_buffer = frame_buffer0;
uint8_t *back_buffer = frame_buffer1;

void clear_buffer(uint8_t val){
	for(int i = 0; i < 1024; i++)
		back_buffer[i] = val;
}

void clear_buffers(void){
	for(int i = 0; i < 1024; i++)
		back_buffer[i] = front_buffer[i] = 0;
}

void swap_buffers(void){
	graphic_draw_screen();
	uint8_t *temp = front_buffer;
	front_buffer = back_buffer;
	back_buffer = temp;
}

void graphic_draw_screen(void){
	uint32_t k = 0;
	bool bupdate_addr = true;
	uint8_t controller;
	for(uint8_t j = 0; j < 8; j++){
		graphic_write_command(LCD_SET_PAGE | j, B_CS1);
		graphic_write_command(LCD_SET_ADD | 0,B_CS1);
		for(uint8_t i = 0; i < 64; i++,k++){
			if(back_buffer[k] != front_buffer[k]){
				if(bupdate_addr)
					graphic_write_command(LCD_SET_ADD | i, B_CS1);
				graphic_write_data(back_buffer[k], B_CS1);
				bupdate_addr = false;//auto inkrement så behöver inte öka adressen
			}else
				bupdate_addr = true;//fel adress för vi skrev inget på denna address
		}
		
		graphic_write_command(LCD_SET_PAGE | j, B_CS2);
		graphic_write_command(LCD_SET_ADD | 0, B_CS2);
		for(uint8_t i = 0; i < 64; i++,k++){
			if(back_buffer[k] != front_buffer[k]){
				if(bupdate_addr)
					graphic_write_command(LCD_SET_ADD | i, B_CS2);
				graphic_write_data(back_buffer[k], B_CS2);
				bupdate_addr = false;//auto inkrement så behöver inte öka adressen
			}else
				bupdate_addr = true;//fel adress för vi skrev inget på denna address
		}
	}
}

void pixel_dubbelbuffer(int x, int y){//0-indexerad!!
	if( (x > 127) || (x < 0) || (y > 63) || (y < 0) )
		return;
	
	uint8_t mask = 1<<(y%8);
	
	int index = 0;
	
	index = x + (y/8)*128;
	
	back_buffer[index] |= mask;
}

int check_neighbors(int x, int y){
	int rv = 0;
	int mask;
	int index = x + (y/8)*128; //detta är för den punkten som som ska checkas alltså mitten punkten
	//testar från övre vänstra hörn till nedre högre hörn genom att följa raderna vänster till höger
	int uppe = ((y % 8 ) == 0) ? 128 : 0;
	int nere  = ((y % 8 ) == 7) ? 128 : 0;


	mask = 1<<( ( y-1) % 8);
	if(front_buffer[index - uppe - 1]  & mask) 	rv++;//kollar uppe till vänster
	if(front_buffer[index - uppe]  & mask) 		rv++;//kollar uppe i mitten
	if(front_buffer[index  - uppe + 1]  & mask) rv++;//kollar uppe till höger

	mask = 1<<(y % 8);
	if(front_buffer[index - 1]  & mask) 		rv++;//kollar till vänster
	if(front_buffer[index + 1]  & mask) 		rv++;//kollar till höger

	mask = 1<<( (y + 1) % 8);
	if(front_buffer[index  + nere - 1]  & mask) rv++;//kollar nere till vänster
	if(front_buffer[index + nere ]  & mask) 	rv++;//kollar nere i mitten
	if(front_buffer[index +nere + 1]  & mask) 	rv++;//kollar nere till höger
			
	return rv;
}