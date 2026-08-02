/* tppcl.c */

#include <stdio.h>

#define LEFT	1
#define RIGHT	2
#define BELOW	4
#define ABOVE	8
#define MAXNCUT 128
#define online(x1,y1,x2,y2,x3,y3) ((x1>x3)?1:((x1<x3)?-1:0))

extern int      xmin;
extern int      ymin;
extern int      xmax;
extern int      ymax;
extern int      verbose;
extern long     vecpp;
extern long     vecpb;

int             intersect();
int             xcvt();
int             ycvt();
int             outwindow();
int             clip();
int             nint();
void            line();
void            tpcl();
void            circset();
void            circclr();
void            spotset();
void            spotclr();
void            vline();

/* tppcl - Two-point 'polygon-clipped' line (assumes x1 == x2) */
void
tppcl(x1, y1, x2, y2, color, n, px, py)
short           x1, y1, x2, y2, color;
short           n;
short          *px, *py;
{
    short           i, j, on0, on1, on2, onside, nc;
    short           yc[MAXNCUT];
    short           px1, py1, px2, py2, ax, ay, cx1, cy1, cy2;

    if (n < 3)
	return;
    cx1 = xcvt(x1);
    if (cx1 >= xmin && cx1 <= xmax) {
	nc = 0;
	px2 = px[n - 2];
	py2 = py[n - 2];
	on0 = online(x1, y1, x2, y2, px2, py2);
	px1 = px[n - 1];
	py1 = py[n - 1];
	on1 = online(x1, y1, x2, y2, px1, py1);
	for (i = 0; i < n; i++) {	/* Find the intersection points */
	    px2 = px[i];
	    py2 = py[i];
	    if (px2 == px1 && py2 == py1)
		continue;
	    on2 = online(x1, y1, x2, y2, px2, py2);
	    onside = (on0 == 0 || on1 != 0 || on2 == on0);
	    if (onside && on1 != on2 && nc < MAXNCUT) {
		if (intersect(x1, y1, x2, y2, px1, py1, px2, py2, &ax, &ay)) {
		    yc[nc] = ay;
		    nc++;
		}
	    }
	    if (nc >= MAXNCUT) {
		fprintf(stderr, "Too many cuts (%d).\n", nc);
		return;
	    }
	    px1 = px2;
	    py1 = py2;
	    on0 = on1;
	    on1 = on2;
	}

	for (i = 0; i < nc; i++) {	/* sort list of intersection points */
	    for (j = i + 1; j < nc; j++) {
		if (yc[j] < yc[i]) {
		    ay = yc[i];
		    yc[i] = yc[j];
		    yc[j] = ay;
		}
	    }
	}

	for (i = 1; i < nc; i += 2) {	/* draw lines between pairs of points */
	    cy1 = ycvt(yc[i - 1]);
	    cy2 = ycvt(yc[i]);
	    vline(cx1, cy1, cy2, color);
	}
	vecpb++;
    }
    vecpp++;
}

/*
 * intersect - find the intersection of line A and line B Returns true if a
 * valid intersection is found. The intersection point is returned in
 * (abx,aby).
 */
int
intersect(ax1, ay1, ax2, ay2, bx1, by1, bx2, by2, abx, aby)
    short           ax1, ay1, ax2, ay2, bx1, by1, bx2, by2;
    short          *abx, *aby;
{
    double          dax, day, dbx, dby, dx1, dy1, ta, tb, det;

    dax = ax2 - ax1;
    day = ay2 - ay1;
    dbx = bx2 - bx1;
    dby = by2 - by1;
    det = dax * dby - day * dbx;
    if (det == 0.)
	return (0);		/* can't continue if lines are parallel */
    dx1 = bx1 - ax1;
    dy1 = by1 - ay1;
    ta = (dby * dx1 - dbx * dy1) / det;
    tb = (dax * dy1 - day * dx1) / det;
    if (ta < 0. || 1. < ta || 0. < tb || 1. < ta)
	return (0);		/* intersection point not within line
				 * segments */
    *abx = (short) (ax1 + ta * dax);
    *aby = (short) (ay1 + ta * day);
    return (1);
}

/* line - convert a single line segment to a sequence of pixels */

