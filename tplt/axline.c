/*
 * 	axline(revers, xflag, vbrk, vpct, xbeg, ybeg)
 *
 *
 *	DATE OF FIRST VERSION:		 9-Feb-83
 *	DATE OF CURRENT VERSION:	21-Jun-83
 *
 *	PURPOSE:	PLTFIL routine to plot axis lines.
 *
 *	PROGRAMMER:	E. Javel / M. Prucka
 *
 ********************************************************************
 *
 */
#include <math.h>
#include "plt.h"
#include "pltblk.h"
#include "fort.h"

void    linetp(double, double, double, double);

static float b1 = 0.25;
static float b2 = 0.75;

void
axline(char revers, char xflag, double vbrk, double vpct, double vshf,
       double xcor, double ycor)
{
    double axlen, x, y, a, d;
    double  xbr1, xbr2, xbr3, xbr4, xbr5, xbr6;
    double  ybr1, ybr2, ybr3, ybr4, ybr5, ybr6;
    double  xbeg, ybeg, xend, yend;
/*
 *
 *---------------------------------------------------------------------
 *
 */
    d = (revers) ? -1 : 1;
    a = d * size_.tiklen;
    if ((xflag % 2) != 0) {
	axlen = axes_.rxlen;
	if (param_.corner) {
	    xbeg = xcor;
	    xend = xbeg + d * axlen;
	} else {
	    xbeg = xcor + d * axlen * ((100 - vpct) / 2 + d * vshf) / 100;
	    xend = xbeg + d * axlen * vpct / 100;
	}
	ybeg = yend = ycor;
	if (vbrk > 0) {
	    x = xcor + axlen * (0.01 * vbrk - ((revers) ? 1.0 : 0.0));
	    y = 0.5 * a;
	} else {
	    x = xcor - axlen * (0.01 * vbrk + ((revers) ? 1.0 : 0.0));
	    y = -0.5 * a;
	}
	xbr1 = x - a * b1;
	xbr2 = x + a * b1;
	xbr3 = x - a * b2;
	xbr4 = x + a * b2;
	xbr5 = x - a * b1;
	xbr6 = x + a * b1;
	ybr1 = ybeg;
	ybr2 = ybeg + y;
	ybr3 = ybeg - y;
	ybr4 = ybeg + y;
	ybr5 = ybeg - y;
	ybr6 = ybeg;
    } else {
	axlen = axes_.rylen;
	if (param_.corner) {
	    ybeg = ycor;
	    yend = ybeg + d * axlen;
	} else {
	    ybeg = ycor + d * axlen * ((100 - vpct) / 2 + d * vshf) / 100;
	    yend = ybeg + d * axlen * vpct / 100;
	}
	xbeg = xend = xcor;
	if (vbrk > 0) {
	    x = 0.5 * a;
	    y = ybeg + axlen * (0.01 * vbrk - ((revers) ? 1.0 : 0.0));
	} else {
	    x = -0.5 * a;
	    y = ybeg - axlen * (0.01 * vbrk + ((revers) ? 1.0 : 0.0));
	}
	xbr1 = xbeg;
	xbr2 = xbeg + x;
	xbr3 = xbeg - x;
	xbr4 = xbeg + x;
	xbr5 = xbeg - x;
	xbr6 = xbeg;
	ybr1 = y - a * b1;
	ybr2 = y + a * b1;
	ybr3 = y - a * b2;
	ybr4 = y + a * b2;
	ybr5 = y - a * b1;
	ybr6 = y + a * b1;
    }

    if (vbrk == 0) {
	linetp(xbeg, ybeg, xend, yend);	/* draw line */
    } else {
	linetp(xbeg, ybeg, xbr1, ybr1);	/* draw line */
	linetp(xbr2, ybr2, xbr3, ybr3);	/* draw line */
	linetp(xbr4, ybr4, xbr5, ybr5);	/* draw line */
	linetp(xbr6, ybr6, xend, yend);	/* draw line */
    }
}
