/*
 * d0 - subroutines for producing tek 4663 commands
 */

#include <stdio.h>
#include "fort.h"
#include "copa.h"
#include "output.h"
#include "device.h"
#define ADUPI 390.		/* addressable device units per inch */
#define puttek(s)	fputs(s,fpout)

static int lpen = 0;

/* device 0 - Tektronix pen plotter */
int     d0open(void);
void    d0close(void), d0page(void);
void    d0trns(double, double, int);
void    d0pnwd(double);

/*
 * d0bind - override default device function
 */
void
d0bind(void)
{
    d_open = d0open;
    d_close = d0close;
    d_trns = d0trns;
    d_page = d0page;
    d_pnwd = d0pnwd;
}

/*
 * d0open - initialize plotting device
 */
int
d0open(void)
{
    puttek("\033AE\033AN\033AV");
    lpen = 0;
    dev_.adupi = ADUPI;
    dev_.txpr = TXPRSTR;
    dev_.maxpen = 9;
    dev_.prompt = 0;
    dev_.border = 0;
    dev_.fillwhite = -1;	/* not possible to write white */
    return (1);
}

/*
 * d0close - close plotting device
 */
void
d0close(void)
{
}

/*
 * d0trns - convert NC to Tektronix byte string and move or draw
 *
 *	nx,yy	coordinates in inches  (NC)
 *	do_gs	0 = move
 *		1 = draw
 */
void
d0trns(double xx, double yy, int do_gs)
{
    char    buf[8], *bp;
    int     x, y;
    x = (int) (xx * ADUPI);
    y = (int) (yy * ADUPI);
    bp = buf;
    if (do_gs == 0.) {
	*bp++ = '\035';		/* GS char. is used to enable graphics mode */
    }
    *bp++ = ' ' + (y >> 7);	/* hiy */
    *bp++ = '`' + (x & 3) + ((y & 3) << 2);	/* eb */
    *bp++ = '`' + ((y & 127) >> 2);	/* loy */
    *bp++ = ' ' + (x >> 7);	/* hix */
    *bp++ = '@' + ((x & 127) >> 2);	/* lox */
    *bp = 0;
    puttek(buf);
}

/*
 * d0page - signal new page (media change)
 */
void
d0page(void)
{
    puttek("\033AK");
}

/*
 * d0pnwd - places tek 4662 instruction for pen
 */
void
d0pnwd(double pw)
{
    char    strbuf[20];
    int     mpen;

    mpen = (int) ((pw > 0) ? 200.0 * pw + 0.5 : -pw);
    mpen = limit(1, mpen, dev_.maxpen);	/* force into range */
    if (mpen != lpen) {
	sprintf(strbuf, "\033ABP%d", mpen);
	puttek(strbuf);
	lpen = mpen;
    }
}
