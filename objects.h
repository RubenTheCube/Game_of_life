#ifndef OBJECTS_H
#define OBJECTS_H

typedef unsigned char uint8_t;
typedef enum {false,true } bool;

typedef struct {
	uint8_t x;
	uint8_t y;
}POINT;

typedef struct {
	int numpoints;
	int sizex;
	int sizey;
	POINT px[MAX_POINTS];
}GEOMETRY, *PTR_GEO;

typedef struct tObj{
	PTR_GEO geo;
	int speedx, speedy;
	int posx, posy;
}OBJECT, *PTR_OBJ;

#endif