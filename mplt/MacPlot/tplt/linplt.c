/* linplt.c 
 *
 *
 * 	linplt(npts,xdat,ydat,zdat)
 *
 *	PROGRAM NAME:			linplt.c
 *
 *	DATE OF  FIRST  VERSION:	21-Jun-83
 *	DATE OF  CURRENT  VERSION:	 9-SEP-88
 *
 *	PURPOSE:	PLTFIL routine to plot non-histogram data
 *
 *	USAGE:		linplt(npts,xdat,ydat,zdat)
 *
 *			Where	npts -- Number of data points to plot
 *				xdat -- X-values   (float array)
 *				ydat -- Y-values   (float array)
 *				zdat -- Error bars (float array)
 *
 *	Functions called:
 *		    amax1, cosdeg, errbar, errout, fabs, line, linetp, marker
 *		    pythag, setmks, setmkt, setmkup, setplt, setplw, setpps,
 *		    sindeg, smooth, sort, spline, sprintf, xpos, ypos
 *
 *	Called by:
 *		    doplot
 *
 *
 *	PROGRAMMER:	Mike Collins / Eric Javel / Matt Prucka
 *
 ***********************************************************************
 */
#include <math.h>
#include "plt.h"
#include "pltblk.h"
#include "fort.h"

extern int maxnp;
extern float *xdat, *ydat, *zdat;

double  cosdeg(double);
double  pythag(double, double);
double  sindeg(double);
double  xpos(double);
double  ypos(double);
int     mordat(void);
void    arrow(int, float *, float *);
void    errbar(double, double, double, double, double, double, double);
void    histo(int, float *, float *);
void    linetp(double, double, double, double);
void    shadearea(int, float *, float *, int);
void    smooth3(int, float *);
void    sort(int, float *, float *, float *);

static void spline(int, float *, float *, int);
static double bspline(float *, double);

