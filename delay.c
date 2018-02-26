#include "delay.h"
#include "types.h"

void delay_250ns( void ){ 
	*STK_CTRL = 0;
	*STK_LOAD =(168/4) - 1;
	*STK_VAL = 0;
	*STK_CTRL = 5;
	while(*STK_CTRL & 0x10000)
		;
	*STK_CTRL = 0;
}

void delay_500ns( void ){
	delay_250ns();
	delay_250ns();
}

void delay_mikro(uint32_t us){
	while(us--){
		delay_250ns();
		delay_250ns();
		delay_250ns();
		delay_250ns();
	}
}

void delay_milli(uint32_t ms){
#ifdef SIMULATOR
	while(ms--)
		delay_mikro(1);
#else
	while(ms--)
		delay_mikro(1000);
	#endif
}