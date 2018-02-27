#ifndef OBJECTS_C
#define OBJECTS_C
#include "objects.h"
						
GEOMETRY marker_geometry = {8,5,5,{
						{2,0},{2,1},{2,3},{2,4},
						{0,2},{1,2},{3,2},{4,2}}};

OBJECT marker =
{
	&marker_geometry, 
	0,0,			//speed
	0,0,			//pos
	draw_object,
	clear_object
};

void draw_object(PTR_OBJ this){
	POINT *ptr, *end_ptr;
	ptr = &(this->geo->px);
	
	end_ptr = &(this->geo->px[this->geo->numpoints]); //pekar faktiskt på Pointen efter den sista använda punkten (8'an) men eftersom det är < täcken i for loopen så funkar det fint
    for(ptr; ptr < end_ptr; ptr++)
		pixel(this->posx + ptr->x,this->posy + ptr->y,1);
}

void clear_object(PTR_OBJ this){
	POINT *ptr, *end_ptr;
	ptr = &(this->geo->px);
	
	end_ptr = &(this->geo->px[this->geo->numpoints]); //pekar faktiskt på Pointen efter den sista använda punkten (8'an) men eftersom det är < täcken i for loopen så funkar det fint
    for(ptr; ptr < end_ptr; ptr++)
		pixel(this->posx + ptr->x,this->posy + ptr->y,0);
}

#endif