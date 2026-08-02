/* hollowarea - outline and hollow the area */

#include <stdio.h>
#include "copa.h"
#include "output.h"
#include "device.h"

void    outlinarea(int, float *, float *);
void    pencolor(int, int);
void    penwid(double);
void    tpcl(double, double, double, double);

void
hollowarea(int n, float *px, float *py)
{
    pencolor(copa_.curfaci, 0);
    penwid((copa_.curplw < 0) ? copa_.curplw : copa_.curplw * 0.005);
    outlinarea(n, px, py);
}

/* outlinarea - outline the area */

void
outlinarea(int n, float *px, float *py)
{
    float   xt, yt, x0, y0, x1, y1, x2, y2;

    if (n > 0) {
	otrans(*px++, *py++, &xt, &yt);
	x1 = x0 = xt;
	y1 = y0 = yt;
	while (--n > 0) {
	    otrans(*px++, *py++, &xt, &yt);
	    x2 = xt;
	    y2 = yt;
	    tpcl(x1, y1, x2, y2);
	    x1 = xt;
	    y1 = yt;
	}
	tpcl(x1, y1, x0, y0);
    }
}
