/* line.c */

#include <stdio.h>
#include "copa.h"
#include "cview.h"

double  pythag(double, double);
int     clip(float *, float *, float *, float *);
void    dshlin(double, double, double, double, double, double, double, int, int);
void    penwid(double);
void    pencolor(int, int);

/*
 line - draw a line connecting arrays of (x,y) locations
 */
void
line(int n, float *px, float *py)
{
    double  space, dash, pwid, pdif;
    int     ndot, icont;
    float   x1, y1, x2, y2, x1t, y1t, x2t, y2t;

    if (n <= 1)
	return;
    pencolor(copa_.curpci, 0);
    pwid = (copa_.curplw < 0) ? copa_.curplw : copa_.curplw * 0.005;
    penwid(pwid);
    if (copa_.curplt == 0) {
	space = 0;
	dash = 0;
	ndot = 0;
    } else if (copa_.curplt == 1) {
	space = copa_.curpps * cview_.vgn;
	dash = 0;
	ndot = 0;
    } else {
	space = copa_.curpps * cview_.vgn;
	dash = space * (copa_.curplt - 1) / 4;
        ndot = copa_.curdot;
    }
    pdif = 0;
    icont = 0;
    otrans(*px++, *py++, &x1, &y1);
    x1t = x1;
    y1t = y1;
    while (--n) {
	otrans(*px++, *py++, &x2, &y2);
	if (n > 2) {
	    pdif += pythag(x2 - x1t, y2 - y1t) * 100;
	    x1t = x2;
	    y1t = y2;
	} else {
	    pdif = 1;
	}
	if (pdif >= 1) {
	    x2t = x2;
	    y2t = y2;
	    if (!clip(&x1, &y1, &x2, &y2)) {
		icont = 0;
	    } else {
		dshlin(x1, y1, x2, y2, dash, space, pwid, ndot, icont);
		icont = 1;
	    }
	    pdif = 0;
	    x1 = x2t;
	    y1 = y2t;
	}
    }
}
