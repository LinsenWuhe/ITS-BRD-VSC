#include "error.h"
#include "calc.h"

extern bool fehlerBeiPhasenwechsel;


void fehlerLoeschen(void)
{
    calcInit();
    fehlerBeiPhasenwechsel = false;
}
 