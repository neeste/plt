/* tekcvt.c */

#include <stdio.h>
#include "alfcom.h"

#define	AMODE	 1
#define	ESC	27
#define	GMODE	 2
#define	GS	29
#define	LF	10
#define OFF	 0
#define ON	 1
#define	SI	15
#define	SO	14
#define	SYN	22
#define	US	31

void alfa();
void esccod();
void grfmod();
void set_altfont();
void set_stdfont();
void tekcvt();

extern int plotterstatus;

/*
 tekcvt - convert a tek file to move and draw commands
 */

void
tekcvt()
{
   int b, dflag, gettkb();
   int x=0, y=0;
   int mode;

   dflag = 0;
   while( ( b = gettkb() ) != EOF ) {
	if ( b == ESC ) {		/* # look for escape codes */
	    esccod(&x,&y);
	} else if ( plotterstatus == OFF ) {	/* # plotter is OFF */
	    fputc ( b, stderr );
	} else {			/* # plotter is ON */
	  switch( b ) {
	      case LF:			/* # ignore line feeds */
	      break;
	      case SYN:			/* # ignore line sync */
	      break;
	      case SO:			/* # set alternate font */
		  set_altfont();
		  alfcom_.font=alfcom_.altfnt;
	      break;
	      case SI:			/* # set standard font */
		  set_stdfont();
		  alfcom_.font = alfcom_.stdfnt;
	      break;
	      case US:			/* # enter alpha mode */
		  mode = AMODE;
	      break;
	      case GS:			/* # enter graphics mode */
		  mode = GMODE;
		  dflag = 0;		/* # move to next (x,y) */
	      break;
	      default:
		  if (mode == GMODE)	/* # process byte in graphics mode */
		      grfmod(b,&x,&y,&dflag);
		  else
		      alfa(b,&x,&y);	/* # process byte in alpha mode */
	      break;
	  }
      }
   }
}

