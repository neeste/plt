/* 
 *  axes3.c - process grid lines
 */
#include "plt.h"
#include "pltblk.h"

void    axgrid(char, char, char, char,
               double, double, double, double, double, double,
               double, double);

extern double inch_unit;

void
axes3(void)
{
/*
 * Process GRID LINES
 */
    if (param_.grid == 0 || size_.grdlnw == 0)
	return;
    setplt((int) param_.grdtyp, 0);     /* set grid line type */
    setplw(size_.grdlnw);               /* Set the grid line weight */
    if (misc_.colors)
	setpci((int) param_.grdcol);    /* Set grid color */
    setpps(size_.grdlps);	        /* set grid line pattern size (dots/inch) */

    /* left axis */
    axgrid(1, 0, !axes_.taxis, !axes_.xaxis, param_.yper, param_.yshift,
	axes_.rymin, axes_.rymax, param_.yint, param_.ycycle,
	axes_.rxllc, axes_.ryllc + axes_.rylen);
    /* bottom axis */
    axgrid(0, 1, !axes_.yaxis, !axes_.raxis, param_.xper, param_.xshift,
	axes_.rxmin, axes_.rxmax, param_.xint, param_.xcycle,
	axes_.rxllc, axes_.ryllc);

}