void
linplt(int npts)
{
	int     i, k, nsp, plty, pc, fc;
	double  yp, yn, a, b, eb, eg, en, gap, xn, xp, delx, dely;
	double  top, bot;

/*
 *----------------------------------------------------------------------
 */

#ifdef DEBUG
	if (flags_.dbflg) {
	sprintf(ctext_.msg, "linplt: npts=%d, npkept=%d\n", npts, misc_.npkept);
	errout(ctext_.msg);
	}
#endif
	plty = (int) param_.pltype;	/* plot type: 1=symbols, 2=lines, 3=both */

/* Set up for error bars */

	if (param_.gapsiz >= 0.0)
	gap = param_.gapsiz * size_.hitsym / 2.;
	else
	gap = -param_.gapsiz;
	if (param_.ebwid >= 0.0)
	eb = param_.ebwid * amax1(gap, size_.avgaxl * .01);
	else
	eb = -param_.ebwid;
	if (plty == 2) {
	eg = 0.;
	} else {
	eg = size_.hitsym / 2.;
	}
	en = param_.ebnum;

/* Check if splines are to be drawn */

	if (param_.spline == 0 || npts < 4) {
	nsp = 0;
	} else {
	nsp = (int) ((param_.spline > 0) ? npts * param_.spline : -param_.spline);
	while ((npts + nsp) > maxnp)
		if (mordat() == 0)
		break;
	if ((npts + nsp) > maxnp) {
		errout("ERROR: too many data points for spline smoothing\n");
		nsp = 0;
	}
	}

/* Sort and smooth the data values, if specified */

	if (misc_.npkept == 0) {
	if (param_.sort != 0.0)
		sort(npts, xdat, ydat, zdat);
	if (param_.smooth != 0.0 && npts >= 3)
		smooth3(npts, ydat);
	}

/* set line attributes for plot line and error bars */

	setplw(size_.pltlnw);	/* select plot line width */
	if (misc_.colors)
	setpci((int) param_.pltcol);    /* select plot line color */

/* Loop over data values */

	for (i = 0; i < npts; i++) {

/* Scale the data values */

	if (i < misc_.npkept) {
		xp = xdat[i];
		yp = ydat[i];
	} else {
		xp = xpos((double) xdat[i]);
		yp = ypos((double) ydat[i]);
	}

/* Plot error bar if nonzero and sticks out beyond the symbol */

	a = fabs((double) zdat[i]);
	if (a > 0.0) {
		if (param_.ycycle == 0.) {
		b = ypos(a) - ypos(0.0);
		} else {
		b = ypos(a) - ypos(1.0);
		}
		top = yp + b;
		bot = yp - b;
		if (fabs(top - bot) >= size_.hitsym) {
		errbar(xp, yp, top, bot, eb, eg, en);
		}
	}
/* Store the scaled data values in place */

	xdat[i] = (float) xp;
	ydat[i] = (float) yp;
	}

/*
 * Connect the symbols
 */
	setplt((int) param_.lintyp, (int) param_.lindot);       /* select line type */
	setpps(size_.pltlps);	                            /* set plot line pattern size */
	/* lines */
	if ((plty == 2 || plty == 3) && nsp > 0) {		    /* draw spline curve */
	spline(npts, xdat, ydat, nsp);
	} else if ((plty == 2 || plty == 3) && param_.stem) {   /* draw stems */
		if (!axes_.logy) {
		yp = ypos(0.0);
	} else {
		yp = param_.ymin;
	}
	for (i = 0; i < npts; i++) {
		xp = xdat[i];
		xn = xdat[i];
		yn = ydat[i];
		yn = yn - gap * (yn / fabs(yn));
		linetp(xp, yp, xn, yn);
	}
	} else if (plty == 2) {				    /* connect the points */
	line(npts, xdat, ydat);
	arrow(npts, xdat, ydat);
	} else if (plty == 3) {				    /* draw lines between points */
	for (i = 1; i < npts; i++) {

		xp = xdat[i - 1];
		yp = ydat[i - 1];
		xn = xdat[i];
		yn = ydat[i];
/*
		Juggle the starting and ending positions of the line so
		that the lines do not go through the symbols.  The lines
		begin and end one-half of a symbol height away from the datum.
 */
		delx = xn - xp;
		dely = yn - yp;
		a = pythag(delx, dely);
		if (a > gap) {
		b = gap / a;
		xp += delx * b;
		yp += dely * b;
		xn -= delx * b;
		yn -= dely * b;
		}
		linetp(xp, yp, xn, yn);
	}
	}
	/* symbols */
	if (plty == 1 || plty == 3) {	/* draw symbol at points */
	k = (int) param_.symbol;
	if (k <= 5 && param_.solid == 1)
		k += 10;
	setmkfw(param_.solid != -1);	/* set marker fill white */
	setmkt(k);		/* select marker type */
	setmks(size_.hitsym);	/* set marker size */
	if (misc_.colors) {
		pc = param_.pltcol < 0 ? inqbci() : (int) param_.pltcol;
		fc = param_.filcol < 0 ? pc : (int) param_.filcol;
		setmkci(pc);
		setmkfi(fc);
	}
	setmkup(-sindeg(param_.symang), cosdeg(param_.symang));
	marker(npts, xdat, ydat);
	}
}

/* spline - B-spline curve from Foley & Van Dam, p. 521. */

static void
spline(int n, float *x, float *y, int nsp)
{
	double  nn, t, dn;
	float  *xx, *yy;
	int     i, j;

	xx = x + n;
	yy = y + n;
	dn = ((n - 3.0) / (nsp - 1.0));
	for (i = 0; i < (nsp - 1); i++) {
	nn = dn * i;
	j = (int) nn;
	t = nn - j;
	xx[i] = (float) bspline(x + j, t);
	yy[i] = (float) bspline(y + j, t);
	}
	xx[nsp - 1] = (float) bspline(x + n - 4, 1.0);
	yy[nsp - 1] = (float) bspline(y + n - 4, 1.0);
	line(nsp, xx, yy);
}

static double
bspline(float *x, double t)
{
	float   t0, t1, t2, t3;

	t3 = -x[0] + 3 * x[1] - 3 * x[2] + x[3];
	t2 = 3 * x[0] - 6 * x[1] + 3 * x[2];
	t1 = -3 * x[0] + 3 * x[2];
	t0 = x[0] + 4 * x[1] + x[2];
	return ((((t3 * t + t2) * t + t1) * t + t0) / 6);
}

