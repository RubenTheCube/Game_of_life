#ifndef _GRAFIKFUNKTIONER_
#define _GRAFIKFUNKTIONER_

#include "types.h" 
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

void graphic_ctrl_bit_set(uint8_t x);
void graphic_ctrl_bit_clear(uint8_t x);
void select_controller(uint8_t controller);
void graphic_wait_ready(void);
unsigned char graphic_read(unsigned char controller);
void graphic_write(uint8_t value, uint8_t controller);
void graphic_write_command(uint8_t command, uint8_t controller);
void graphic_write_data(uint8_t data, uint8_t controller);
uint8_t graphic_read_data(uint8_t controller);
void graphic_initalize(void);
void graphic_clear_screen(void);
void pixel(int x, int y, int set);
void graphics_clear_area(int page, int add);

#endif