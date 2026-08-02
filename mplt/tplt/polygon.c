/* polygon (ndat,xdat,ydat)
 *
 *
 *	PURPOSE:	PLTFIL routine to plot polygon
 *
 *	USAGE:		polygon (ndat,xdat,ydat)
 *
 *			Where	ndat = No. data points
 *				xdat,ydat = corners
 *
 **********************************************************************
 */
#include "plt.h"
#include "pltblk.h"

double  xpos(double);
double  ypos(double);
void    shadearea(int, float *, float *, int);

void
polygon(int ndat, float *xdat, float *ydat)
{
    int     i;

    for (i = misc_.npkept; i < ndat; i++) {	/* Transform data in-place */
	xdat[i] = (float) xpos(xdat[i]);
	ydat[i] = (float) ypos(ydat[i]);
    }
    setplw(size_.pltlnw);
    if (misc_.colors)
	setpci((int) param_.pltcol);
    shadearea(ndat, xdat, ydat, (int) param_.shade);
}
