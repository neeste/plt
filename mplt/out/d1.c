/*
 * d1 - subroutines for producing tek 4010 commands
 */

#include <stdio.h>
#include "copa.h"
#include "output.h"
#include "device.h"

#define ADUPI (390.0 * 0.999 * 0.25)	/* addressable device units/inch */
#define puttek(s)	fputs(s, fpout)

static void begin_page(void);
static void end_page(void);
static void box(double, double, double, double);

static int cleared = 0;
static int oldhiy = 0, oldloy = 0, oldhix = 0;

/* device 1 - Tektronix graphics terminal */
int     d1open(void);
void    d1close(void), d1page(void);
void    d1trns(double, double, int);

/*
 * d1bind - override default device function
 */
void
d1bind(void)
{
    d_open = d1open;
    d_close = d1close;
    d_trns = d1trns;
    d_page = d1page;
}

/*
 * d1open - initialize plotting device
 */
int
d1open(void)
{
    dev_.adupi = (float) ADUPI;
    dev_.txpr = TXPRSTK;
    dev_.maxpen = 1;
    dev_.prompt = 1;
    dev_.border = 1;
    dev_.fillwhite = -1;	/* not possible to write white */
    puttek("\037");		/* switch to alpha mode */
    puttek("\033\f");		/* erase */
    puttek("\035");		/* switch to graphics mode */
    puttek("\033ML1");		/* select line index with foreground color */
    cleared = 0;
    return (1);
}

/*
 * d1close - close plotting device
 */
void
d1close(void)
{
    end_page();
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);
    puttek("\033\f\0332\033\"0g");
    puttek("\b\b\b\b    \b\b\b\b");
    puttek("\033[?38l");	/* put kermit in text mode */
    puttek("\b\b\b\b    \b\b\b\b");
}

/*
 * d1trns - convert NC to tektronix byte string and move or draw
 *
 *	xx,yy	coordinates in inches (NC)
 *	do_gs	0 = move
 *		1 = draw
 */
void
d1trns(double xx, double yy, int do_gs)
{
    char    buf[8], *bp;
    char    hiy, loy, hix, lox;
    int     x, y;

    begin_page();
    x = (int) (xx * ADUPI);
    y = (int) (yy * ADUPI);
    hiy = ' ' + (y >> 5);
    loy = '`' + (y & 31);
    hix = ' ' + (x >> 5);
    lox = '@' + (x & 31);
    bp = buf;
    if (do_gs == 0) {
	*bp++ = '\035';		/* GS char. is used to enable graphics mode */
    }
    if (hiy != oldhiy)
	*bp++ = hiy;
    if (loy != oldloy || hiy != oldhiy || hix != oldhix)
	*bp++ = loy;
    if (hix != oldhix)
	*bp++ = hix;
    *bp++ = lox;
    *bp = 0;
    puttek(buf);
    oldhiy = hiy;
    oldloy = loy;
    oldhix = hix;
}

/*
 * d1page - signal new page (media change)
 */
void
d1page(void)
{
    end_page();
}

static void
begin_page(void)
{
    if (cleared)
	return;
    cleared = 1;
    puttek("\033\f");		/* erase */
    oldhix = oldhiy = oldloy = 0;
    if (dev_.border)
	box(0.0, 0.0, 10.5, 8.0);
}

static void
end_page(void)
{
    if (!cleared)
	return;
    cleared = 0;
    if (dev_.prompt) {
	puttek("\r\007");	/* bell */
	fflush(stdout);
	while (getchar() >= ' ')
		continue;
    }
}

/* box - draw box
*/
static void
box(double x1, double y1, double x2, double y2)
{
    d1trns(x1, y2, 0);
    d1trns(x1, y1, 1);
    d1trns(x2, y1, 1);
    d1trns(x2, y2, 1);
    d1trns(x1, y2, 1);
    fflush(stdout);
}

