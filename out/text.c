/* text.c */

#include "output.h"
#include "device.h"
#include "copa.h"
#include "cview.h"

void    skpnwd(double);
void    pencolor(int, int);
void    sktext(double, double, char *);
void    sktxxt(double, double, char *);
void    setdch(void);

/*
 text - put text at x,y (WC)
 */

void
text(x, y, pstr)
double  x, y;
char   *pstr;
{
    double  stdwid, reqwid;
    float   xt, yt;

    pencolor(copa_.curtxci, 0);
    otrans(x, y, &xt, &yt);
    setdch();			/* set device character variables */

    stdwid = 0.125 * copa_.curchh * cview_.vgn;
    reqwid = (copa_.curtxlw > 0.0) ? copa_.curtxlw * stdwid : copa_.curtxlw;

    switch (copa_.curtxpr) {
    case TXPRSTK:		/* Software characters */
	skpnwd(reqwid);
	sktext(xt, yt, pstr);
	break;
    case TXPREXT:		/* Outline text extent */
	skpnwd(reqwid);
	sktxxt(xt, yt, pstr);
	break;
    case TXPRSTR:		/* Hardware characters */
	DEV_PNWD(reqwid);
	DEV_TEXT(xt, yt, pstr);
	dev_.posknwn = 0;
	break;
    }
}

void
setdch(void)
{
    float   mg;

    dev_.chhnc = copa_.curchh * cview_.vgn;
    switch (copa_.rot) {
    case 0:
	dev_.chuxnc = copa_.curchux;
	dev_.chuync = copa_.curchuy;
	break;
    case 1:
	dev_.chuxnc = -copa_.curchuy;
	dev_.chuync = copa_.curchux;
	break;
    case 2:
	dev_.chuxnc = -copa_.curchux;
	dev_.chuync = -copa_.curchuy;
	break;
    case 3:
	dev_.chuxnc = copa_.curchuy;
	dev_.chuync = -copa_.curchux;
	break;
    }
    mg = dev_.chuxnc * dev_.chuxnc + dev_.chuync * dev_.chuync;
    if (mg > 0.0 && mg != 1.0) {/* Normalize vector */
	dev_.chuxnc /= mg;
	dev_.chuync /= mg;
    }
    dev_.chsxnc = copa_.curchsx;
    dev_.chsync = copa_.curchsy;
    dev_.chxp = copa_.curchxp;
    dev_.chsp = copa_.curchsp;
}
