/* marker.c
 *
 *    PURPOSE:	Plot a centered symbol
 *
 *    USAGE:
 *		marker(n,px,py)
 *
 *    WHERE:
 *	px,py = Arrays of coordinates of symbols to be plotted
 *	n     = Number of points
 *
 *************************************************************************
 */
#include "output.h"
#include "device.h"
#include "copa.h"
#include "clip.h"
#include "cview.h"
#include "font.h"

#define BLACK 15
#define WHITE 0
#define sizfac(k)   (1.0 + symfnt->xadv[k] * 0.01)
#define symtyp(k)   (symfnt->xlft[k])

#define MAXN 512
#define DRAW 0
#define MOVE 1
#define CIRC 5
#define ARC  6
#define ENDC 7

int     getnodes(short *, short *, int, TRANSF *, float *, float *, char *, int);
void    connect_nodes(float *, float *, char *, int, int);
void    dranodes(short *, short *, int, TRANSF *, int);
void    outlinarea(int, float *, float *);
void    penwid(double);
void    pencolor(int, int);
void    setdch(void);
void    skpnwd(double);
void    sktext(double, double, char *);
void    tpcl(double, double, double, double);
void    getbox(int, float  *, float  *, float  *, float  *, float  *, float  *);
FONT   *fntsym(void);

static void drasym(double, double);
static void sym_marker(int, float *, float *, int);
static void chr_marker(int, float *, float *, int);

static char zz[MAXN];
static double size, pw;
static float xx[MAXN], yy[MAXN];
static int type, nsides, ndx0, ndy0;
static short *fnode, *lnode;
static FONT *symfnt;
static TRANSF ndt;

void
marker(int n, float *px, float *py)
{
    int     k;

    k = copa_.curmkt;
    if (0 <= k && k < 32)
        sym_marker(n, px, py, k);
    if (k < -32 || k > 32)
        chr_marker(n, px, py, k);
}

static void
sym_marker(int n, float *px, float *py, int k)
{
    float   a;

    if (k < 0)  /* check range of symbol code */
        return;
    symfnt = fntsym();
    ndx0 = symfnt->fnx0;
    ndy0 = symfnt->fny0;
    type = symtyp(k);
    fnode = symfnt->node + symfnt->khar[k] - 1;
    lnode = symfnt->node + symfnt->khar[k + 1] - 2;
    size = copa_.curmks * sizfac(k) / symfnt->fchh;
    if (copa_.curplw > 0)
	pw = (type == 3) ? copa_.curmks * 0.02 : copa_.curplw * 0.005;
    else
	pw = copa_.curplw;
    nsides = limit(8, (int) (32 * size * dev_.adupi * cview_.vgn), 64);

    a = (float) size * cview_.vgn;
    switch (copa_.rot) {
    case 0:
	ndt.txx = copa_.curmkuy * a;
	ndt.txy = copa_.curmkux * a;
	break;
    case 1:
	ndt.txx = copa_.curmkux * a;
	ndt.txy = -copa_.curmkuy * a;
	break;
    case 2:
	ndt.txx = -copa_.curmkuy * a;
	ndt.txy = -copa_.curmkux * a;
	break;
    case 3:
	ndt.txx = -copa_.curmkux * a;
	ndt.txy = copa_.curmkuy * a;
	break;
    }
    ndt.tyx = -ndt.txy;
    ndt.tyy = ndt.txx;

    while (0 < n--)
	drasym(*px++, *py++);
}

static void
chr_marker(int n, float *px, float *py, int k)
{
    char s[2];
    double stdwid, reqwid;
    double chh, chux, chuy, chsx, chsy, chxp, chsp;
    int txft, txalh, txalv;
    
    chh = copa_.curchh;
    chux = copa_.curchux;
    chuy = copa_.curchuy;
    chsx = copa_.curchsx;
    chsy = copa_.curchsy;
    chxp = copa_.curchxp;
    chsp = copa_.curchsp;
    txft = copa_.curtxft;
    txalh = copa_.curtxalh;
    txalv = copa_.curtxalv;
    copa_.curchh = copa_.curmks;    /* character height = marker size */
    copa_.curchux = 0;
    copa_.curchuy = 1;
    copa_.curchsx = 1;
    copa_.curchsy = 0;
    copa_.curchxp = 1;
    copa_.curchsp = 0;
    copa_.curtxft = (k >= 0) ? STDFONT : ALTFONT;
    copa_.curtxalh = 2;             /* horiz. align. = center */
    copa_.curtxalv = 3;             /* vert. align. = half */
    s[0] = (k >= 0) ? k : -k;
    s[1] = 0;
    pencolor(copa_.curmkci, 0);
    setdch();			    /* set device character variables */
    stdwid = 0.125 * copa_.curchh * cview_.vgn;
    reqwid = (copa_.curtxlw > 0.0) ? copa_.curtxlw * stdwid : copa_.curtxlw;
    skpnwd(reqwid);
    while (0 < n--)
	sktext(*px++, *py++, s);
    copa_.curchh = (float) chh;
    copa_.curchux = (float) chux;
    copa_.curchuy = (float) chuy;
    copa_.curchsx = (float) chsx;
    copa_.curchsy = (float) chsy;
    copa_.curchxp = (float) chxp;
    copa_.curchsp = (float) chsp;
    copa_.curtxft = txft;
    copa_.curtxalh = txalh;
    copa_.curtxalv = txalv;
}

static int
close_fig(float *x, float *y, char *z, int n)
{
    if (x[n - 1] != x[0] || y[n - 1] != y[0]) {
	x[n] = x[0];
	y[n] = y[0];
	z[n] = 0;
	n++;
    }
    return (n);
}

/*
 * drasym - draw a symbol using table of symbol node definitions
 */
static void
drasym(double xll, double yll)
{
    int     np, fc, fb;
    float   xllt, yllt;
    float   bx1, by1, bx2, by2;
    static int clipyet = 0;

    otrans(xll, yll, &xllt, &yllt);
    ndt.txo = xllt - (ndt.txx + ndt.txy) * ndx0;
    ndt.tyo = yllt - (ndt.tyx + ndt.tyy) * ndy0;
    switch (type) {
    case 1:
    case 2:
	np = getnodes(fnode, lnode, -nsides, &ndt, xx, yy, zz, MAXN - 1);
	np = close_fig(xx, yy, zz, np);
	getbox(np, xx, yy, &bx1, &by1, &bx2, &by2);
	if (bx1 > XMX || by1 > YMX || bx2 < XMN || by2 < YMN)
	    break;
	if ((type == 2) || copa_.curmkfw) {
	    if (type == 2) {
		fb = 1;
	        fc = (copa_.curmkfi >= 0) ? copa_.curmkfi : copa_.curmkci;
	        pencolor(fc, 0);
		penwid(pw);
	    } else {
	        pencolor(0, 0);	/* needed to reset dev_.gray */
		fb = 0;
	    }
	    DEV_FILL(np, xx, yy, fb, 1);
	}
        pencolor(copa_.curmkci, 0);
        penwid(pw);
	np = getnodes(fnode, lnode, nsides, &ndt, xx, yy, zz, MAXN - 1);
	np = close_fig(xx, yy, zz, np);
        connect_nodes(xx, yy, zz, np, 0);
	break;
    case 0:
    case 3:
        pencolor(copa_.curmkci, 0);
        penwid(pw);
	dranodes(fnode, lnode, nsides, &ndt, clipyet);
	break;
    }
}
