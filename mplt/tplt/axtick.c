/* axtick - plot tick marks */

#include <math.h>
#include "plt.h"
#include "pltblk.h"
#include "fort.h"

#undef  log2
#define log2(x)	(log10(x) / zlog[0])

void    linetp(double, double, double, double);

static double zlog[8] = {
    0.3010, 0.4771, 0.6021, 0.6990, 0.7782, 0.8451, 0.9031, 0.9542
};

void
axtick(char revers, char xflag, char bflag, char eflag,
    double vpct, double vshf, double vtsz, double vmin, double vmax, 
    double vint, double vcyc, double xbeg, double ybeg)
{
    double  xb, yb, xdir, ydir, axlen;
    double  s1, s2, zmin = 0;
    double  t1x, t1y, t2x, t2y, v, vendmn, vendmx, pos, vend;
    double  x, y, vnext, vmid, a;
    double  delv, fpct, frc1, frc2;
    double  x1, x2, y1, y2;
    int     linax, nminor, iexp = 0, i;
/*
 *
 *---------------------------------------------------------------------
 *
 */
    if (vint == 0.0 && vcyc == 0.0)
	return;
    if (vtsz == 0.0)
	return;
    if (vmax == vmin)
	return;
/*
 * Determine the offset to the first tick and minor tick length
 */
    fpct = vpct / 100.0;
    frc1 = (50.0 - vpct / 2.0 + vshf) / 100.0;
    frc2 = (50.0 - vpct / 2.0 - vshf) / 100.0;
    if ((xflag % 2) != 0) {
	xdir = 1.0;
	ydir = 0.0;
	t1x = 0.0;
	t1y = vtsz;
	axlen = axes_.rxlen;
	xb = axes_.rxllc + axlen * frc1;
	yb = ybeg;
    } else {
	xdir = 0.0;
	ydir = 1.0;
	t1x = vtsz;
	t1y = 0.0;
	axlen = axes_.rylen;
	xb = xbeg;
	yb = axes_.ryllc + axlen * frc1;
    }
    t2x = 0.5 * t1x;
    t2y = 0.5 * t1y;

/*
 * Determine the number of minor intervals and, if it is a log axis,
 * the limit to refer points to
 */
    if (revers) {
	s2 = vmin;
	s1 = vmax;
    } else {
	s1 = vmin;
	s2 = vmax;
    }
    if (vcyc == 0.0) {
	if (vint < 1.0)
	    vint = 1.0;
	delv = (s2 - s1) / floor(vint);
	nminor = ((int) (10.0 * vint + 0.5)) % 10;
	linax = TRUE;
    } else {
	delv = (s2 < s1) ? -1.0 : 1.0;
	if (vint == -2.0) {
	    nminor = 1;
	    zmin = 0.0;
	    vcyc = log2(vmax / vmin);
	    s1 = log2(s1 / vmin);
	    s2 = log2(s2 / vmin);
	} else {
	    nminor = 9;
	    zmin = log10(vmin);
	    vcyc = log10(vmax / vmin);
	    s1 = log10(s1);
	    s2 = log10(s2);
	}
	linax = FALSE;
    }
/*
 * Check whether beginning and ending tick marks are off the ends
 */
    bflag = bflag || ((revers ? frc2 : frc1) != 0) || (param_.corner == 0);
    eflag = eflag || ((revers ? frc1 : frc2) != 0) || (param_.corner == 0);
/*
 * Do major and minor intervals until we reach the end of the segment
 */
    v = s1;
    vendmn = amin1(s1, s2);
    vendmx = amax1(s1, s2);
    x1 = axes_.rxllc - 0.001;
    y1 = axes_.ryllc - 0.001;
    x2 = axes_.rxllc + axes_.rxlen + 0.001;
    y2 = axes_.ryllc + axes_.rylen + 0.001;
    do {
	/* Determine the position of this tick and the # value associated
	   with the next major interval */
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
	/* Position the pen and draw the major tick, except when ticks are
	   inward-going at the segment edge and there is no offset */
	pos *= axlen * fpct;
	x = xb + pos * xdir;
	y = yb + pos * ydir;
	if ((v != s1 && v != s2) || bflag) {
            if ((x > x1) && (x < x2) && (y > y1) && (y < y2)) {
                linetp(x, y, x + t1x, y + t1y);
            }
	}
	/* Do the minor intervals, if they exist */
	if (nminor > 1) {
	    if (linax)		/* Linear axis */
		;
	    else {
		vmid = 0.5 * (v + vend);
		if (vmid < 0.0)
		    iexp = (int) (vmid - 1.0);
		else
		    iexp = (int) vmid;
	    }
	    for (i = 1; i < nminor; i++) {
		if (linax) {	/* Linear axis */
		    vnext = v + i * delv / nminor;
		    pos = (vnext - vmin) / (vmax - vmin);
		} else {	/* Log axis */
		    if (delv > 0.0)
			vnext = iexp + zlog[i - 1];
		    else
			vnext = iexp + zlog[nminor - i - 1];
		    pos = (vnext - zmin) / vcyc;
		}
		if (vnext <= vendmn || vendmx <= vnext)
		    continue;
		pos *= axlen * fpct;
		x = xb + pos * xdir;
		y = yb + pos * ydir;
                if ((x > x1) && (x < x2) && (y > y1) && (y < y2)) {
                    linetp(x, y, x + t2x, y + t2y);
                }
	    }
	}
	v = vend;
    } while (vendmn < v && v < vendmx);
/*
 * Finish the segment up by drawing the last tick mark,
 * unless it overlaps an adjacent axis line.
 */
    if (eflag) {
	a = (revers) ? 0.0 : axlen * vpct / 100.0;
	x = xb + a * xdir;
	y = yb + a * ydir;
        if ((x > x1) && (x < x2) && (y > y1) && (y < y2)) {
	    linetp(x, y, x + t1x, y + t1y);
        }
    }
}
