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

#define gridx 15
#define gridy 15
PTR_OBJ mark = &marker;

//int grid[gridx][gridy], gridbuffer[gridx][gridy];

int main(int argc, char **argv)
{
	//init
	mark->posx = 4;
	mark->posy = 4;
	init_app();
	graphic_initalize();
	ascii_init();
	ascii_init();
	ascii_write_cmd(1);
	graphic_clear_screen();
	clear_buffers();
	
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
	//print_grid();
	
	int rv = 0;
	while(1)
	{
		check_neighbors(20,7);
		clear_buffer(0);
		pixel_dubbelbuffer(20,7);
		pixel_dubbelbuffer(21,8);
				
		swap_buffers();
		
		delay_milli(40);
		
		
		//graphics_clear_area(2, 20);
		//print_grid();		
		
		//delay_mikro(500);
	}
}

void print_grid(){
	for(int i = 0; i < gridx; i++){
		for(int j = 0; j < gridy; j++){
			if(grid[i][j])			
				pixel(i,j,1);
		}
	}	
}

void clear_grid(){
	for(int i = 0; i < gridx; i++){
		for(int j = 0; j < gridy; j++){
			grid[i][j] = 0;
		}
	}
}

void cursor_mode(){
	bool looping = true;
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
	while(looping){
		uint8_t dir;
		mark->clear(mark);
		print_grid();
		dir = keyb();
		switch(dir){
			case 6: mark->posx++; break;
			case 4: mark->posx--; break;
			case 2: mark->posy++; break;
			case 8: mark->posy--; break;
			case 5: 
				grid[mark->posx + 2][mark->posy + 2] = 1;
				break;
			case 1: looping = false; break;			
		}
		mark->draw(mark);
		delay_mikro(500);	
	}
}

