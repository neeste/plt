/* histo.c - plot histogram data
 *
 *
 *	DATE OF  FIRST  VERSION:	20-Sep-78
 *	DATE OF CURRENT VERSION:	21-Jun-83
 *
 *	PURPOSE:	PLTFIL routine to plot histogram data
 *
 *	USAGE:		HISTO (NVALS,BIN,err)
 *
 *			Where	NVALS = No. of bins
 *				BIN   = Bin values
 *				ERR   = Error values
 *
 *	NOTES FOR PROGRAMMERS:	The histogram fills the entire X-axis length
 *				with the number of bins provided.  Thus, all
 *				bins along the X-axis must be accounted for.
 *
 *	PROGRAMMER:	Mike Collins / Eric Javel / Matt Prucka
 *
 **********************************************************************
 */
#include <math.h>
#include "plt.h"
#include "pltblk.h"

double  ypos(double);
void    errbar(double, double, double, double, double, double, double);
void    shadearea(int, float *, float *, int);

void
histo(int nvals, float *pbin, float *perr)
{
/*
 * -----------------------------------------------------------------------
 *
 * Initialize
 */
    double  xbase, barwid, eb, en, a;
    double  xbegin, ybegin, xend, yend;
    double  top, bot;
    float   bx[4], by[4];
    float  *pb, *pbend, *pe;
    int     ssi;

    xbase = axes_.rxllc + axes_.rxlen * (1. - param_.xper / 100.) / 2.;
    barwid = axes_.rxlen * param_.xper / 100. / nvals;	/* # histogram bin width
							   (inches) */
    if (param_.ebwid >= 0.0)
	eb = param_.ebwid * barwid * 0.16667;	/* error bar width (inches) */
    else
	eb = -param_.ebwid;
    en = param_.ebnum;		/* number of error bars */
    setplw(size_.pltlnw);	/* plot line weight for error bars */
    if (misc_.colors)
	setpci((int) param_.pltcol);	/* Set axis color */
    ssi = (int) param_.shade;	/* shade style index */
/*
 * Do the histogram
 */
    xend = xbase;
    for (pb = pbin, pbend = pbin + nvals, pe = perr; pb < pbend; pb++) {
	xbegin = xend;
	xend += barwid;
	if (param_.ymin > 0) {
	    ybegin = ypos(param_.ymin);
	} else {
	    ybegin = ypos(0.0);
	}
	if (param_.ymin > (double) *pb) {
	    yend = ypos(param_.ymin);
	} else {
	    yend = ypos((double) *pb);
	}
	bx[0] = (float) xbegin;
	by[0] = (float) ybegin;
	bx[1] = (float) xend;
	by[1] = (float) ybegin;
	bx[2] = (float) xend;
	by[2] = (float) yend;
	bx[3] = (float) xbegin;
	by[3] = (float) yend;
	shadearea(4, bx, by, ssi);
/*
	    Plot error bar if it exists
 */
	a = fabs(*pe++);
	if (a > 0.) {
	    if (param_.ycycle == 0.) {
		top = ypos(*pb + a);
		bot = ypos(*pb - a);
	    } else {
		top = ypos(*pb * a);
		bot = ypos(*pb / a);
	    }
	    errbar((xbegin + xend) * .5, yend, top, bot, eb, 0.0, en);
	}
    }
}
