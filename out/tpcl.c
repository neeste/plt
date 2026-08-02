/*
 * tpcl.c
 */

#include <math.h>
#include "output.h"
#include "device.h"
#include "clip.h"
#include "copa.h"
#include "cview.h"
#include "cwind.h"

int     at_curpnt(double, double);
void    set_curpnt(double, double);

/*
 * clip - line clipped at window (in NC)
 */
int
clip(float *xt1, float *yt1, float *xt2, float *yt2)
{
    int     w1, w2;
    int     accept;
    float   dx, dy;

/* clip line, if necessary */

    w1 = outwindow(*xt1, *yt1);	/* is first point outside ? */
    w2 = outwindow(*xt2, *yt2);	/* is second point outside ? */
    while (!(accept = !w1 && !w2) && !(w1 & w2)) {
	dx = *xt2 - *xt1;
	dy = *yt2 - *yt1;
	if (w1) {		/* (x1,y1) out of the window */
	    if (w1 & LEFT) {
		*yt1 += (XMN - *xt1) * dy / dx;
		*xt1 = XMN;
	    } else if (w1 & RIGHT) {
		*yt1 += (XMX - *xt1) * dy / dx;
		*xt1 = XMX;
	    } else if (w1 & BELOW) {
		*xt1 += (YMN - *yt1) * dx / dy;
		*yt1 = YMN;
	    } else if (w1 & ABOVE) {
		*xt1 += (YMX - *yt1) * dx / dy;
		*yt1 = YMX;
	    }
	    w1 = outwindow(*xt1, *yt1);
	} else {		/* (x2,y2) out of the window */
	    if (w2 & LEFT) {
		*yt2 += (XMN - *xt2) * dy / dx;
		*xt2 = XMN;
	    } else if (w2 & RIGHT) {
		*yt2 += (XMX - *xt2) * dy / dx;
		*xt2 = XMX;
	    } else if (w2 & BELOW) {
		*xt2 += (YMN - *yt2) * dx / dy;
		*yt2 = YMN;
	    } else if (w2 & ABOVE) {
		*xt2 += (YMX - *yt2) * dx / dy;
		*yt2 = YMX;
	    }
	    w2 = outwindow(*xt2, *yt2);
	}
    }
    return (accept);
}

/*
 * tpl - draw the basic two-point line
 *       (assumed to be already transformed to NC & clipped)
 */

void
tpl(double xt1, double yt1, double xt2, double yt2)
{
    if (!at_curpnt(xt1, yt1)) {
	DEV_TRNS(xt1, yt1, 0);
    }
    DEV_TRNS(xt2, yt2, 1);
    set_curpnt(xt2, yt2);
}

/*
 * tpcl - two-point clipped line (input assumed to be in NC)
*/
void
tpcl(double xxt1, double yyt1, double xxt2, double yyt2)
{
    float   xt1, yt1, xt2, yt2;

    xt1 = (float) xxt1;
    yt1 = (float) yyt1;
    xt2 = (float) xxt2;
    yt2 = (float) yyt2;

/* Clip the line and plot it if there's anything left after clipping */

    if (clip(&xt1, &yt1, &xt2, &yt2))
    tpl(xt1, yt1, xt2, yt2);
}

/*
 * tpcc - two-point clipped curve (input assumed to be in NC)
*/
void
tpcc(float *x, float *y)
{
    double xx, yy, np;
    float xm, ym, x1[4], y1[4], x2[4], y2[4];
    int c, i;

    c = 0;
    for (i = 0; i < 4; i++) {
	if (outwindow(x[i], y[i]))
	    c++;
    }
    if (c == 4) {			// all points outside
	return;
    } else if (c == 0) {
        if (!at_curpnt(x[0], y[0])) {	// all points inside
	    DEV_TRNS(x[0], y[0], 0);
	    set_curpnt(x[0], y[0]);
	}
	DEV_CURV(x + 1, y + 1);
        set_curpnt(x[3], y[3]);
    } else {				// needs clipping
        xm = (x[0] + 3 * x[1] + 3 * x[2] + x[3]) / 8;
        ym = (y[0] + 3 * y[1] + 3 * y[2] + y[3]) / 8;
        xx = xm - (x[0] + x[3]) / 2;
        yy = ym - (y[0] + y[3]) / 2;
        np = sqrt(xx * xx + yy * yy) * dev_.adupi;
	if (np < 1) {			// draw two lines
	    tpcl(x[0], y[0], xm, ym);
	    tpcl(xm, ym, x[3], y[3]);
	} else {			// draw two curves
	    x1[0] = x[0];
	    y1[0] = y[0];
	    x1[1] = (x[0] + x[1]) / 2;
	    y1[1] = (y[0] + y[1]) / 2;
	    x1[2] = (x[0] + 2 * x[1] + x[2]) / 4;
	    y1[2] = (y[0] + 2 * y[1] + y[2]) / 4;
	    x1[3] = xm;
	    y1[3] = ym;
	    x2[0] = xm;
	    y2[0] = ym;
	    x2[1] = (x[3] + 2 * x[2] + x[1]) / 4;
	    y2[1] = (y[3] + 2 * y[2] + y[1]) / 4;
	    x2[2] = (x[3] + x[2]) / 2;
	    y2[2] = (y[3] + y[2]) / 2;
	    x2[3] = x[3];
	    y2[3] = y[3];
	    tpcc(x1, y1);
	    tpcc(x2, y2);
	}
    }
}

/*
 * curve - draw Bezier from (x1,y1) to (x[2],y[2])
 */

void
curve(float *x, float *y)
{
    double  x1, y1, x2, y2, xa, ya, xb, yb, xx, yy;
    double a, b, c, d, s, t, np;
    int i, n;

    x1 = dev_.curxnc;
    y1 = dev_.curync;
    xa = x[0];
    ya = y[0];
    xb = x[1];
    yb = y[1];
    x2 = x[2];
    y2 = y[2];
    //
    // interpolate between endpoints
    //
    xx = ((x1 + x2) - (xa + xb)) / 2;
    yy = ((y1 + y2) - (ya + yb)) / 2;
    np = sqrt(xx * xx + yy * yy) * dev_.adupi;
    n = (int) ceil(np * 2);
    for (i = 1; i < n; i++) {
	t = (double) i / n;
	s = 1 - t;
	a = s * s * s;
	b = t * s * s * 3;
	c = t * t * s * 3;
	d = t * t * t;
	xx = a * x1 + b * xa + c * xb + d * x2;
	yy = a * y1 + b * ya + c * yb + d * y2;
        DEV_TRNS(xx, yy, 1);
    }
    DEV_TRNS(x2, y2, 1);
}

