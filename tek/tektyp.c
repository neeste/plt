/* tektyp.c */

#include <stdio.h>
#include <stdlib.h>
#include "arccom.h"
#include "tek.h"

#define STDOUT 6
#define STDIN 5
#define ERROUT 0
#define XMAX 80
#define YMAX 24
#define BELL '\07'
#define BUF(r,c)   cpplot_.buf[(((r)-1) + ((c)-1)*XMAX)]

void doplot();
void pwait();

static double sf = (10.5 / 4096);

/*
 * tektyp - output plot file to console terminal
 */

int
main(int argc, char **argv)
{
    tekcat(argc, argv);
    return(0);
}


static struct scale {
    float   xsc, ysc;
}       scale_;

static struct cpplot {
    char    buf[XMAX * YMAX];
    int     ixpen, iypen;
    char    symbol;
}       cpplot_;

/*
 * pdopen -
 */
void
pdopen()
{
    char   *pbuf, *pend;
    arccom_.segmin = 39.;	/* minimum arc segment length */
    arccom_.arcsmo = 1.;
/*
 * Scaled for tektronics 4663 addressable graphics units (ADU)
 * Page format is `A-DRAFTING`
 * X = 10.5 inches = 4096 ADU
 * Y = 7.74 inches = 3019.23 ADU
 */
    scale_.xsc = 7 * sf;	/* 7 dots per inch in x dimension */
    scale_.ysc = 3 * sf;	/* 3 dots per inch in y dimension */


    for (pbuf = cpplot_.buf, pend = pbuf + YMAX * XMAX; pbuf < pend;)
	*pbuf++ = ' ';
    cpplot_.symbol = '*';
}

/*
 * pdclose -
 */

void
pdclose()
{
    doplot();
    pwait();
}

#define NINT(a) ( (int) (((a) < 0.0) ? (a) - 0.5 : (a) + 0.5))

/*
 * pmove - move the pen to (ixm,iym) without drawing
 */
void
pmove(int ix, int iy)
{
    cpplot_.ixpen = ix * scale_.xsc + 1.;
    cpplot_.iypen = iy * scale_.ysc + 1.;
}

/*
 * pdraw - draw a line from current pen position to the point (ixp,iyp)
 */
void
pdraw(int ix, int iy)
{
    double  x, y, dx, dy;
    int     ixp, iyp, ixdif, iydif, ixdis, iydis, idx, idy;

    ixp = ix * scale_.xsc + 1.;
    iyp = iy * scale_.ysc + 1.;
    ixdif = ixp - cpplot_.ixpen;
    iydif = iyp - cpplot_.iypen;
    ixdis = abs(ixdif);
    iydis = abs(iydif);
    BUF(cpplot_.ixpen, cpplot_.iypen) = cpplot_.symbol;
    if ((ixdis == 0) && (iydis == 0))
	return;
    if (ixdis > iydis) {
	idx = ixdif / ixdis;
	dy = iydif / (double) ixdif;
	y = cpplot_.iypen;
	for (ix = cpplot_.ixpen; ix != ixp; ix += idx) {
	    BUF(ix, NINT(y)) = cpplot_.symbol;
	    y += dy;
	}
    } else {
	dx = ixdif / (double) iydif;
	x = cpplot_.ixpen;
	idy = iydif / iydis;
	for (iy = cpplot_.iypen; iy != iyp; iy += idy) {
	    BUF(NINT(x), iy) = cpplot_.symbol;
	    x += dx;
	}
    }
    BUF(ixp, iyp) = cpplot_.symbol;
    cpplot_.ixpen = ixp;
    cpplot_.iypen = iyp;
}

/*
 * ppen -
 */
void
ppen(int ipen)
{
    cpplot_.symbol = ipen;
}

/*
 * ppage -
 */
void
ppage()
{
    char   *pbuf, *pend;
    doplot();
    for (pbuf = cpplot_.buf, pend = pbuf + XMAX * YMAX; pbuf < pend;)
	*pbuf++ = ' ';
    pwait();
}

/*
 * pwait
 */
void
pwait()
{
    putchar(BELL);
    fflush(stdout);
    getchar();
}

/*
 * doplot - put out hard copy of plot file to the console
 */
void
doplot()
{
    int     i, j, n;
    for (j = YMAX; j >= 1; j--) {
	for (n = XMAX; BUF(n, j) == ' '; n--)
	    continue;
	for (i = 1; i <= n; i++) {
	    putchar(BUF(i, j));
	}
	putc('\n', stdout);
    }
}

