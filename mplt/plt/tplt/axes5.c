/* 
 *  axes5.c - process axis labels
 */
#include "plt.h"
#include "pltblk.h"

extern float xtsz, ytsz, rtsz, ttsz;
extern float xanlen, yanlen, ranlen, tanlen;

void    label(double, double, char *);

void
axes5(void)
{
    double  xpos, ypos, margin, annsiz;
    int     yhoriz;

/*
 * Process AXIS LABELS
 */
    if (size_.lablnw == 0.0)
	return;
    settxlw(size_.lablnw);	/* Set text line weight */
    if (misc_.colors)
	settxci((int) param_.labcol);	/* Set text color */
    setplw(size_.lablnw);	/* Set plot line weight */
    if (misc_.colors)
	setpci((int) param_.pltcol);	/* Set plot color */
    setchh(size_.hitlab);	/* Set character height */
    setchxp(param_.mhxp);	/* Set character expansion factor */
    setchsp(param_.mhsp);	/* Set character spacing */
    setchan(0.0);		/* Character slant angle */

    margin = 0.25 * size_.hitlab;
    annsiz = size_.hitann;
    yhoriz = (param_.yhor == 0.0);

    /* Bottom segment (X) */
    if (axes_.xaxis && ctext_.xlabel[0] != '\0') {
	xpos = axes_.rxllc + 0.5 * axes_.rxlen;
	ypos = axes_.ryllc - margin;
	if (xtsz < 0)
	    ypos += xtsz;
	if (xanlen > 0.0)
	    ypos -= annsiz + annsiz;
	setchup(0.0, 1.0);	/* Set character up vector */
	settxal(2, 1);		/* text alignment = center,top */
	label(xpos, ypos, ctext_.xlabel);
    }
    /* Left segment (Y) */
    if (axes_.yaxis && ctext_.ylabel[0] != '\0') {
	ypos = axes_.ryllc + 0.5 * axes_.rylen;
	xpos = axes_.rxllc - margin;
	if (ytsz < 0)
	    xpos += ytsz;
	if (yanlen > 0.0)
	    xpos -= ((yhoriz) ? annsiz : yanlen) + annsiz;
	setchup(-1.0, 0.0);	/* Set character up vector */
	settxal(2, 5);		/* text alignment = center,bottom */
	label(xpos, ypos, ctext_.ylabel);
    }
    /* Right segment (R) */
    if (axes_.raxis && ctext_.rlabel[0] != '\0') {
	ypos = axes_.ryllc + 0.5 * axes_.rylen;
	xpos = axes_.rxllc + axes_.rxlen + margin;
	if (rtsz > 0)
	    xpos += rtsz;
	if (ranlen > 0.0)
	    xpos += ((yhoriz) ? annsiz : ranlen) + annsiz;
	setchup(-1.0, 0.0);	/* Set character up vector */
	settxal(2, 1);		/* text alignment = center,top */
	label(xpos, ypos, ctext_.rlabel);
    }
    /* Top segment (T) */
    if (axes_.taxis && ctext_.tlabel[0] != '\0') {
	xpos = axes_.rxllc + 0.5 * axes_.rxlen;
	ypos = axes_.ryllc + axes_.rylen + margin;
	if (ttsz > 0)
	    ypos += ttsz;
	if (tanlen > 0.0)
	    ypos += annsiz + annsiz;
	setchup(0.0, 1.0);	/* Set character up vector */
	settxal(2, 5);		/* text alignment = center,bottom */
	label(xpos, ypos, ctext_.tlabel);
    }
}
