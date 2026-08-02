/* esccod.c */

#include <stdio.h>
#include <math.h>
#include "arccom.h"
#include "alfcom.h"
#include "tcom.h"

#define	AMODE 1
#define OFF 0
#define ON  1
#define gete()	(e = gettkb())

int gettkb();
void alfctr();
void alfrsp();
void devres();
void ppage();
void ppen();
void tarc();
void tcirc_();
void tmove();

extern int     xhi, yhi, xlo, ylo;
extern int     plotterstatus;
extern int     penchange;
extern FILE   *tekfp;

static double btof();

static struct vuport {
    int     vx1, vy1, vx2, vy2;
}       vuport_;

static int put_back = 0;
int     onepen;

/*
 * esccod - decodes tek escape sequences for tekcvt
 */

void
esccod(int *x, int *y)
{
    int     gettkb(), e, num;
    int     getnum();
    int     radius;
    double  ax1, ay1, ax2, ay2;
    e = gettkb();
    if (e != 'A') {		/* look for device name 'A' */
	if (e == 014) {		/* clear, home */
	    xlo = 0;
	    xhi = 0;
	    ylo = 127 - '`';
	    yhi = 55 - ' ';
	    *y = yhi * 128 + ylo * 4;
	    tmove(0, *y);
	} else if (e == '`');
	else
	    put_back = e;
	return;
    }
    e = gettkb();		/* get next byte after 'A' */
    switch (e) {
    case 'A':
	e = gettkb();
	switch (e) {
	case 'A':		/* draw arc */
	    ax1 = btof();
	    if (gettkb() != ',')
		break;
	    ay1 = btof();
	    if (gettkb() != ',')
		break;
	    ax2 = btof();
	    if (gettkb() != ',')
		break;
	    ay2 = btof();
	    tarc(ax1, ay1, ax2, ay2);
	    break;
	case 'C':		/* draw circle */
	    radius = getnum();
	    tcirc_(&radius);
	    break;
	case 'H':		/* move to home */
	    tmove(vuport_.vx1, vuport_.vy2);
	    e = getnum();
	    break;
	}
	break;
    case 'B':
	e = gettkb();
	switch (e) {
	case 'A':		/* set arc smoothness */
	    arccom_.arcsmo = btof();
	    break;
	case 'P':		/* pen select */
	    e = gettkb();
	    if (e == '0') {
		ppen('0');
	    } else if (e < '0' || '9' < e) {
		ppen('0');
		put_back = e;
	    } else if (penchange == ON) {
		ppen(e);
	    } else {
		ppen(onepen);
	    }
	    break;
	case 'G':		/* alpha slant */
	    alfcom_.slnang = getnum();
	    alfctr(alfcom_.charsp, alfcom_.linesp, alfcom_.rotang,
		alfcom_.slnang, alfcom_.tr);
	    break;
	case 'Q':		/* set alternate font */
	    alfcom_.altfnt = getnum();
	    break;
	}
	break;
    case 'E':			/* turn plotter on */
	plotterstatus = ON;
	break;
    case 'F':			/* turn plotter off */
	plotterstatus = OFF;
	break;
    case 'I':			/* set alpha dimension */
	alfcom_.charsp = getnum();
	if ((e = gettkb()) == ',') {
	    alfcom_.linesp = getnum();
	} else {
	    put_back = e;
	}
	alfctr(alfcom_.charsp, alfcom_.linesp, alfcom_.rotang,
	    alfcom_.slnang, alfcom_.tr);
	break;
    case 'J':			/* set alpha rotation */
	alfcom_.rotang = getnum();
	alfctr(alfcom_.charsp, alfcom_.linesp, alfcom_.rotang,
	    alfcom_.slnang, alfcom_.tr);
	break;
    case 'K':
	ppage();		/* new page */
	break;
    case 'N':			/* device reset */
	num = getnum();
	devres(num);
	break;
    case 'T':			/* change standard font */
	alfcom_.stdfnt = getnum();
	break;
    case 'V':			/* reset alpha parameters */
	alfrsp();
	break;
    default:
	put_back = e;
	break;
    }
}

/*
 * btof - get a floating-point number from the input stream
 */