void
cumplt(int npts)
{
	double  yp, yn, xn = 0, oon, tcnt, pcnt, ccnt;
	int     i, j, npcd, nn;
	float  *xx, *yy;

	if (npts <= 0)
	return;
	npcd = 2 * (npts + 2);  // allow for extensions
	//
	// Allocate memory for cum. dist.
	//
	xx = (float *) calloc(npcd * 2, sizeof(float));
	if (xx == NULL)
	return;
	yy = xx + npcd;
	//
	// Sort the data values
	//
	sort(npts, xdat, ydat, zdat);
	//
	// Determine total count
	//
	if (param_.count) {
	tcnt = 0;
		for (i = 0; i < npts; i++) {
		tcnt += ydat[i];
	}
	} else {
	tcnt = npts;
	}
	nn = 0;
	//
	// left extension
	//
	if (param_.cdext && param_.xmin < xdat[0]) {
		yn = ypos(param_.yofst);
		xn = xpos(param_.xmin);
		xx[nn] = (float) xn;
		yy[nn] = (float) yn;
		nn++;
		xx[nn] = (float) xn;
		yy[nn] = (float) yn;
		nn++;
	}
	//
	// Compute the y data values
	//
	pcnt = 0;
	oon = (param_.norm != 0) ? param_.norm / tcnt : 1;
	for (i = 0; i < npts; i = j) {
	ccnt = pcnt;
	for (j = i; j < npts && xdat[j] == xdat[i]; j++)
		ccnt += param_.count ? ydat[j] : 1;
	if (xdat[i] >= param_.xmin && xdat[i] <= param_.xmax) {
		yp = ypos(pcnt * oon * param_.ygain + param_.yofst);
		yn = ypos(ccnt * oon * param_.ygain + param_.yofst);
		xn = (float) xpos(xdat[i]);
		yy[nn] = (float) yp;
		xx[nn] = (float) xn;
		nn++;
		yy[nn] = (float) yn;
		xx[nn] = (float) xn;
		nn++;
	}
	pcnt = ccnt;
	}
	//
	// right extension
	//
	if (param_.cdext && param_.xmax >  xdat[npts - 1]) {
		yn = ypos(tcnt * oon * param_.ygain + param_.yofst);
		xn = xpos(param_.xmax);
	xx[nn] = (float) xn;
	yy[nn] = (float) yn;
	nn++;
	xx[nn] = (float) xn;
	yy[nn] = (float) yn;
	nn++;
	}
	//
	// Check for smoothing
	//
	if (param_.smooth) {
		for (i = 1; i < nn / 2; i++) {
		xx[2 * i - 1] = (xx[2 * i - 1] + xx[2 * i - 0]) / 2;
		yy[2 * i - 2] = (yy[2 * i - 2] + yy[2 * i - 1]) / 2;
	}
	if (param_.smooth == 2)
			smooth3(nn, yy);
	}
	//
	// Connect the points
	//
	setplt((int) param_.lintyp, (int) param_.lintyp);       /* select line type */
	setpps(size_.pltlps);		                    /* set plot line pattern size */
	setplw(size_.pltlnw);		                    /* select plot line width */
	if (misc_.colors)
	setpci((int) param_.pltcol);                        /* select plot line color */
	line(nn, xx, yy);			                    /* cumulative distribution */
	//
	// free memory
	//
	free(xx);
}

#define MAXNBIN 99

