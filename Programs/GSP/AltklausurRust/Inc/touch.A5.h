#ifndef TOUCH_H
#define TOUCH_H

#include "main.h"

//die Position des Blocks 0 bis 8
int getBlockPos(Coordinate c);


//liefert die Position des Blocks, der auf dem Display berührt wurde
int getTouchedBlock(void);
	
#endif
