/*
 * d9 - dummy device as placeholder for new output format
 */

#include <stdio.h>
#include "copa.h"
#include "output.h"
#include "device.h"

#define DEFAULT_DEVICE 1

static float adupi = 1000.0;

/* device 9 - NULL device */
int     d9open(void);
void    d9close(void), d9page(void);
void    d9trns(double, double, int);
void    d9pnwd(double);
void    d9fill(int, float *, float *, int, int);

/*
 * d9bind - overide default device function
 */
void
d9bind(void)
{
    d_open = d9open;
    d_close = d9close;
    d_trns = d9trns;
    d_page = d9page;
    d_pnwd = d9pnwd;
    d_fill = d9fill;
}

/*
 * d9open - initialize plotting device
 */
int
d9open(void)
{
    dev_.adupi = adupi;		/* addressable units per inch */
    dev_.txpr = TXPRSTK;	/* text precision = stroke */
    dev_.prompt = 0;		/* prompt user at end of page */
    dev_.border = 0;		/* draw page border */
    dev_.maxpen = 0;		/* maximum pen value */
    dev_.fillwhite = -1;	/* not possible to write white */
    printf("open\n");
    return (1);			/* 0 = unsuccessful open, 1 = successful open */
}

/*
 * d9close - close plotting device
 */
void
d9close(void)
{
    printf("close\n");
}

/*
 * d9trns - convert NC to device units and move or draw
 *
 *	xx,yy	coordinates in inches (NC) already clipped to page boundary:
 *		0 <= xx <= XPAGE  and  0 <= yy <= YPAGE
 *	do_gs	move/draw flag: 0 = move, 1 = draw
 */
void
d9trns(double xx, double yy, int do_gs)
{
    printf("%s to (%6.2f, %6.2f)\n", (do_gs) ? "draw" : "move", xx, yy);
}

/*
 * d9page - signal new page (media change)
 */
void
d9page(void)
{
    printf("page\n");
}

/*
 * d9pnwd -
 */
void
d9pnwd(double pw)
{
    static double cpw = 0.0;

    if (pw != cpw) {
	printf("pnwd: pw=%.3f\n", pw);
	cpw = pw;
    }
}

void
d9fill(int n, float *px, float *py, int fg, int nc)
{
    int     i;

    printf("fill: n=%d nc=%d fg=%d\n", n, nc, fg);
    for (i = 0; i < n; i++)
	printf("    (%6.2f, %6.2f)\n", px[i], py[i]);
}
