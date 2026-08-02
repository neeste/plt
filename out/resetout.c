/*
 * resetout - reset output
 */

#include "output.h"
#include "device.h"
#include "clip.h"
#include "copa.h"
#include "cview.h"
#include "cwind.h"
#include "font.h"

void    comp_trns(void);
void    reset_colors(void);
void    reset_fonts(void);

static void defopa(void);

void
resetout(void)
{
    defopa();
    reset_fonts();
    reset_colors();
    comp_trns();
    dev_.posknwn = 0;
}

/*
 * updout - force output
 */
void
updout(void)
{
    fflush(fpout);
}

/*
 * defopa - reset default output primitive atributes
 */
static void
defopa(void)
{
    copa_.curfgci = dev_.bci;	    /* foreground color index */
    copa_.curfais = 0;		    /* fill area interior style */
    copa_.curfasi = 0;		    /* fill area style index */
    copa_.curfaci = -1;		    /* fill area color index */
    copa_.curfalw = (float) 0.005;  /* fill area line width */
    copa_.curxrf = 0.;		    /* pattern reference point */
    copa_.curyrf = 0.;		    /* pattern reference point */
    copa_.curplt = 0;		    /* plot line type */
    copa_.curplw = 1.;		    /* plot line width */
    copa_.curpci = -1;		    /* plot color index */
    copa_.curpps = (float) 0.05;    /* plot pattern size (inches) */
    copa_.curchh = (float) 0.1;	    /* character height (inches) */
    copa_.curtxlw = 1;		    /* text line weight */
    copa_.curtxpr = dev_.txpr;	    /* text precision */
    copa_.curtxft = STDFONT;	    /* text font */
    copa_.curtxci = -1;		    /* text color index */
    copa_.curtxalh = 0;		    /* text horizontal alignment */
    copa_.curtxalv = 0;		    /* text vertical alignment */
    copa_.curchux = 0;		    /* character up vector x */
    copa_.curchuy = 1.;		    /* character up vector y */
    copa_.curchsx = 1.;		    /* character slant vector x */
    copa_.curchsy = 0.;		    /* character slant vector y */
    copa_.curchxp = 1.;		    /* character expansion factor */
    copa_.curchsp = 0.;		    /* character spacing */
    copa_.curmkt = 1;		    /* marker type */
    copa_.curmkci = -1;		    /* marker color index */
    copa_.curmkfi = -1;		    /* marker fill index */
    copa_.curmkfw = 1;		    /* marker fill white */
    copa_.curmks = 1.;		    /* marker size */
    copa_.curmkux = 0.;		    /* marker up vector x */
    copa_.curmkuy = 1.;		    /* marker up vector y */

    cview_.vx1 = 0;		    /* view port xllc */
    cview_.vy1 = 0;		    /* view port yllc */
    cview_.vgn = 1;		    /* view port gain */
    cview_.vro = 0;		    /* view port rotation */

    cwind_.wx1 = 0;		    /* input window xllc */
    cwind_.wy1 = 0;		    /* input window yllc */
    cwind_.wx2 = XPAGE;		    /* input window xurc */
    cwind_.wy2 = YPAGE;		    /* input window yurc */

    cclip_.cx1 = 0;		    /* clipping window xllc */
    cclip_.cy1 = 0;		    /* clipping window yllc */
    cclip_.cx2 = XPAGE;		    /* clipping window xurc */
    cclip_.cy2 = YPAGE;		    /* clipping window yurc */
}

void
reset_fonts(void)
{
    int     ft;

    for (ft = 0; ft < 3; ft++) {
	if (curfont[ft] != deffont[ft]) {
	    free(curfont[ft]->xlft);
	    free(curfont[ft]->xadv);
	    free(curfont[ft]->khar);
	    free(curfont[ft]->node);
	    free(curfont[ft]);
	}
	curfont[ft] = deffont[ft];
    }
}

void
reset_colors(void)
{
    int     nci;
    extern RGB_COLOR ct16[16];

    nci = 16;
    if (ct != ct16)
	free(ct);
    ct = ct16;

    dev_.bci = 0;		    /* black color index */
    dev_.wci = nci - 1;		    /* white color index */
    dev_.nci = nci;		    /* color table size */

    DEV_SETPAL();

    dev_.fgci = dev_.bci;	    /* foreground color is black */
    dev_.bgci = dev_.wci;	    /* background color is white */
    dev_.fnci = dev_.bgci;	    /* no fountain */
}
