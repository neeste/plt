/*
 * 	axgrid -	PLTFIL routine to plot axis grid lines.
 *
 */
#include <math.h>
#include "plt.h"
#include "pltblk.h"
#include "fort.h"

#undef  log2
#define log2(x)	(log10(x) / log10(2))

void    linetp(double, double, double, double);

//static float zlog[8] = {
//    0.3010, 0.4771, 0.6021, 0.6990, 0.7782, 0.8451, 0.9031, 0.9542
//};

void
axgrid(char revers, char xflag, char bflag, char eflag,
       double vpct, double vshf, double vmin, double vmax, double vint, double vcyc,
       double xbeg, double ybeg)
{
    double   xb, yb, xdir, ydir, axlen;
    double   s1, s2, zmin = 0, offs;
    double   t1x, t1y, v, vendmn, vendmx, pos, vend;
    double   x, y, a;
    double   delv, fpct, frc1, frc2;
    int     linax;
/*
 *
 *---------------------------------------------------------------------
 *
 */
    if ((xflag % 2) != 0) {
	xdir = 1.0;
	ydir = 0.0;
	t1x = 0.0;
	t1y = axes_.rylen;
	axlen = axes_.rxlen;
    } else {
	xdir = 0.0;
	ydir = 1.0;
	t1x = axes_.rxlen;
	t1y = 0.0;
	axlen = axes_.rylen;
    }

    if (vint == 0.0 && vcyc == 0.0)
	return;
    if (revers) {
	s2 = vmin;
	s1 = vmax;
    } else {
	s1 = vmin;
	s2 = vmax;
    }
/*
 * Determine the number of minor intervals and, if it is a log axis,
 * the limit to refer points to
 */
    if (vcyc == 0.0) {
	if (vint < 1.0)
	    vint = 1.0;
	delv = (s2 - s1) / floor(vint);
	linax = TRUE;
    } else {
	delv = (s2 < s1) ? -1.0 : 1.0;
	if (vint == -2.0) {
	    zmin = 0.0;
	    vcyc = log2(vmax / vmin);
	    s1 = log2(s1 / vmin);
	    s2 = log2(s2 / vmin);
	} else {
	    zmin = log10(vmin);
	    vcyc = log10(vmax / vmin);
	    s1 = log10(s1);
	    s2 = log10(s2);
	}
	linax = FALSE;
    }
/*
 * Determine the offset to the first grid line
 */
    fpct = vpct / 100.0;
    frc1 = (50.0 - vpct / 2.0 + vshf) / 100.0;
    frc2 = (50.0 - vpct / 2.0 - vshf) / 100.0;
    offs = axlen * frc1;
/*
 * Check whether beginning and ending tick marks are off the ends
 */
    bflag = bflag || ((revers ? frc2 : frc1) != 0);
    eflag = eflag || ((revers ? frc1 : frc2) != 0);
/*
 * Start path
 */
    xb = xbeg;
    yb = ybeg;
    if ((xflag % 2) != 0)
	xb = axes_.rxllc + offs;
    else
	yb = axes_.ryllc + offs;
/*
 * Do major intervals until we reach the end of the segment
 */
    v = s1;
    vendmn = amin1(s1, s2);
    vendmx = amax1(s1, s2);
    do {
	/* Determine the position of this grid line */
	if (linax) {
	    pos = (v - vmin) / (vmax - vmin);
	    vend = v + delv;
	} else {
	    pos = (v - zmin) / vcyc;
	    a = floor(v);
	    if ((delv < 0.0 && v > a) || (delv > 0.0 && v < a))
		vend = a;
	    else
		vend = floor(v + delv);
	}
	vend = amax1(vend, vendmn);
	vend = amin1(vend, vendmx);
	/* Position the pen and draw the major grid line except at the
	   segment edge when there is no offset */
	pos *= axlen * fpct;
	x = xb + pos * xdir;
	y = yb + pos * ydir;
	if ((v != s1 && v != s2) || bflag) {
	    linetp(x, y, x + t1x, y + t1y);
	}
	v = vend;
    } while (vendmn < v && v < vendmx);
/*
 * Finish the segment up by drawing the last grid line
 */
    if (eflag) {
	a = (revers) ? 0.0 : axlen * vpct / 100.0;
	x = xb + a * xdir;
	y = yb + a * ydir;
	linetp(x, y, x + t1x, y + t1y);
    }
}
