/* hatchare.c */

#include <stdio.h>
#include <math.h>
#include "copa.h"
#include "cview.h"

#define GN cview_.vgn

double  pythag(double, double);
void    getbox(int, float *, float *, float *, float *, float *, float *);
void    penwid(double);
void    pencolor(int, int);
void    tppcl(double, double, double, double, int, float *, float *, int);

static void singlehatch(int, float *, float *, double, double);

/* hatcharea - fill an area with hatch pattern */

void
hatcharea(int n, float *px, float *py)
{
    int     i, j, k;
    float   ls;
    static float linsp[2] = {(float) 0.05, (float) 0.08};
    static float slope[6] = {2.0, -2.0, 1.0, -1.0, 0.0, 1.e9};

    i = copa_.curfasi / 9;
    if (i < 0 || 1 < i)
	return;
    pencolor(copa_.curfaci, 0);
    penwid(copa_.curfalw);
    ls = linsp[i] / GN;
    j = copa_.curfasi % 9;
    if (j < 3) {
	k = j * 2;
	singlehatch(n, px, py, ls, slope[k]);
	singlehatch(n, px, py, ls, slope[k + 1]);
    } else {
	k = j - 3;
	singlehatch(n, px, py, ls, slope[k]);
    }
}

/* singlehatch - fill area with hatch pattern */

static void
singlehatch(int n, float *px, float *py, double ds, double sl)
{
    double  by, xx, yy, dx, dy, xend, xref, yref;
    float   bx1, by1, bx2, by2;
    static double eps = 0.001;

    getbox(n, px, py, &bx1, &by1, &bx2, &by2);

    xref = yref = 0.0;
    if (fabs(sl) < eps) {	/* Horizontal lines */
	yy = ceil((by1 - yref) / ds) * ds + yref;
	while (yy < by2) {
	    tppcl(bx1, yy, bx2, yy, n, px, py, 0);
	    yy += ds;
	}
    } else if (fabs(sl) > 1 / eps) {	/* Vertical lines */
	xx = ceil((bx1 - xref) / ds) * ds + xref;
	while (xx < bx2) {
	    tppcl(xx, by1, xx, by2, n, px, py, 0);
	    xx += ds;
	}
    } else if (sl > 0.) {	/* Positive slope lines */
	dy = ds * pythag(1.0, sl);
	dx = dy / sl;
	by = floor(((by2 - copa_.curyrf) - (bx1 - copa_.curxrf) * sl) / dy) * dy;
	xx = (by2 - by) / sl;
	yy = by + bx1 * sl;
	xend = bx2 + (by2 - by1) / sl;
	while (xx < xend) {
	    tppcl(bx1, yy, xx, by2, n, px, py, 0);
	    xx += dx;
	    yy -= dy;
	}
    } else if (sl < 0.) {	/* Negative slope lines */
	dy = ds * pythag(1., sl);
	dx = -dy / sl;
	by = ceil(((by1 - copa_.curyrf) - (bx1 - copa_.curxrf) * sl) / dy) * dy;
	xx = (by1 - by) / sl;
	yy = by + bx1 * sl;
	xend = bx2 + (by1 - by2) / sl;
	while (xx < xend) {
	    tppcl(bx1, yy, xx, by1, n, px, py, 0);
	    xx += dx;
	    yy += dy;
	}
    }
}
