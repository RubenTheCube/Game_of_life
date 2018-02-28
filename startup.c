/*
 * 	startup.c
 *
 */

#include "grafikdisplay.h"
#include "keypad.h"
#include "portar.h"
#include "objects.h"
 
void startup(void) __attribute__((naked)) __attribute__((section (".start_section")) );

void startup ( void )
{
__asm volatile(
	" LDR R0,=0x2001C000\n"		/* set stack */
	" MOV SP,R0\n"
	" BL main\n"				/* call main */
	"_exit: B .\n"				/* never return */
	) ;
}

void init_app(void)
{
	GPIO_E.moder = 0x55555555;//0-15 utgångar
	GPIO_E.otyper = 0x00000000; 	// outputs are push/pull 
	GPIO_E.ospeedr = 0x55555555; // medium speed 
	GPIO_E.pupdr = 0x55550000; 
	
	
	GPIO_D.moder = 0x55005555;
	GPIO_D.otyper &= 0xFFFF00FF;//otyper
	GPIO_D.otyper |= 0x00000000;
	
	GPIO_E.pupdr &= 0x0000FFFF;//pupdr
	GPIO_E.pupdr |= 0x00AA0000;
	
}

#define gridx 128
#define gridy 64
#define glider_gunx 38
#define glider_guny 15

PTR_OBJ mark = &marker;
uint8_t temp_grid[gridx][gridy];			   //1					   3					 5					   7
int glider_gun[glider_guny][glider_gunx] = {{0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,1,1, 0,0,0,0,0, 0,0,0,0,1, 1,0,0},//1
											{0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,1, 0,0,0,0,0, 0,0,0,0,1, 1,0,0},
											{1,1,0,0,0, 0,0,0,0,1, 1,0,0,0,0, 0,0,0,0,0, 0,0,1,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0},
											{1,1,0,0,0, 0,0,0,1,0, 1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0},
											{0,0,0,0,0, 0,0,0,1,1, 0,0,0,0,0, 0,1,1,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0},//5
											{0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,1,0,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0},
											{0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,1,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0},
											{0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,1,0},
											{0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,0,1},
											{0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,0,0},//10
											{0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0},
											{0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0},
											{0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1, 1,1,0,0,0, 0,0,0,0,0, 0,0,0},
											{0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1, 0,0,0,0,0, 0,0,0,0,0, 0,0,0},
											{0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,0,0,0,0, 0,0,0,0,0, 0,0,0}//15
};

//int grid[gridx][gridy], gridbuffer[gridx][gridy];

int main(int argc, char **argv)
{
	//init
	mark->posx = 4;
	mark->posy = 40;
	init_app();
	graphic_initalize();
	ascii_init();
	ascii_write_cmd(1);
	graphic_clear_screen();
	clear_buffers();
	clear_grid();
	/*
	//skriv intro till spelet
	char *s;
	char skapare[] = "Rob's & Mr.O's";
	char titel[] = "Game of life";

	ascii_gotoxy(1,1);
	s = skapare;
	while(*s)
		ascii_write_char(*s++);
	ascii_gotoxy(1,2);
	s = titel;
	while(*s)
		ascii_write_char(*s++);
	delay_milli(1000);
	while(ascii_read_status() & 0x80)
			;
	delay_mikro(8);
	delay_milli(4000);
	 */

	//int shapex = 3, shapey = 3;
	//int shape[3][3] = {	{1,0,0},
	//						{0,1,1},{1,1,0}};
						
	//clear_grid();
	//cursor_mode();
				
/*
	for(int i = 1; i <= shapex; i++){		//copy shape to grid
		for(int j = 1; j <= shapey; j++){
			grid[i][j] = shape[j-1][i-1]; 	//this needs to be this wonky
		}
	}
*/
	
	int rv = 0;
	int delay = 0;
	bool looping;
	while(1){								//main loop
		looping = true;
		cursor_mode();
		clear_buffers();
		graphic_clear_screen();
		print_grid();
		swap_buffers();
		while(looping){						//simulator loop
			clear_buffer(0);
			for(int i = 0; i < 127; i++){
				for(int j = 0; j < 63; j++){
					rv = check_neighbors(i,j);
					if(rv == 3 || rv == 12 || rv == 13) //3 i alla fall, 12/13 pupulated med 2/3 grannar
						pixel_dubbelbuffer(i,j);
				}
			}
			switch (keyb()){
				case 3: looping = false; break;
				case 7: delay += 100; break;
				case 9: 
					delay -= 100; 
					if(delay < 0) delay = 0;
					break;
			}
			swap_buffers();
			delay_milli(40);
		}
	}
}

