#ifndef DELAY_H
#define	DELAY_H
#include "types.h"

#define STK 0xE000E010
#define STK_CTRL ((volatile unsigned int *) STK)
#define STK_LOAD ((volatile unsigned int *) (STK + 4))
#define STK_VAL ((volatile unsigned int *) (STK + 8))

typedef struct tstk{
	uint32_t CTRL;
	uint32_t LOAD;
	uint32_t VAL;
}SYSTK;

void delay_250ns( void );
void delay_500ns( void );
void delay_mikro(uint32_t us);
void delay_milli(uint32_t ms);



#endif //DELAY_H