void
line(x0, y0, x1, y1, color)
    int             x0, y0, x1, y1, color;
{
    int             dx, dy;
    int             xinc, yinc;
    int             res1;
    int             res2;

    xinc = 1;
    yinc = 1;
    if ((dx = (int) (x1 - x0)) < 0) {
	xinc = -1;
	dx = -dx;
    }
    if ((dy = (int) (y1 - y0)) < 0) {
	yinc = -1;
	dy = -dy;
    }
    res1 = 0;
    res2 = 0;
    if (color) {
	if (dx >= dy)
	    while (x0 != x1) {
		circset(x0, y0);
		if (res1 > res2) {
		    res2 += dx - res1;
		    res1 = 0;
		    y0 += yinc;
		}
		res1 += dy;
		x0 += xinc;
	    }
	else
	    while (y0 != y1) {
		circset(x0, y0);
		if (res1 > res2) {
		    res2 += dy - res1;
		    res1 = 0;
		    x0 += xinc;
		}
		res1 += dx;
		y0 += yinc;
	    }
	spotset(x1, y1);
    } else {
	if (dx >= dy)
	    while (x0 != x1) {
		circclr(x0, y0);
		if (res1 > res2) {
		    res2 += dx - res1;
		    res1 = 0;
		    y0 += yinc;
		}
		res1 += dy;
		x0 += xinc;
	    }
	else
	    while (y0 != y1) {
		circclr(x0, y0);
		if (res1 > res2) {
		    res2 += dy - res1;
		    res1 = 0;
		    x0 += xinc;
		}
		res1 += dx;
		y0 += yinc;
	    }
	spotclr(x1, y1);
    }
}

/* tpcl - draws a two-point clipped line */

void
tpcl(xt1, yt1, xt2, yt2, color)
    int             xt1, yt1, xt2, yt2;
{

    /* Clip the line and plot it if there's anything left after clipping */

    if (clip(&xt1, &yt1, &xt2, &yt2)) {
	line(xt1, yt1, xt2, yt2, color);
	vecpb++;
    }
    vecpp++;
}

/* clip - line clipped at window (in device coordinates)  */

int
clip(xt1, yt1, xt2, yt2)
    int            *xt1, *yt1, *xt2, *yt2;
{
    int             w1, w2;
    int             accept;
    double          dx, dy;

    /* clip line, if necessary */

    w1 = outwindow(*xt1, *yt1);	/* is first point outside ? */
    w2 = outwindow(*xt2, *yt2);	/* is second point outside ? */
    while (!(!w1 && !w2) && !(w1 & w2)) {
	dx = *xt2 - *xt1;
	dy = *yt2 - *yt1;
	if (w1) {		/* (x1,y1) out of the window */
	    if (w1 & LEFT) {
		*yt1 += nint((xmin - *xt1) * dy / dx);
		*xt1 = xmin;
	    } else if (w1 & RIGHT) {
		*yt1 += nint((xmax - *xt1) * dy / dx);
		*xt1 = xmax;
	    } else if (w1 & BELOW) {
		*xt1 += nint((ymin - *yt1) * dx / dy);
		*yt1 = ymin;
	    } else if (w1 & ABOVE) {
		*xt1 += nint((ymax - *yt1) * dx / dy);
		*yt1 = ymax;
	    }
	    w1 = outwindow(*xt1, *yt1);
	} else {		/* (x2,y2) out of the window */
	    if (w2 & LEFT) {
		*yt2 += nint((xmin - *xt2) * dy / dx);
		*xt2 = xmin;
	    } else if (w2 & RIGHT) {
		*yt2 += nint((xmax - *xt2) * dy / dx);
		*xt2 = xmax;
	    } else if (w2 & BELOW) {
		*xt2 += nint((ymin - *yt2) * dx / dy);
		*yt2 = ymin;
	    } else if (w2 & ABOVE) {
		*xt2 += nint((ymax - *yt2) * dx / dy);
		*yt2 = ymax;
	    }
	    w2 = outwindow(*xt2, *yt2);
	}
    }
    accept = !w1 && !w2;
    return (accept);
}

/* outwindow - determine if (x,y) is outside clipping window */

int
outwindow(x, y)
    int             x, y;
{
    int             outcode = 0;

    if (x < xmin)
	outcode |= LEFT;
    if (x > xmax)
	outcode |= RIGHT;
    if (y < ymin)
	outcode |= BELOW;
    if (y > ymax)
	outcode |= ABOVE;
    return (outcode);
}

/* nint - convert floating-point number to nearest integer */

int
nint(x)
    double          x;
{
    if (x >= 0.)
	x += 0.5;
    else
	x -= 0.5;
    return ((int) x);
}
