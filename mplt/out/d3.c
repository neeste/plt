/*
 * d3 - subroutines for producing hpgl plotter commands
 */

#include <stdio.h>
#include <string.h>
#include "copa.h"
#include "output.h"
#include "device.h"

#define ADUPI 950
#define UP	0
#define DOWN	1
#define putmsg(s)	fputs(s,stderr)

static void putout(char *);
static void begin_page(void);
static void end_page(void);

static int penpos = UP;
static int lastcolor = 0;
static int plot_yet = 0;
static int page_no = 0;

/* device 3 - HPGL pen plotter */
int     d3open(void);
void    d3close(void), d3page(void);
void    d3trns(double, double, int);

/*
 * d3bind - override default device function
 */
void
d3bind(void)
{
    d_open = d3open;
    d_close = d3close;
    d_trns = d3trns;
    d_page = d3page;
}

/*
 * d3open - initialize plotting device
 */
int
d3open(void)
{
    dev_.adupi = ADUPI;
    dev_.txpr = TXPRSTK;
    dev_.maxpen = 8;
    dev_.prompt = 1;
    dev_.border = 0;
    dev_.fillwhite = -1;	/* not possible to write white */
    return (1);
}

/*
 * d3close - close plotting device
 */
void
d3close(void)
{
    end_page();
    if (dev_.prompt != 0) {
	putmsg("\nDone ... \007\n");	/* bell */
    }
}

/*
 * d3trns - convert NC to hpgl and move or draw
 *
 *	xx,yy	coordinates in inches (NC)
 *	do_gs	0 = move
 *		1 = draw
 */
void
d3trns(double xx, double yy, int do_gs)
{
    char    s[20];
    int     ix, iy;

    if (dev_.fgci != lastcolor) {
	if (penpos != UP) {
	    putout("PU;");
	    penpos = UP;
	}
	sprintf(s, "SP%d;", dev_.fgci);
	putout(s);
	lastcolor = dev_.fgci;
    }
    if (do_gs != penpos) {
	if (do_gs == UP) {
	    putout("PU;");
	    penpos = UP;
	} else {
	    putout("PD;");
	    penpos = DOWN;
	}
    }
    ix = (int) (xx * ADUPI);
    iy = (int) (yy * ADUPI);
    sprintf(s, "PA%d,%d;", ix, iy);
    putout(s);
}

/*
 * d3page - signal new page (media change)
 */
void
d3page(void)
{
    end_page();
    begin_page();
}

static void
putout(char *s)
{
    static int nput = 0;
    int     nc;

    if (!plot_yet)
	begin_page();
    nc = (int)strlen(s);
    if (nput + nc > 72 || nc == 0) {
	fputs("\n", fpout);
	nput = 0;
    }
    fputs(s, fpout);
    nput += nc;
}

static void
begin_page(void)
{
    if (dev_.prompt != 0) {
	putmsg("\nGet plotter ready and hit return ...\007\n");
	while (getchar()!= '\n')
	    continue;
    }
    plot_yet = 1;		/* set this before calling putout to avoid a
				   recursive loop */
    if (page_no == 0) {
	putout("\033.(");	/* plotter on */
	putout("\033.I81;;17:");/* set handshake mode xon/xoff */
	putout("\033.N;19:");	/* set handshake mode xon/xoff */
	putout("IN;SC;PU;");	/* initialize defaults & pen up */
	penpos = UP;
	lastcolor = 0;
    }
    putout("VS8;");		/* pen velocity = 8 cm/sec */
    page_no++;
}

static void
end_page(void)
{
    putout("PU;SP;");
    putout("");			/* force a new line */
    fflush(fpout);
    penpos = UP;
    lastcolor = 0;
    plot_yet = 0;
}
