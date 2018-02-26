#ifndef OBJECTS_H
#define OBJECTS_H

typedef unsigned char uint8_t;

typedef struct {
	uint8_t x;
	uint8_t y;
}POINT;

typedef struct {
	int numpoints;
	int sizex;
	int sizey;
	POINT px[20];
}GEOMETRY, *PTR_GEO;

typedef struct tObj{
	PTR_GEO geo;
	int speedx, speedy;
	int posx, posy;
	void (* draw) (struct tObj *);
	void (* clear) (struct tObj *);
}OBJECT, *PTR_OBJ; 

void draw_object(PTR_OBJ this);
void clear_object(PTR_OBJ this);

OBJECT marker;


#endif