void
frqplt(int npts)
{
	float   x1, x2, xx[MAXNBIN * 2], yy[MAXNBIN * 2], cnt, oon = 0;
	int     i, j, k, b, nmaj, nmin, nn;
	static float  cs[MAXNBIN];
	static int ns = 0;

	nmaj = (int) param_.xint;
	nmin = (int) ((param_.xint - nmaj) * 10 + 0.01);
	if (nmin > 0 && nmaj * nmin <= MAXNBIN)
	nn = nmaj * nmin;
	else if (nmaj > 0 && nmaj <= MAXNBIN)
	nn = nmaj;
	else if (nmaj > 0)
	nn = MAXNBIN;
	else
	nn = 1;
	for (b = 0; b < nn; b++) {
	x1 = param_.xmin + ((param_.xmax - param_.xmin) / nn) * b;
	x2 = param_.xmin + ((param_.xmax - param_.xmin) / nn) * (b + 1);
	cnt = 0;
	for (i = 0; i < npts; i++) {
		if (xdat[i] >= x1 && xdat[i] < x2)
			cnt += param_.count ? ydat[i] : 1;
	}
	//
	// check likelihood parameter
	//
	if (param_.like < 2) {
		oon = (param_.norm != 0) ? param_.norm / npts : 1;
		if (param_.like == 1) {
		cs[b] = cnt;
		ns = b + 1;
		}
	} else if (param_.like >= 2 && nn == ns) {
		oon = (param_.norm != 0) ? param_.norm : 1;
			if (param_.like == 2 && cnt > 0) {
			cnt = cnt / (cnt + cs[b]);
		} else if (param_.like == 3 && cs[b] > 0) {
			cnt = cs[b] / (cnt + cs[b]);
		} else {
		cnt = 0;
		}
	}
	j = b * 2;
	k = j + 1;
		xx[j] = (float) xpos(x1);
		yy[j] = (float) ypos(cnt * oon * param_.ygain + param_.yofst);
		xx[k] = (float) xpos(x2);
		yy[k] = (float) ypos(cnt * oon * param_.ygain + param_.yofst);
	}
	//
	// Check for smoothing
	//
	if (param_.smooth) {
		for (i = 1; i < nn; i++) {
		xx[2 * i - 2] = (xx[2 * i - 2] + xx[2 * i - 1]) / 2;
		yy[2 * i - 1] = (yy[2 * i - 1] + yy[2 * i - 0]) / 2;
	}
	if (param_.smooth == 2)
			smooth3(nn, yy);
	}
	//
	// Connect the points
	//
	setplt((int) param_.lintyp, (int) param_.lindot);       /* select line type */
	setpps(size_.pltlps);		                    /* set plot line pattern size */
	setplw(size_.pltlnw);		                    /* select plot line width */
	if (misc_.colors)
	setpci((int) param_.pltcol);                        /* select plot line color */
	if (param_.like != 1)
		line(nn * 2, xx, yy);
}

static void
draw_arrow(double x1, double y1, double x2, double y2)
{
	float x[5], y[5];
	int     n = 0;
	double  w, h, r, t, dx, dy, xx, yy;

	if (size_.arosiz > 0 && param_.arowid > 0) {
		dx = x2 - x1;
	dy = y2 - y1;
	r = sqrt(dx * dx + dy * dy);
	if (r > 0) {
		dx /= r;
			dy /= r;
		h = size_.arosiz;
		w = h * param_.arowid / 2;
			t = h * param_.arotip;
		xx = x2;
		yy = y2;
			x[n] = (float) (xx - t * dx);
		y[n] = (float) (yy - t * dy);
		n++;
		x[n] = (float) (xx - h * dx - w * dy);
		y[n] = (float) (yy - h * dy + w * dx);
		n++;
		x[n] = (float) xx;
		y[n] = (float) yy;
		n++;
		x[n] = (float) (xx - h * dx + w * dy);
		y[n] = (float) (yy - h * dy - w * dx);
		n++;
		x[n] = (float) (xx - t * dx);
		y[n] = (float) (yy - t * dy);
		n++;
	}
	}
	shadearea(n, x, y, 141);    /* match arrow color to line */
}

void
arrow(int npts, float * xdat, float *ydat)
{
	int e;

	e = nint(param_.aronum);
	if (e == 1 || e == 2)
		draw_arrow(xdat[npts-2], ydat[npts-2], xdat[npts-1], ydat[npts-1]); 
	if (e == 2 || e == -1)
		draw_arrow(xdat[1], ydat[1], xdat[0], ydat[0]); 
}
