/*
 * dranodes - draw lines to connect character nodes
 */

#include <stdio.h>
#include "output.h"
#include "device.h"
#include "font.h"

#define MAXN 512
#define DRAW 0
#define MOVE 1
#define CIRC 5
#define ARC  6
#define ENDC 7

#define LSB  ((int)((char *) &byt_ord)[0])
#define MSB  ((int)((char *) &byt_ord)[1])

int     getarc(double, double, double, int, float *, float *, char *);
void    tpcc(float *, float *);
void    tpcl(double, double, double, double);
void    set_curpnt(double, double);

static char aa[MAXN];
static float xx[MAXN], yy[MAXN];
static short byt_ord = 256;

int
getnodes(short *fnode, short *lnode, int ns, TRANSF *n, 
	 float *xx, float *yy, char *aa, int mx)
{
    double  deg;
    float   nx, ny;
    int     nodtyp, i, ix, iy, nn, pd;
    short  *pnode;

    //
    // fetch nodes from font definition
    //
    nn = 0;
    pd = 0;
	ix = MSB;
	iy = LSB;
    for (pnode = fnode; pnode <= lnode; pnode++) {
	nx = ((unsigned char *) pnode)[ix];
	ny = ((unsigned char *) pnode)[iy];
	nodtyp = (int) ((nx == 255) ? ny : DRAW);
	switch (nodtyp) {
	case DRAW:
	    if (nn >= mx)
		return (nn);
	    xx[nn] = nx;
	    yy[nn] = ny;
	    aa[nn] = pd ? 0 : 2;
	    nn++;
	    pd = 1;
	    break;
	case MOVE:
	    pd = 0;
	    break;
	case CIRC:
	case ARC:
	    if (nn < 1 || nn >= mx - 12)
		return (nn);
	    deg = (nodtyp == CIRC) ? 360 : *++pnode;
	    pnode++;
	    ny = ((unsigned char *) pnode)[LSB];
	    nx = ((unsigned char *) pnode)[MSB];
	    nn--;
	    nn += getarc(nx, ny, deg, ns, xx + nn, yy + nn, aa + nn);
	    pd = 1;
	    break;
	case ENDC:
	    pnode = lnode;
	    break;
	}
    }
    //
    // transform nodes into normalized coordinates
    //
    for (i = 0; i < nn; i++) {
        nx = n->txx * xx[i] + n->txy * yy[i] + n->txo;
        ny = n->tyx * xx[i] + n->tyy * yy[i] + n->tyo;
	xx[i] = nx;
	yy[i] = ny;
    }
    return (nn);
}

void
connect_nodes(float *x, float *y, char *a, int np, int clipyet)
{
    float   xx[4], yy[4];
    double  xp = 0, yp = 0;
    double  xt, yt;
    int     i;

    if (np < 2)
	return;
    xt = *x;
    yt = *y;
    if (clipyet) {
	DEV_TRNS(xt, yt, 0);
    }
    xp = xt;
    yp = yt;
    for (i = 1; i < np; i++) {
	x++;
	y++;
	a++;
	xt = *x;
	yt = *y;
	if (*a == 1) {
	    xx[0] = (float) xp;
	    yy[0] = (float) yp;
	    xx[1] = (float) xt;
	    yy[1] = (float) yt;
	    xx[2] = (float) x[1];
	    yy[2] = (float) y[1];
	    xx[3] = (float) x[2];
	    yy[3] = (float) y[2];
	    tpcc(xx, yy);
	    x += 2;
	    y += 2;
	    a += 2;
	    i += 2;
	    xt = xx[3];
	    yt = yy[3];
	} else if (clipyet) {
	    DEV_TRNS(xt, yt, *a == 0);
	} else if (*a == 0) {
	    tpcl(xp, yp, xt, yt);
	}
	xp = xt;
	yp = yt;
    }
    if (clipyet) {
	set_curpnt(xt, yt);
    }
}

/*
 * dranodes - draw symbol nodes given pointer to table of symbol nodes
 *
 *    Called by:
 *            drasym, drachar
 */
void
dranodes(short *fnode, short *lnode, int nsides, TRANSF *n, int clipyet)
{
    int     nn;

    nn = getnodes(fnode, lnode, nsides, n, xx, yy, aa, MAXN);
    connect_nodes(xx, yy, aa, nn, clipyet);
}
