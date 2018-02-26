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
    int x,y;
    for(y = 0; this->geo->sizex*y < this->geo->numpoints;y++){
        for(x = 0; x < this->geo->sizex; x++)
            pixel(this->posx + this->geo->px[this->geo->sizex*y+x].x,this->posy + this->geo->px[this->geo->sizex*y+x].y,1);
    }
}

void clear_object(PTR_OBJ this){
    int x = 0,y = 0;
    for(y = 0; this->geo->sizex*y < this->geo->numpoints;y++){
        for(x = 0; x < this->geo->sizex; x++)
            pixel(this->posx + this->geo->px[this->geo->sizex*y+x].x,this->posy + this->geo->px[this->geo->sizex*y+x].y,0);
    }
}

#endif