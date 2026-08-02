/* grfmod.c */

#include "tcom.h"
#include "arccom.h"
#include "stdlib.h"

int inwind();
void pdraw();
void pmove();
void tdraw();
void tmove();

extern int     xhi, yhi, xlo, ylo;

/*
 * grfmod - process byte in graphics mode
 */
void
grfmod(int b, int *xp, int *yp, int *dflagp)
{
   static int xeb=0,yeb=0,xeeb=0,
		yeeb=0,xeeeb=0,yeeeb=0,xflag=0;
   int x, y;

   if(b >= ' ' && b < '@') {		/* # xhi and yhi */
      if(xflag > 0)
         xhi = (b - ' ') % 4096;
      else
         yhi = (b - ' ') % 4096;
   } else if(b >= '@' && b < '`') {		/* # xlo and action byte */
      xlo = (b - '@') % 4096;
      x = xhi*128 + xlo*4 + xeb;
      y = yhi*128 + ylo*4 + yeb;
      *xp = x;
      *yp = y;
      if(*dflagp==0)
         tmove(x,y);			/* # move command */
      else
         tdraw(x,y);			/* # draw command */
      *dflagp=1;				/* # draw to next (x,y) */
      xflag=0;
   } else if(b >= '`') {		/* # ylo and extended bits xeb,yeb */
      xeeeb = xeeb;
      yeeeb = yeeb;
      xeeb = xeb;
      yeeb = yeb;
      xeb = ylo % 4;
      yeb = (ylo % 16)/4;
      ylo = (b - '`') % 4096;
      xflag=1;
   }
}

/*
 * # tmove - tek move
 */
void
tmove(int x, int y)
{
    tcom_.lastx = tcom_.currx;
    tcom_.lasty = tcom_.curry;
    tcom_.currx = x;
    tcom_.curry = y;
}

/*
 * # tdraw - tek draw
 */
void
tdraw(int x, int y)
{
	int dflag, inwres;

	inwres = inwind(tcom_.currx, tcom_.curry);
	if((tcom_.tx!=tcom_.currx || tcom_.ty!=tcom_.curry) && (inwres>=0)) {
		pmove(tcom_.currx, tcom_.curry);
		dflag=1;
		tcom_.tx=tcom_.currx;
		tcom_.ty=tcom_.curry;
	} else {
	    dflag = (abs(tcom_.currx-x)+abs(tcom_.curry-y))
	    		>= arccom_.segmin*.5;
	}
	if(inwind(x,y)>=0 & inwres>=0 & dflag==1)	/* #draw */
		{
		pdraw(x, y);
		tcom_.tx = x;
		tcom_.ty = y;
		}
	tcom_.lastx = tcom_.currx;
	tcom_.lasty = tcom_.curry;
	tcom_.currx = x;
	tcom_.curry = y;
}

/*
 * # inwind - see if (x,y) is in the window
 */
#define	wx1 0
#define wy1 0
#define wx2 4095
#define wy2 3121

int
inwind(int x, int y)
{
    return ( (wx1 <= x && x <= wx2 && wy1 <= y && y <= wy2) ?
	((wx1 < x && x < wx2 && wy1 < y && y < wy2) ? 1 : 0) : -1);
}

