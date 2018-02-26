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
};

#endif