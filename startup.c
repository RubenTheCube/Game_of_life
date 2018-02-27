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
PTR_OBJ mark = &marker;
uint8_t temp_grid[gridx][gridy];

//int grid[gridx][gridy], gridbuffer[gridx][gridy];

int main(int argc, char **argv)
{
	//init
	mark->posx = 4;
	mark->posy = 4;
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
	//int shape[3][3] = {	{1,0,0},{0,1,1},{1,1,0}};
						
	//clear_grid();
	//cursor_mode();
				
/*
	for(int i = 1; i <= shapex; i++){		//copy shape to grid
		for(int j = 1; j <= shapey; j++){
			grid[i][j] = shape[j-1][i-1]; 	//this needs to be this wonky
		}
	}
*/
	cursor_mode();
	clear_buffers();
	graphic_clear_screen();
	print_grid();
	swap_buffers();
	int rv = 0;
	while(1)
	{
		clear_buffer(0);
		for(int i = 0; i < 127; i++){
			for(int j = 0; j < 63; j++){
				rv = check_neighbors(i,j);
				if(rv == 3 || rv == 12 || rv == 13) //3 i alla fall, 12/13 pupulated med 2/3 grannar
 					pixel_dubbelbuffer(i,j);
			}
		}
		
		swap_buffers();
		delay_milli(40);
		
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
			case 6: mark->posx++; break;
			case 4: mark->posx--; break;
			case 2: mark->posy++; break;
			case 8: mark->posy--; break;
			case 5: 
				temp_grid[mark->posx + 2][mark->posy + 2] = 1;
				break;
			case 1: looping = false; break;			
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

