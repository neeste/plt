/* tekps.c */

#include <stdio.h>
#include <stdlib.h>
#include "arccom.h"
#include "tek.h"

void out_draw();
void out_move();
void opnout();
void penwid();
void clsout();
void page();

static double adupi = (10.5 / 4095);

int
main(int argc, char **argv)
{
    tekcat(argc, argv);
    return(0);
}

/* pdopen - open the plotting device */

void
pdopen()
{
    int     idev;

    arccom_.segmin = 8.;	/* minimum arc segment length */
    arccom_.arcsmo =.5;

    idev = 5;			/* select postscript */
    opnout(idev);		/* open output */
    penwid(0.01);		/* line width = 0.01 inch */
}

/* pmove - move pen to (ix,iy) */

void
pmove(int ix, int iy)
{
    double  x, y;

    x = ix * adupi;
    y = iy * adupi;
    out_move(x, y);
}

/* pdraw - draw a line segment from current location to (ix,iy) */

void
pdraw(int ix, int iy)
{
    double  x, y;

    x = ix * adupi;
    y = iy * adupi;
    out_draw(x, y);
}

/* ppage - erases screen and home */

void
ppage()
{
    page();			/* wait for carriage return from keyboard */
}

/* ppen - does nothing */

void
ppen(int bpen)
{
    return;
}


/* pdclose */

void
pdclose()
{
    clsout();
}

