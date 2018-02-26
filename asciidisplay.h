#ifndef _ASCIIFUNKTIONER_
#define _ASCIIFUNKTIONER_
#include "types.h"
 
void ascii_bit_set(uint8_t x);
void ascii_bit_clear(uint8_t x);
void ascii_write_controller(uint8_t a_byte);
void ascii_write_cmd(uint8_t command);
void ascii_write_data(uint8_t data);
uint8_t ascii_read_controller(void);
uint8_t ascii_read_status(void);
uint8_t ascii_read_data(void);
void ascii_init(void);
void ascii_gotoxy (int x, int y);
void ascii_write_char(uint8_t c);

#endif