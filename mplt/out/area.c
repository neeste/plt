/* area.c */

#include <stdio.h>
#include "copa.h"

void    hollowarea(int, float *, float *);
void    solidarea(int, float *, float *);
void    hatcharea(int, float *, float *);

/* area - outline or fill an area defined by points */

void
area(int n, float *px, float *py)
{
    switch ((int) copa_.curfais) {
    case 0:
	hollowarea(n, px, py);
	break;
    case 1:
	solidarea(n, px, py);
	break;
    case 3:
	hatcharea(n, px, py);
	break;
    }
}
