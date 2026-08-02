/*
 *
 *     setsiz
 *
 *	PROGRAM NAME:			setsiz.c
 *
 *	DATE OF  FIRST  VERSION:	28-Feb-83
 *	DATE OF CURRENT VERSION:	 9-SEP-88
 *
 *	PURPOSE:	PLT routine to establish line weights and
 *			various sizes for plotting, based on the
 *			average axis length and the value of the
 *			parameter SIZFAC:
 *
 *			  If SIZFAC is negative, use the values
 *			    for the previous frame.
 *			  If SIZFAC is zero, then compute the default
 *			    weights and sizes.
 *			  If SIZFAC is positive, then use its value
 *			    as an average axis length on which to
 *			    base the weights and sizes.
 *
 *
 *	USAGE:		setsiz()
 *
 *	Functions called:
 *		    errout, sprintf
 *
 *	Called by:
 *		    doplot, msgpos
 *
 *
 *	PROGRAMMER:	Eric Javel / Matt Prucka
 *
 *****************************************************************
 */
#include "plt.h"
#include "pltblk.h"

void
setsiz(void)
{
/* See if new weights are desired or is previous weights are to be used */
    if (param_.sizfac < 0.)
	return;			/* If previous to be used */
    else if (param_.sizfac == 0.)	/* If no factor provided */
	size_.avgaxl = (param_.xlen + param_.ylen) / 2;
    else
	size_.avgaxl = param_.sizfac;	/* If factor provided */

#ifdef DEBUG
    if (flags_.dbflg) {
	sprintf(ctext_.msg, "setsiz: avgaxl = %f\n", size_.avgaxl);
	errout(ctext_.msg);
    }
#endif

/* Tick length */
    if (param_.ticsiz < 0.)	/* set absolute size */
	size_.tiklen = -param_.ticsiz;
    else
	size_.tiklen = param_.ticsiz * size_.avgaxl / 40;

/* Data symbol height */
    if (param_.symsiz < 0.)
	size_.hitsym = -param_.symsiz;
    else
	size_.hitsym = param_.symsiz * size_.avgaxl / 50;

/* Annotation height */
    if (param_.annsiz < 0.)
	size_.hitann = -param_.annsiz;
    else
	size_.hitann = param_.annsiz * size_.avgaxl / 40;

/* Axis label height */
    if (param_.labsiz < 0.)
	size_.hitlab = -param_.labsiz;
    else
	size_.hitlab = param_.labsiz * size_.avgaxl / 40;

/* Message height */
    if (param_.msgsiz < 0.)
	size_.hitmsg = -param_.msgsiz;
    else
	size_.hitmsg = param_.msgsiz * size_.avgaxl / 40;

/* Arrowhead size */
    if (param_.arosiz < 0.)
	size_.arosiz = -param_.arosiz;
    else
	size_.arosiz = param_.arosiz * size_.avgaxl / 40;

/* Annotation line weight */
    size_.annlnw = param_.annlwt;

/* Label line weight */
    size_.lablnw = param_.lablwt;

/* Message line weight */
    size_.msglnw = param_.msglwt;

/* Plot (data) line weight */
    if (param_.pltlwt < 0.)
	size_.pltlnw = param_.pltlwt;
    else
	size_.pltlnw = (float) (param_.pltlwt * size_.avgaxl / 1.5);

/* Axis line weight */
    if (param_.axlwt < 0.)
	size_.axlnw = param_.axlwt;
    else
	size_.axlnw = param_.axlwt * size_.avgaxl;

/* Grid line weight */
    if (param_.grdlwt < 0.)
	size_.grdlnw = param_.grdlwt;
    else
	size_.grdlnw = param_.grdlwt * size_.avgaxl / 3;

/* Shade (hatch) line weight */
    if (param_.shdlwt < 0.)
	size_.shdlnw = param_.shdlwt;
    else
	size_.shdlnw = (float) (param_.shdlwt * 0.005);

/* set plot line pattern size */
    if (param_.linsiz < 0.)
	size_.pltlps = -param_.linsiz;
    else
	size_.pltlps = param_.linsiz * size_.avgaxl / 70;

/* set grid line pattern size */
    if (param_.grdsiz < 0.)
	size_.grdlps = -param_.grdsiz;
    else
	size_.grdlps = (float) (param_.grdsiz * 0.05);

/*
 * Set foreground/background colors
 */
    if (misc_.colors) {
	setbgci((int) param_.bgcol);
	if (param_.fgcol >= 0)
	    setfgci((int) param_.fgcol);
	if (param_.fncol >= 0)
	    setfnci((int) param_.fncol);
    }
}
