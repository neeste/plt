/*
 *
 * 	subroutine wave (NPTS,YDAT)
 *
 * 	PURPOSE:	PLTFIL routine to plot waveforms
 *
 *	USAGE:		wave (NPTS,YDAT)
 *
 *			Where	NPTS -- Number of data points to plot
 *				YDAT -- Y-values   (REAL*4 array)
 *
 ***********************************************************************
 */
#include "plt.h"
#include "pltblk.h"
#include "fort.h"

double  xpos(double);
double  ypos(double);

void
wave(int npts, float *ydat, float *xdat)
{
    int     i;
    double  x1, x2, dx;

    setplt((int) param_.lintyp, 0); /* select line type */
    setplw(size_.pltlnw);	    /* seet line width */
    if (misc_.colors)
	setpci((int) param_.pltcol);
    setpps(size_.pltlps);	    /* set line pattern size */
/*
    Scale the data values
 */
    if (npts < 2)
	return;
    x1 = xpos(axes_.rxmin);
    x2 = xpos(axes_.rxmax);
    dx = (x2 - x1) / (npts - 1);
    for (i = misc_.npkept; i < npts; i++) {
	xdat[i] = (float) (x1 + i * dx);
	ydat[i] = (float) ypos(ydat[i]);
    }
    line(npts, xdat, ydat);
}