void print_grid(){
	for(int i = 0; i < gridx; i++){
		for(int j = 0; j < gridy; j++){
			if(temp_grid[i][j])			
				pixel_dubbelbuffer(i,j);
		}
	}	
}

void clear_grid(){
	for(int i = 0; i < gridx; i++){
		for(int j = 0; j < gridy; j++){
			temp_grid[i][j] = 0;
		}
	}
}

void glider_preset(int offset_x, int offset_y){
	int glider[3][3] = {{1,0,0},{0,1,1},{1,1,0}};
	int *glider_rows[3] = {glider[0], glider[1],glider[2]};
	preset_to_grid(&glider[0], 3,3, offset_x, offset_y);
}

void glider_gun_preset(int offset_x, int offset_y){//helt klart det mest optimala sättet....
	int *glider_gun_rows[15] = {glider_gun[0], glider_gun[1], glider_gun[2], glider_gun[3], glider_gun[4],
								glider_gun[5], glider_gun[6], glider_gun[7], glider_gun[8], glider_gun[9], 
								glider_gun[10], glider_gun[11], glider_gun[12], glider_gun[13], glider_gun[14]};
	preset_to_grid(&glider_gun[0], 15,38, offset_x, offset_y);
}

void light_spaceship_preset(int offset_x, int offset_y){
	int ship[4][5] = {{0,1,1,1,1},{1,0,0,0,1},{0,0,0,0,1},{1,0,0,1,0}};
	int *ship_rows[4] = {ship[0], ship[1], ship[2], ship[3]};
	preset_to_grid(&ship[0], 4,5, offset_x, offset_y);
}

void small_explosion_preset(int offset_x, int offset_y){
	int small_explosion[4][3] = {{0,1,0},{1,1,1},{1,0,1},{0,1,0}};
	int *small_rows[4] = {small_explosion[0], small_explosion[1], small_explosion[2], small_explosion[3]};
	preset_to_grid(&small_rows[0], 4,3, offset_x, offset_y);
}

void big_explosion_preset(int offset_x, int offset_y){
	int big_explosion[5][5] = {{1,0,1,0,1},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{1,0,1,0,1}};
	int *big_rows[5] = {big_explosion[0], big_explosion[1], big_explosion[2], big_explosion[3], big_explosion[4]};
	preset_to_grid(&big_rows[0], 5,5, offset_x, offset_y);
}

void preset_to_grid(int *row_ptr, int rows, int col, int offset_x, int offset_y){
	for(int y = 0; y < rows; row_ptr+=col, y++){	//#NonStandard #NonCare #fuckDaSystem
		for(int x = 0; x < col; x++)
			temp_grid[x + offset_x][y + offset_y] = row_ptr[x];
	}
}

void cursor_mode(){
	bool looping = true;
/*
	char *s;
	ascii_write_cmd(1);
	delay_milli(2);
	char instruktion[] = "make din organism";
	ascii_gotoxy(1,1);
	s = instruktion;
	while(*s)
		ascii_write_char(*s++);
	ascii_gotoxy(1,1);
	delay_milli(4000);
	ascii_write_cmd(1);
	delay_milli(2);
	char instruktion1[] = "press 5 to place";
	s = instruktion1;
	while(*s)
		ascii_write_char(*s++);
	ascii_gotoxy(1,2);
	char instruktion2[] = "2 ^| 4 <-| 6 ->| 8 v";
	s = instruktion2;
	while(*s)
		ascii_write_char(*s++);
	delay_milli(1000);
	while(ascii_read_status() & 0x80);
	delay_mikro(8);
	delay_milli(4000);
	*/
	
	while(looping){
		uint8_t dir;
		dir = keyb();
		switch(dir){
			case 1: looping = false; break;
			case 2: mark->posy--; break;
			case 4: mark->posx--; break;
			case 5: temp_grid[mark->posx + 2][mark->posy + 2] = 1;break;
			case 6: mark->posx++; break;
			case 7: clear_grid(); break;
			case 8: mark->posy++; break;
			case 0xA: glider_preset(mark->posx + 2,mark->posy + 2);			break;
			case 0xB: glider_gun_preset(mark->posx + 2,mark->posy + 2);		break;
			case 0xC: small_explosion_preset(mark->posx + 2,mark->posy + 2);break;
			case 0xD: big_explosion_preset(mark->posx + 2,mark->posy + 2);	break;
			case 0xF: light_spaceship_preset(mark->posx + 2,mark->posy + 2);break;
		}
		clear_buffer(0);
		mark->draw(mark);
		for(int x = 0; x < 128; x++){
			for(int y = 0; y < 64; y++){
				if(temp_grid[x][y])
					pixel_dubbelbuffer(x,y);
			}
		}
		swap_buffers();
		delay_mikro(500);	
	}
}

