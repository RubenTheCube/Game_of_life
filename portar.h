#ifndef	PORTAR_H
#define	PORTAR_H
#include "types.h"

typedef struct tgpio{
	uint32_t	moder;
	uint32_t	otyper;
	uint32_t	ospeedr;
	uint32_t	pupdr;
	union{
		uint32_t idr;
		struct{
			byte idrLow;
			byte idrHigh;
		};
	};
	union{
		uint32_t odr;
		struct{
			byte odrLow;
			byte odrHigh;
		};
	};
}GPIO;

#define GPIO_D (*((volatile GPIO *) 0x40020C00))
#define GPIO_E (*((volatile GPIO *) 0x40021000))

 /*

 #define GPIO_E 0x40021000
 #define E_MODER ((volatile unsigned int *) GPIO_E)
 #define E_OTYPER ((volatile unsigned char *) (GPIO_E + 5))
 #define E_PUPDR ((volatile unsigned int *) (GPIO_E+ 0xC))
 #define E_IDRLOW ((volatile unsigned char *) (GPIO_E + 0x10))
 #define E_IDRHIGH ((volatile unsigned char *) (GPIO_E + 0x11))
 #define E_ODRLOW ((volatile unsigned char *) (GPIO_E + 0x14))
 #define E_ODRHIGH ((volatile unsigned char *) (GPIO_E+ 0x15))
  * Watch variabler
0x40021000
(*(volatile unsigned int *) 0x40021000)
(*(volatile unsigned char *) (0x40021000 + 5))
(*(volatile unsigned int *) (0x40021000+ 0xC))
(*(volatile unsigned char *) (0x40021000 + 0x10))
(*(volatile unsigned char *) (0x40021000 + 0x11))
(*(volatile unsigned char *) (0x40021000 + 0x14))
(*(volatile unsigned char *) (0x40021000 + 0x15))
*/
#endif //PORTAR_H