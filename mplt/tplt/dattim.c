/*
 *
 *     dattim
 *
 * 	PROG NAME:			dattim.c
 *
 * 	DATE OF FIRST VERSION:          25-Sep-78
 * 	DATE OF CURRENT VERSION:         9-Sep-88
 *
 * 	PURPOSE:	Writes file name, date of last modifcation of file
 * 			at bottom of plot
 *
 * 	USAGE:	 	dattim()
 *
 *	Functions called:
 *		    errout, marker, setchh, setchan, setchup, setmks, setmkt,
 *		    setmkup, settxal, settxlw, sprintf, text
 *
 *	Called by:
 *		    doplot
 *
 *
 * 	PROGRAMMER:	M. Collins / E. Javel / M. Prucka
 *
 **********************************************************************
 */
#include "plt.h"
#include "pltblk.h"

#define suppr_head(e) (((int)param_.header)&(1<<(e)))

extern double inch_unit;

void
dattim(void)
{
    char    bpag[MAXMSG], *fn;
    float   xx, yy;
    int     i;
    double  sz, x, y, w0, w1, w2, w3, w4, w5, wc;
    static char *inst = "BTNRH";
/*
 * ---------------------------------------------------------------------
 */
/* current page number */
    sprintf(bpag, "Page %d", misc_.inow);

#ifdef DEBUG
    if (flags_.dbflg) {
	sprintf(ctext_.msg,
	    "dattim: %s  %s %s\n", misc_.hdate, misc_.filinm, bpag);
	errout(ctext_.msg);
    }
#endif
/*
 * Plot this stuff at the bottom of the page
 */
    sz = 0.24 * inch_unit;
    setmkt(30);			/* marker type = 30 */
    setmks(sz);			/* marker size = sz (inches) */
    setmkup(0., 1.);		/* marker up vector = (0,1) */
    settxlw(1.0);		/* text line weight = 1 */
    if (misc_.colors)
	settxci((int) param_.fgcol);	/* Set text color */
    setchup(0., 1.);		/* character up vector = (0,1) */
    setchan(10.);		/* character slant angle */
    setchh(.08 * inch_unit);		/* character height = .08 inches */
    setchxp(1.0);		/* Set character expansion factor */
    setchsp(0.0);		/* Set character spacing */
    settxal(1, 5);		/* Set text alignment = left, bottom */

    w1 = w2 = w3 = w4 = w5 = 0;
    
    if (!suppr_head(1)) {	    /* logo */
        xx = (float) (param_.wxllc + 0.10 * inch_unit + 0.5 * sz);
        yy = (float) (param_.wyllc + 0.05 * inch_unit + 0.5 * sz);
	marker(1, &xx, &yy);		
        w1 = 0.10 * inch_unit + sz;
    }
    y = param_.wyllc + 0.05 * inch_unit;
    if (!suppr_head(2)) {	    /* institution */
        x = param_.wxllc + w1 + qtxln(" ");
        text(x, y, inst);
	w2 = qtxln(inst) + qtxln("  ");
    }
    if (!suppr_head(3)) {	    /* date */
        x = param_.wxllc + w1 + w2 + qtxln(" ");
	text(x, y, misc_.hdate);
        w3 = qtxln(misc_.hdate) + qtxln("  ");
    }
    if (!suppr_head(5)) {	    /* page */
        w5 = qtxln(bpag) + qtxln("   ");
        x = param_.wxllc + param_.wxlen - w5 + qtxln(" ");
	text(x, y, bpag);
    }
    if (!suppr_head(4)) {
        fn = misc_.filinm;
	w0 = w1 + w2 + w3;
        w4 = qtxln(fn);
	wc = param_.wxlen - w0 - w4 - w5;
	while ((wc < 0) && *fn) {   /* shorten the filename ??? */
	    fn++;
            w4 = qtxln(fn);
	    wc = param_.wxlen - w0 - w4 - w5;
	}
	if (fn > misc_.filinm) {    /* insert ellipsis to indicate shortened filename */
	    for (i = 0; i < 3; i++) {
		bpag[i] = fn[i];
		fn[i] = '.';
	    }
	}
	if ((w0 > 0) && (w5 == 0)) {
	    x = param_.wxllc + param_.wxlen - w4 - qtxln("  ");
	} else if ((w0 == 0) && (w5 > 0)) {
            x = param_.wxllc  + qtxln(" ");
	} else {
	    x = param_.wxllc + (param_.wxlen + w0 - w4 - w5) / 2;
	}
	text(x, y, fn);
	if (fn > misc_.filinm) {    /* restore filename covered by ellipsis */
	    for (i = 0; i < 3; i++) {
		fn[i] = bpag[i];
	    }
	}
    }
}
