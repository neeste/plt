/*
 * d4 - dummy device as placeholder for system dependent graphics
 */

#include <stdio.h>
#include "copa.h"
#include "output.h"
#include "device.h"

#define DEFAULT_DEVICE 1

static float adupi = 1000.0;

/* device 4 - system dependent graphic device */
int     d4open();
void    d4close(), d4trns(), d4page();

/*
 * d4bind - override default device function
 */
void
d4bind()
{
    d_open = d4open;
    d_close = d4close;
    d_trns = d4trns;
    d_page = d4page;
}

/*
 * d4open - initialize plotting device
 */
int
d4open()
{
    dev_.adupi = adupi;		/* addressable units per inch */
    dev_.txpr = TXPRSTK;	/* text precision = stroke */
    dev_.prompt = 0;		/* prompt user at end of page */
    dev_.border = 0;		/* draw page border */
    dev_.maxpen = 0;		/* maximum pen value */
    dev_.fillwhite = -1;	/* not possible to write white */
    return (0);			/* 0 = unsuccessful open, 1 = successful open */
}

/*
 * d4close - close plotting device
 */
void
d4close()
{
}

/*
 * d4trns - convert NC to device units and move or draw
 *
 *	xx,yy	coordinates in inches (NC) already clipped to page boundary:
 *		0 <= xx <= XPAGE  and  0 <= yy <= YPAGE
 *	do_gs	move/draw flag: 0 = move, 1 = draw
 */
void
d4trns(xx, yy, do_gs)
double  xx, yy;
int     do_gs;
{
    printf("%s to (%6.2f, %6.2f)\n", (do_gs) ? "draw" : "move", xx, yy);
}

/*
 * d4page - signal new page (media change)
 */
void
d4page()
{
}

int
chkdev()
{
    return (DEFAULT_DEVICE);
}
