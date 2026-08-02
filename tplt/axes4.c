/* 
 *  axes4.c - process annotations
 */
#include "plt.h"
#include "pltblk.h"

double  axanno(int, double, double, double, double, double, char *, double);
double  txanno(int, char *, double);
extern float xtsz, ytsz, rtsz, ttsz;
extern float xanlen, yanlen, ranlen, tanlen;

void
axes4(void)
{
    double  hafch, annpos;

/*
 * Process ANNOTATIONS
 */
    xanlen = yanlen = ranlen = tanlen = 0.0;
    if (size_.annlnw == 0)
	return;
    setchh(size_.hitann);	/* Set annotation height */
    setchxp(1.0);		/* Set character expansion factor */
    setchsp(0.0);		/* Set character spacing */
    setchan(0.0);		/* Character slant angle */
    settxlw(size_.annlnw);	/* Set text line weight */
    if (misc_.colors)
	settxci((int) param_.anncol);	/* Set text color */
    hafch = 0.5 * size_.hitann;	/* half height of character */

    /* Bottom segment */
    if (axes_.xaxis && param_.xanskp >= 0.0) {
	annpos = axes_.ryllc - hafch;
	if (xtsz < 0)
	    annpos += xtsz;
	setchup(0.0, 1.0);	/* Character up vector */
	settxal(2, 1);		/* Text alignment = center,top */
	if (ctext_.xannot[0] != '\0') {
	    xanlen = (float) txanno(0, ctext_.xannot, annpos);
	} else {
	    xanlen = (float) axanno(0, axes_.rxmin, axes_.rxmax, param_.xcycle, 
		param_.xint, param_.xanskp, ctext_.xfmt, annpos);
	}
    }
    /* Left segment */
    if (axes_.yaxis && param_.yanskp >= 0.0) {
	annpos = axes_.rxllc - hafch;
	if (ytsz < 0)
	    annpos += ytsz;
	if (param_.yhor != 0.0) {
	    setchup(0.0, 1.0);	/* Character up vector */
	    settxal(3, 3);	/* Text alignment = right,half */
	} else {
	    setchup(-1.0, 0.0);	/* Character up vector */
	    settxal(2, 5);	/* Text alignment = center,bottom */
	}
	if (ctext_.yannot[0] != '\0') {
	    yanlen = (float) txanno(1, ctext_.yannot, annpos);
	} else {
	    yanlen = (float) axanno(1, axes_.rymin, axes_.rymax, param_.ycycle,
		param_.yint, param_.yanskp, ctext_.yfmt, annpos);
	}
    }
    /* Right segment */
    if (axes_.raxis && param_.ranskp >= 0.0
	&& (param_.rmin != 0.0 || param_.rmax != 0.0)) {
	annpos = axes_.rxllc + axes_.rxlen + hafch;
	if (rtsz > 0)
	    annpos += rtsz;
	if (param_.yhor != 0.0) {
	    setchup(0.0, 1.0);	/* Character up vector */
	    settxal(1, 3);	/* Text alignment = left,half */
	} else {
	    setchup(-1.0, 0.0);	/* Character up vector */
	    settxal(2, 1);	/* Text alignment = center,top */
	}
	if (ctext_.rannot[0] != '\0') {
	    ranlen = (float) txanno(2, ctext_.rannot, annpos);
	} else {
	    ranlen = (float) axanno(2, param_.rmin, param_.rmax, param_.rcycle,
		param_.rint, param_.ranskp, ctext_.rfmt, annpos);
	}
    }
    /* Top segment */
    if (axes_.taxis && param_.tanskp >= 0.0
	&& (param_.tmin != 0.0 || param_.tmax != 0.0)) {
	annpos = axes_.ryllc + axes_.rylen + hafch;
	if (ttsz > 0)
	    annpos += ttsz;
	setchup(0.0, 1.0);	/* Character up vector */
	settxal(2, 5);		/* Text alignment = center,bottom */
	if (ctext_.tannot[0] != '\0') {
	    tanlen = (float) txanno(3, ctext_.tannot, annpos);
	} else {
	    tanlen = (float) axanno(3, param_.tmin, param_.tmax, param_.tcycle,
		param_.tint, param_.tanskp, ctext_.tfmt, annpos);
	}
    }
}
