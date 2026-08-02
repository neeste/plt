/* 
 *  axes2.c - process axis ticks
 */

#include "plt.h"
#include "pltblk.h"
#include "fort.h"

void    axtick(char, char, char, char,
               double, double, double, double, double,
               double, double, double, double);
float   xtsz = 0;
float   ytsz = 0;
float   ttsz = 0;
float   rtsz = 0;

void
axes2(void)
{
    char    out, top, right;

/*
 * Process AXIS TICKS
 */
    if (size_.axlnw == 0.0)
	return;
    out = (param_.ticdir == 0.0);
    top = (param_.tmin != 0.0 || param_.tmax != 0.0);
    right = (param_.rmin != 0.0 || param_.rmax != 0.0);
    setplw(size_.axlnw);	/* Set axis line weight */
    if (misc_.colors)
	setpci((int) param_.axcol);	/* Set axis color */
    setplt(0, 0);			/* set line type to solid */

    xtsz = (out ? -1 : 1) * size_.tiklen * param_.xtick;
    ytsz = (out ? -1 : 1) * size_.tiklen * param_.ytick;
    ttsz = (out ? 1 : -1) * size_.tiklen * param_.ttick;
    rtsz = (out ? 1 : -1) * size_.tiklen * param_.rtick;

    /* left axis [Y] */
    if (axes_.yaxis)
	axtick(1, 0, 1, 1,
	    param_.yper, param_.yshift, ytsz,
	    axes_.rymin, axes_.rymax, param_.yint, param_.ycycle,
	    axes_.rxllc, axes_.ryllc + axes_.rylen);

    /* bottom axis [X] */
    if (axes_.xaxis)
	axtick(0, 1, 1, 1,
	    param_.xper, param_.xshift, xtsz,
	    axes_.rxmin, axes_.rxmax, param_.xint, param_.xcycle,
	    axes_.rxllc, axes_.ryllc);

    /* right axis [R] */
    if (axes_.raxis) {
	if (right) {
	    axtick(0, 2, 1, 1,
		param_.rper, param_.rshift, rtsz,
		param_.rmin, param_.rmax, param_.rint, param_.rcycle,
		axes_.rxllc + axes_.rxlen, axes_.ryllc);
	} else if (param_.rint != 0) {
	    axtick(0, 2, 1, 1,
		param_.yper, param_.yshift, rtsz,
		axes_.rymin, axes_.rymax, param_.yint, param_.ycycle,
		axes_.rxllc + axes_.rxlen, axes_.ryllc);
	}
    }

    /* top axis [T] */
    if (axes_.taxis) {
	if (top) {
	    axtick(1, 3, 1, 1,
		param_.tper, param_.tshift, ttsz,
		param_.tmin, param_.tmax, param_.tint, param_.tcycle,
		axes_.rxllc + axes_.rxlen, axes_.ryllc + axes_.rylen);
	} else if (param_.tint != 0) {
	    axtick(1, 3, 1, 1,
		param_.xper, param_.xshift, ttsz,
		axes_.rxmin, axes_.rxmax, param_.xint, param_.xcycle,
		axes_.rxllc + axes_.rxlen, axes_.ryllc + axes_.rylen);
	}
    }
}
