/* 
 *  axes1.c - process axis lines
 */
#include "plt.h"
#include "pltblk.h"
#include "fort.h"

void    axline(char, char, double, double, double, double, double);

void
axes1(void)
{
    char    top, right;

    if (size_.axlnw == 0.0)
	return;
    top = (param_.tmin != 0.0 || param_.tmax != 0.0);
    right = (param_.rmin != 0.0 || param_.rmax != 0.0);
    setplw(size_.axlnw);	/* Set axis line weight */
    if (misc_.colors)
	setpci((int) param_.axcol);	/* Set axis color */
    setplt(0, 0);			/* set line type to solid */

    /* left axis [Y] */
    if (axes_.yaxis)
	axline(1, 0, param_.ybreak, param_.yper, param_.yshift,
	    axes_.rxllc, axes_.ryllc + axes_.rylen);

    /* bottom axis [X] */
    if (axes_.xaxis)
	axline(0, 1, param_.xbreak, param_.xper, param_.xshift,
	    axes_.rxllc, axes_.ryllc);

    /* right axis [R] */
    if (axes_.raxis) {
	if (right) {
	    axline(0, 2, param_.rbreak, param_.rper, param_.rshift,
		axes_.rxllc + axes_.rxlen, axes_.ryllc);
	} else {
	    axline(0, 2, param_.ybreak, param_.yper, param_.yshift,
		axes_.rxllc + axes_.rxlen, axes_.ryllc);
	}
    }
    /* top axis [T] */
    if (axes_.taxis) {
	if (top) {
	    axline(1, 3, param_.tbreak, param_.tper, param_.tshift,
		axes_.rxllc + axes_.rxlen, axes_.ryllc + axes_.rylen);
	} else {
	    axline(1, 3, param_.xbreak, param_.xper, param_.xshift,
		axes_.rxllc + axes_.rxlen, axes_.ryllc + axes_.rylen);
	}
   }
}