static double 
btof()
{
    register unsigned int e;
    int     frac, neg;
    register double x, f;
    frac = 0;
    f = 1.;
    gete();
    while (e == ' ' || e == '\t') {	/* skip over white space */
	gete();
    }
    neg = 1;
    if (e == '-') {		/* look for minus sign */
	gete();
	neg = -1;
    } else if (e == '+') {	/* look for plus sign */
	gete();
    }
    for (x = 0.; e != 0; gete()) {
	if (e == '.') {		/* decimal point */
	    if (frac == 0) {
		frac = 1;
		continue;
	    } else {
		break;
	    }
	} else if (e < '0' || '9' < e) {
	    break;
	} else if (frac == 0) {	/* left of decimal */
	    x = 10.* x + (double) (e - '0');
	} else {		/* right of decimal */
	    f *=.1;
	    x += (double) (e - '0') * f;
	}
    }
    put_back = e;
    return ((double) neg * x);
}

/*
 * devres - device reset
 */
void
devres(int num)
{
    static int px1 = 0, py1 = 0, px2 = 4095, py2 = 3019;	/* plotting limits */
    static float segmin = 1.;	/* minimum arc segment */
    extern int mode;

/*
 *	# place pen outside plotting range
 */
	tcom_.tx = px2 + 1;
    tcom_.ty = py2 + 1;
    tcom_.currx = tcom_.tx;
    tcom_.curry = tcom_.ty;
/*
 *	# move to load point
 */
    tmove(px2, py2);
/*
	# set viewport
 */
    vuport_.vx1 = px1;
    vuport_.vy1 = py1;
    vuport_.vx2 = px1;
    vuport_.vy2 = py2;
    arccom_.arcsmo = 0.5;	/* arc smoothness */
    alfrsp();			/* reset alpha parameters */
}

/*
 * alfrsp - reset alpha parameters
 */
#define	CHARSP 56
#define	LINESP 88
#define	ROTANG  0
#define	SLNANG  0
#define	STDFNT  0
#define	ALTFNT 11
#define	FONT    0
#define	SPACON  0
void
alfrsp()
{
    alfcom_.charsp = CHARSP;
    alfcom_.linesp = LINESP;
    alfcom_.rotang = ROTANG;
    alfcom_.slnang = SLNANG;
    alfctr(alfcom_.charsp, alfcom_.linesp, alfcom_.rotang,
	alfcom_.slnang, alfcom_.tr);
}

/*
 * alfctr - comput coordinate transformation
 */
void
alfctr_(int *pawidth, int *pahgt, int *parot, int *pasln, float *ptr)
{
    alfctr(*pawidth, *pahgt, *parot, *pasln, ptr);
}

#define MAX0(a,b)	( ((a) < (b)) ? (b) : (a))
#define MIN0(a,b)	( ((a) < (b)) ? (a) : (b))

void
alfctr(int awidth, int ahgt, int arot, int asln, float *ptr)
{
    double  rot, sr, cr, sln, ts, wid, hgt;
    static double degrad = 3.1415927 / 180.;	/* degrees to radians */

    rot = arot * degrad;
    sr = sin(rot);		/* sine of arot */
    cr = cos(rot);		/* cosine of arot */
    sln = MAX0(-89, MIN0(89, asln)) * degrad;
    ts = sin(sln) / cos(sln);
    wid = awidth / 55.;
    hgt = ahgt / 105.;
    TR(1, 1) = cr * wid;
    TR(1, 2) = (-sr) * hgt + cr * hgt * ts;
    TR(2, 1) = sr * wid;
    TR(2, 2) = cr * hgt + sr * hgt * ts;
}

/*
 * gettkb - get a byte from the tek file 
 */
int
gettkb()
{
    int     c;

    if (put_back != 0) {
	c = put_back;
	put_back = 0;
    } else {
	c = getc(tekfp);
    }
    return (c);
}

/*
 * getnum - get a number from the input stream
 */
int
getnum()
{
    int     gettkb_();
    int     num, e;

    num = 0;
    e = gettkb();

    while ('0' <= e && e <= '9') {
	num = num * 10 + (e - '0');
	e = gettkb();
    }
    put_back = e;
    return (num);
}
