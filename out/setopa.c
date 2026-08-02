/*
 * setopa - set output primitive attributes
 */

#include <string.h>
#include "output.h"
#include "device.h"
#include "fort.h"
#include "clip.h"
#include "copa.h"
#include "cview.h"
#include "cwind.h"

void    comp_trns(void);

/*
 * settxpr - set text precision
 */
void
settxpr(int newtxpr)
{
    if (newtxpr == 0)
	copa_.curtxpr = dev_.txpr;
    else
	copa_.curtxpr = newtxpr;
}

/*
 * settxft - set text font
 */
void
settxft(int newtxft)
{
    copa_.curtxft = newtxft;
}

/*
 * settxci - set text color index
 */
void
settxci(int newtxci)
{
    copa_.curtxci = newtxci;
}

/*
 * setvup - set view port
 */
void
setvup(double x1, double y1, double gn, double ro)
{
    cview_.vx1 = (float) x1;
    cview_.vy1 = (float) y1;
    cview_.vgn = (float) gn;
    cview_.vro = (float) ro;
    comp_trns();
}

/*
 * setwin - set input window
 */
void
setwin(double x1, double y1, double x2, double y2)
{
    cwind_.wx1 = (float) x1;
    cwind_.wy1 = (float) y1;
    cwind_.wx2 = (float) x2;
    cwind_.wy2 = (float) y2;
    comp_trns();
}

/*
 * setclp - set clipping window
 */
void
setclp(double x1, double y1, double x2, double y2)
{
    cclip_.cx1 = (float) x1;
    cclip_.cy1 = (float) y1;
    cclip_.cx2 = (float) x2;
    cclip_.cy2 = (float) y2;
    comp_trns();
}

/*
 * setfais - set fill area interior style
 */
void
setfais(int newfais)
{
    copa_.curfais = newfais;
}

/*
 * setfasi - set fill area style index
 */
void
setfasi(int newfasi)
{
    copa_.curfasi = newfasi;
}

/*
 * setfaci - set fill area color index
 */
void
setfaci(int newfaci)
{
    copa_.curfaci = newfaci;
}

/*
 * setfalw - set fill area line width
 */
void
setfalw(double newfalw)
{
    copa_.curfalw = (float) newfalw;
}

/*
 * setplt - set plot line type
 */
void
setplt(int newplt, int newdot)
{
    copa_.curplt = newplt;
    copa_.curdot = newdot;
}

/*
 * setplw - set plot line width
 */
void
setplw(double newplw)
{
    copa_.curplw = (float) newplw;
}

/*
 * setpci - set plot color index
 */
void
setpci(int newpci)
{
    copa_.curpci = newpci;
}

/*
 * setpps - set plot pattern size
 */
void
setpps(double newpps)
{
    copa_.curpps = (float) newpps;
}

/*
 * setchh - set character height
 */
void
setchh(double newchh)
{
    copa_.curchh = (float) newchh;
}

/*
 * settxal - set text alignment
 */
void
settxal(int newtxalh, int newtxalv)
{
    copa_.curtxalh = newtxalh;
    copa_.curtxalv = newtxalv;
}

/*
 * setmkup - set marker up vector
 */
void
setmkup(double newmkux, double newmkuy)
{
    copa_.curmkux = (float) newmkux;
    copa_.curmkuy = (float) newmkuy;
}

/*
 * setmkci - set marker color index
 */
void
setmkci(int newmkci)
{
    copa_.curmkci = newmkci;
}

/*
 * setmkfi - set marker fill (color) index
 */
void
setmkfi(int newmkfi)
{
    copa_.curmkfi = newmkfi;
}

/*
 * setmkfw - set marker fill white
 */
void
setmkfw(int newmkfw)
{
    copa_.curmkfw = newmkfw;
}

/*
 * setchup - set character up vector
 */
void
setchup(double newchux, double newchuy)
{
    copa_.curchux = (float) newchux;
    copa_.curchuy = (float) newchuy;
}

/*
 * setchan - set character slant angle (degrees)
 */
void
setchan(double angle)
{
    double  cosdeg(double), sindeg(double);

    copa_.curchsx = (float) cosdeg(angle);
    copa_.curchsy = (float) sindeg(angle);
}

/*
 * setchxp - set character expansion factor
 */
void
setchxp(double chxp)
{
    copa_.curchxp = (float) chxp;
}

/*
 * setchsp - set character spacing
 */
void
setchsp(double chsp)
{
    copa_.curchsp = (float) chsp;
}

/*
 * settxlw - set text line weight
 */
void
settxlw(double lw)
{
    copa_.curtxlw = (float) lw;
}

/*
 * setmkt - set marker type
 */
void
setmkt(int type)
{
    copa_.curmkt = type;
}

/*
 * setmks - set marker size
 */
void
setmks(double size)
{
    copa_.curmks = (float) size;
}

/*
 * setnpn - set number of pens
 */
void
setnpn(int npn)
{
    dev_.maxpen = npn;
}

/*
 * setpgp - set page prompt
 */
void
setpgp(int pgp)
{
    dev_.prompt = pgp;
}

/*
 * setpgb - set page border
 */
void
setpgb(int pgb)
{
    dev_.border = pgb;
}

/*
 * setfgci - set all foreground color indices
 */
void
setfgci(int fgc)
{
    if (fgc >= 0 && fgc < dev_.nci)
	copa_.curfgci = fgc;
}

/*
 * setbgci - set background color index
 */
void
setbgci(int bgc)
{
    if (bgc == -1) {
	dev_.bgci = dev_.wci;
    } else if (bgc < dev_.nci) {
	dev_.bgci = bgc;
    }
    dev_.fnci = dev_.bgci;	/* no fountain */
}

/*
 * setfnci - set "fountain" color index
 */
void
setfnci(int fnc)
{
    if (fnc >= 0 && fnc < dev_.nci)
	dev_.fnci = fnc;
}

/*
 * setwidestroke - set wide strokes
 */
void
setwidestroke(int ws)
{
    dev_.widestroke = ws;
}

/*
 * setpalette - set color palette
 */
void
setpalette(int n, float *pr, float *pg, float *pb)
{
    int     i, nci;
    extern RGB_COLOR ct16[16];

    nci = n;
    if (ct != ct16)
	free(ct);
    ct = (RGB_COLOR *) malloc(sizeof(RGB_COLOR) * nci);

    dev_.bci = 0;		/* black color index */
    dev_.wci = nci - 1;		/* white color index */
    dev_.nci = nci;		/* color table size */

    for (i = 0; i < nci; i++) {
	ct[i].r = (short int) (limit(0, pr[i], 1) * ct_max + 0.5);
	ct[i].g = (short int) (limit(0, pg[i], 1) * ct_max + 0.5);
	ct[i].b = (short int) (limit(0, pb[i], 1) * ct_max + 0.5);
    }

    DEV_SETPAL();		/* set device's color palette */
}

void
set_title(char *s)
{
    char *p;
    int n;
    static int maxlen = 64;

    n = (int)strlen(s);
    if (n > maxlen) { 		/* strip path */
        p = s + n - 1;
	while (p >= s && *p != '/' && *p != '\\' && *p != ':')
	    p--;
	s = p + 1;
        n = (int)strlen(s);
	if (n > maxlen) {
	    s += n - maxlen;
	}
    }
    copa_.title = s;
}

void
set_creator(char *s)
{
    copa_.creator = s;
}

void
set_date(char *s)
{
    copa_.date = s;
}
