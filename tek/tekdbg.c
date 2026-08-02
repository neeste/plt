/* tekdbg.c */

#include <stdio.h>
#include <stdlib.h>
#include "alfcom.h"
#include "arccom.h"
#include "tek.h"

/*
 tekdbg - filters files from plt and sends to retrographic terminals
 */

FILE *ttyfp;

int
main(int argc, char **argv)
{
    tekcat(argc, argv);
    return(0);
}

/*
 pdopen - open the plotting device
 */
void
pdopen()
{
    arccom_.segmin = 2.;		/* # minimum arc segment length */
    arccom_.arcsmo = .5;
    printf("open\n");
}

/*
 pmove - move pen to (ix,iy)
 */
void
pmove(int ix, int iy)
{
    printf("move %d %d\n", ix, iy);
}

/*
 pdraw - draw a line segment from current location to (ix,iy)
 */
void
pdraw(int ix, int iy)
{
    printf( "draw %d %d\n", ix, iy);
}

/*
 ppage - erases screen and home
 */
void
ppage()
{
    printf("page\n");
}

/*
 ppen - does nothing
 */
void
ppen(int bpen)
{
    printf ("pen '%c'\n", bpen);
}

/*
 pdclose
 */
void
pdclose()
{
    printf("close\n");
}

