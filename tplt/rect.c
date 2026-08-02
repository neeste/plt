/* rect (ndat,xdat,ydat,zdat)
 *
 *
 *	PURPOSE:	PLTFIL routine to plot rectangles
 *
 *	USAGE:		rect (ndat,xdat,ydat)
 *
 * 	Functions called:
 * 		       errbar, fabs, setplw, shadearea, xpos, ypos
 *
 *	Called by:
 *			doplot
 *
 *
 *			Where	ndat = No. data points
 *				xdat,ydat = corners
 *				zdat   = Error values
 *
 **********************************************************************
 */
#include <math.h>
#include "plt.h"
#include "pltblk.h"

double  xpos(double);
double  ypos(double);
void    errbar(double, double, double, double, double, double, double);
void    shadearea(int, float *, float *, int);

void
rect(int ndat, float *xdat, float *ydat, float *zdat)
{
    double  eb, en, a;
    double  xbeg, ybeg, xend, yend;
    double  top, bot, cen;
    float   bx[4], by[4];
    int     i, ssi;

/* Initialize */
    if (param_.ebwid >= 0.0)
	eb = param_.ebwid * size_.avgaxl * 0.01;	/* error bar width
							   (inches) */
    else
	eb = -param_.ebwid;
    setplw(size_.pltlnw);	/* plot line weight for error bars */
    if (misc_.colors)
	setpci((int) param_.pltcol);
    ssi = (int) param_.shade;

/* Do the rectangles */
    for (i = 0; i + 1 < ndat; i += 2) {
	xbeg = xpos(xdat[i]);
	ybeg = ypos(ydat[i]);
	xend = xpos(xdat[i + 1]);
	yend = ypos(ydat[i + 1]);
	bx[0] = (float) xbeg;
	by[0] = (float) ybeg;
	bx[1] = (float) xend;
	by[1] = (float) ybeg;
	bx[2] = (float) xend;
	by[2] = (float) yend;
	bx[3] = (float) xbeg;
	by[3] = (float) yend;
	shadearea(4, bx, by, ssi);

/* Plot error bars, if they exists */
	a = fabs(zdat[i]);
	if (a > 0.0) {
	    if (param_.ycycle == 0.) {
		top = ypos(ydat[i] + a);
		bot = ypos(ydat[i] - a);
	    } else {
		top = ypos(ydat[i] * a);
		bot = ypos(ydat[i] / a);
	    }
	    cen = (xbeg + xend) / 2;
	    en = (ybeg > yend) ? param_.ebnum : -param_.ebnum;
	    errbar(cen, ybeg, top, bot, eb, 0.0, en);
	}
	a = fabs(zdat[i + 1]);
	if (a > 0.0) {
	    if (param_.ycycle == 0.) {
		top = ypos(ydat[i + 1] + a);
		bot = ypos(ydat[i + 1] - a);
	    } else {
		top = ypos(ydat[i + 1] * a);
		bot = ypos(ydat[i + 1] / a);
	    }
	    cen = (xbeg + xend) * 0.5;
	    en = (yend > ybeg) ? param_.ebnum : -param_.ebnum;
	    errbar(cen, yend, top, bot, eb, 0.0, en);
	}
    }
}

void
cntgrect(int ndat, float *xdat, float *ydat, float *zdat)
{
    double  eb, en, a;
    double  xbeg, ybeg, xend, yend;
    double  top, bot, cen;
    float   bx[4], by[4];
    int     i, ssi;

/* Initialize */
    if (param_.ebwid >= 0.0)
	eb = param_.ebwid * size_.avgaxl * 0.01;	/* error bar width
							   (inches) */
    else
	eb = -param_.ebwid;
    setplw(size_.pltlnw);	/* plot line weight for error bars */
    if (misc_.colors)
	setpci((int) param_.pltcol);
    ssi = (int) param_.shade;

/* Do the rectangles */
    for (i = 1; i < ndat; i += 1) {
	xbeg = xpos(xdat[i - 1]);
	ybeg = ypos(ydat[0]);
	xend = xpos(xdat[i]);
	yend = ypos(ydat[i]);
	bx[0] = (float) xbeg;
	by[0] = (float) ybeg;
	bx[1] = (float) xend;
	by[1] = (float) ybeg;
	bx[2] = (float) xend;
	by[2] = (float) yend;
	bx[3] = (float) xbeg;
	by[3] = (float) yend;
	shadearea(4, bx, by, ssi);

/* Plot error bars, if they exists */
	a = fabs(zdat[i]);
	if (a > 0.0) {
	    if (param_.ycycle == 0.) {
		top = ypos(ydat[i] + a);
		bot = ypos(ydat[i] - a);
	    } else {
		top = ypos(ydat[i] * a);
		bot = ypos(ydat[i] / a);
	    }
	    cen = (xbeg + xend) * 0.5;
	    en = (ybeg > yend) ? param_.ebnum : -param_.ebnum;
	    errbar(cen, ybeg, top, bot, eb, 0.0, en);
	}
    }
}
