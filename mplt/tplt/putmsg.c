/*
 *
 * 	putmsg
 *
 * 	PROGRAM NAME:			putmsg.c
 *
 * 	DATE OF  FIRST  VERSION:	11-Nov-82
 * 	DATE OF CURRENT VERSION:	 5 Dec-88
 *
 * 	PURPOSE:	PLT subroutine to plot a message
 *
 * 	USAGE:		putmsg(mesg, nline)
 *
 * 	PROGRAMMER:	E. Javel / M. Prucka
 *
 * *********************************************************************
 */
#include <string.h>
#include "plt.h"
#include "pltblk.h"
#include "fort.h"

extern double inch_unit;

double  inqchh(void);
double  sindeg(double);
double  cosdeg(double);
int     init_frame(void);
void    label(double, double, char *);

static void msgpos(double *, double *);

void
putmsg(char *mesg, int nline)
{
    static double xm, ym;
    static double lspx, lspy;
    double  chux, chuy;

    if (init_frame())
	return;
    if (misc_.ifrom <= misc_.inow && misc_.inow <= misc_.ito) {
	misc_.pltyet = TRUE;
	if (nline == 0) {
	    msgpos(&xm, &ym);
	    chux = -sindeg(param_.msgang);
	    chuy = cosdeg(param_.msgang);
	    setchup(chux, chuy);
	    setchan(param_.mslant);
	    settxal((int) param_.mhalign, (int) param_.mvalign);
	    lspx = chux * size_.hitmsg * param_.mvsp;
	    lspy = chuy * size_.hitmsg * param_.mvsp;
	} else {
	    xm -= lspx;
	    ym -= lspy;
	}
#ifdef DEBUG
	if (flags_.dbflg) {
	    sprintf(ctext_.msg,
		"putmsg: x=%f y=%f '%s'\n", xm, ym, mesg);
	    errout(ctext_.msg);
	}
#endif
	if (strlen(mesg) > 0)
	    label(xm, ym, mesg);
    }
}

static void
msgpos(double *xp, double *yp)
{
    double  xpos(double), ypos(double);

/*
 *     Set the message size and line weight
 */
    setchh(size_.hitmsg);
    setchxp(param_.mhxp);	/* Set character expansion factor */
    setchsp(param_.mhsp);	/* Set character spacing */
    settxlw(size_.msglnw);	/* Set text line weight */
    if (misc_.colors)
	settxci((int) param_.msgcol);	/* Set text color */
    setplw(size_.msglnw);	/* Set plot line weight for symbols in text */
    if (misc_.colors)
	setpci((int) param_.pltcol);

/*
 *     If MSGDAT is not 0, compute MXLLC and MYLLC in terms of data values;
 *     otherwise, use MXLLC and MYLLC in inches.
 */
    if (param_.msgdat != 0. || param_.mxdat != 0.) {
	*xp = xpos(param_.mxllc);
    } else {
	*xp = axes_.rxllc + param_.mxllc;
    }
    if (param_.msgdat != 0. || param_.mydat != 0.) {
	*yp = ypos(param_.myllc);
    } else {
	*yp = axes_.ryllc + param_.myllc;
    }
}
