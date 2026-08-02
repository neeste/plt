/* tekout.c */

#include <stdio.h>
#include <stdlib.h>
#include "arccom.h"
#include "tek.h"

/*
 tekout - filters files from plt and sends to stdout
 */

#define ANSI 60
#define CLEAR 74
#define CNTRLX 24
#define CONSI 5
#define ESC ''
#define FF  ''
#define GS 29
#define HOME 72
#define LOX0 64
#define LOY0 96
#define HIX0 32
#define HIY0 32
#define EB0 96
#define SYNC 23
#define STDOUT 6

FILE *ttyfp;


main(argc, argv)
    int argc;
    char *argv[];
{
    tekcat(argc, argv);
    exit(0);
}


/*
# pdopen - open the plotting device
 */
pdopen()
{
    arccom_.segmin = 1.;		/* # minimum arc segment length */
    arccom_.arcsmo = .0;
}

/*
# pmove - move pen to (ix,iy)
 */
pmove(ix,iy)
    int ix, iy;
{
    putchar(GS);
    pdraw(ix,iy);
}

#define max0(a,b) (((a) < (b)) ? (b) : (a))
#define min0(a,b) (((a) < (b)) ? (a) : (b))

/*
# pdraw - draw a line segment from current location to (ix,iy)
 */
pdraw(ix,iy)
    int ix, iy;
{
    char hix,hiy,lox,loy,eb;
    int x,y;

    x = max0(0, min0(ix, 4095));
    y = max0(0, min0(iy, 4095));
    hiy = HIY0 + y/128;			/* # hiy */
    eb = LOY0 + x%4 + (y%4)*4;		/* # eb */
    loy = LOY0 + (y%128)/4;		/* # loy */
    hix = HIX0 + x/128;			/* # hix */
    lox = LOX0 + (x%128)/4;		/* # lox */
    putchar(hiy);
    putchar(eb );
    putchar(loy);
    putchar(hix);
    putchar(lox);
}

/*
# ppage - erases screen and home
 */
ppage()
{
    pmove(0,0);
    perase();		/* # erase */
}

/*
# ppen - does nothing
 */
ppen(bpen)
    int bpen;
{
    return;
}

/*
# pdclose
 */
pdclose()
{
    pmove(0,0);
    fflush(stdout);
}

/*
# perase
 */
perase()
{
    putchar(ESC); putchar(FF);		/* # erase */
}
