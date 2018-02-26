/*
 * 	startup.c
 *
 */

#include "grafikdisplay.h"
#include "keypad.h"
#include "portar.h"
 
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

void graphics_clear_area(int page, int add)
{
    for(int i = 0; i < page; i++)
    {
        graphics_write_command(LCD_SET_PAGE | i, B_CS1 | B_CS2);
        graphics_write_command(LCD_SET_ADD | 0 , B_CS1 | B_CS2);
        for(int j = 0; j < add; j++)
            graphics_write_data(0, B_CS1 | B_CS2);
    }
}

void init_app(void)
{
	*portModer = 0x55555555;//0-15 utgångar
	*portOtyper = 0x00000000; 	// outputs are push/pull 
	*portOspeedr = 0x55555555; // medium speed 
	*portPupdr = 0x55550000; 
	
	
	*GPIO_D_Moder = 0x55005555;
	*GPIO_D_Otyper &= 0xFFFF00FF;//otyper
	*GPIO_D_Otyper |= 0x00000000;
	
	*GPIO_D_Pupdr &= 0x0000FFFF;//pupdr
	*GPIO_D_Pupdr |= 0x00AA0000;
	
}

#define gridx 15
#define gridy 15
PTR_OBJ mark = &marker;

int grid[gridx][gridy], gridbuffer[gridx][gridy];

int main(int argc, char **argv)
{
	
	mark->posx = 4;
	mark->posy = 4;
	init_app();
	graphics_initialize();

	
	int shapex = 3, shapey = 3;
	int shape[3][3] = {	{1,0,0},
						{0,1,1},
						{1,1,0}};
						
	clear_grid();
	cursor_mode();
	copy_grid_to_buffer();
				
/*
	for(int i = 1; i <= shapex; i++){		//copy shape to grid
		for(int j = 1; j <= shapey; j++){
			grid[i][j] = shape[j-1][i-1]; 	//this needs to be this wonky
		}
	}
*/
	print_grid();

	int rv = 0;
	while(1)
	{
		for(int i = 0; i < gridx; i++){
			for(int j = 0; j < gridy; j++){
				rv = check_neighbors(i,j);
				if(grid[i][j]){		//active pixel
					if(rv <= 1 || rv >= 4)
						gridbuffer[i][j] = 0;		//pixel dies
				}
				else{						//unactive pixel
					if(rv == 3)
						gridbuffer[i][j]= 1;		//pixel born
				}
			}
		}
		
		for(int i = 0; i < gridx; i++)	//clear outer "frame"
			gridbuffer[i][14] = 0;
		for(int i = 0; i < gridx; i++)
			gridbuffer[14][i] = 0;
		for(int i = 0; i < gridx; i++)
			gridbuffer[i][0] = 0;
		for(int i = 0; i < gridx; i++)
			gridbuffer[0][i] = 0;
		
		copy_buffer_to_grid();
		graphics_clear_area(2, 20);
		print_grid();		
		
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
	while(looping){
		uint8_t dir;
		print_grid();
		mark->clear(mark);
		dir = keyb();
		switch(dir){
			case 6: mark->posx++; break;
			case 4: mark->posx--; break;
			case 2: mark->posy++; break;
			case 8: mark->posy--; break;
			case 5: 
				grid[mark->posx + 2][mark->posy + 2] = 1;
				break;
			case 1: looping = false;
		}
		mark->draw(mark);
		delay_mikro(500);	
	}
}

void copy_grid_to_buffer(){
	for(int i = 0; i < gridx; i++){			//copy grid to gridbuffer
		for(int j = 0; j < gridy; j++){
			gridbuffer[i][j] = grid[i][j];
		}
	}
}

void copy_buffer_to_grid(){
	for(int i = 0; i < gridx; i++){		//copy gridbuffer to grid
		for(int j = 0; j < gridy; j++)
			grid[i][j] = gridbuffer[i][j];
	}
}

int check_neighbors(int i, int j){
	int rv = 0;
	
	if(grid[(i-1)%(gridx-1)] [(j-1)%(gridx-1)])	rv++;
	if(grid[(i-1)%(gridx-1)] [j]) 		rv++;
	if(grid[(i-1)%(gridx-1)] [(j+1)%(gridx-1)])	rv++;
	if(grid[i][(j-1)%(gridx-1)]) 		rv++;
	if(grid[i][(j+1)%(gridx-1)]) 		rv++;
	if(grid[(i+1)%(gridx-1)][(j-1)%(gridx-1)])	rv++;
	if(grid[(i+1)%(gridx-1)][j]) 		rv++;
	if(grid[(i+1)%(gridx-1)][(j+1)%(gridx-1)])	rv++;
			
	return rv;
}

