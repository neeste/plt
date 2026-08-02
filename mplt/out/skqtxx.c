/* skqtxx.c */

#include "output.h"
#include "device.h"
#include "copa.h"
#include "cview.h"
#include "font.h"


extern FONT *skfont;
extern float skchxx;
extern float skchyx;
extern float skchwd;
extern float skchsp;

FONT   *fntstd(void);
FONT   *fntalt(void);

static void open_font(void);
static double skqtxln(char *);

static float chrvalign[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static float chrhalign[4] = {0.0, 0.0, 0.5, 1.0};
static float sklnwd = 1.0;

/*
 * skqtxx - inquire text extent
 */
void
skqtxx(double x, double y, char *pstr, TXX *ptxx)
{
    float   ca, sa;
    float   x1, y1, x2, y2, xp, yp, xc, yc, xof, yof;

    xc = (float) skqtxln(pstr);
    yc = 0;
    x1 = 0;
    y1 = chrvalign[5] * skchyx;
    x2 = xc;
    y2 = chrvalign[1] * skchyx;
    xp = (chrhalign[copa_.curtxalh] - chrhalign[1]) * xc + x1;
    yp = (chrvalign[copa_.curtxalv] - chrvalign[4]) * skchyx;

    sa = -dev_.chuxnc;
    ca = dev_.chuync;
    xof = (float) (x - (xp * ca - yp * sa));
    yof = (float) (y - (yp * ca + xp * sa));
    (*ptxx).xtx0 = xof;
    (*ptxx).ytx0 = yof;
    (*ptxx).xcat = xc * ca - yc * sa + xof;
    (*ptxx).ycat = yc * ca + xc * sa + yof;
    (*ptxx).xtxx[0] = x1 * ca - y1 * sa + xof;
    (*ptxx).ytxx[0] = y1 * ca + x1 * sa + yof;
    (*ptxx).xtxx[1] = x2 * ca - y1 * sa + xof;
    (*ptxx).ytxx[1] = y1 * ca + x2 * sa + yof;
    (*ptxx).xtxx[2] = x2 * ca - y2 * sa + xof;
    (*ptxx).ytxx[2] = y2 * ca + x2 * sa + yof;
    (*ptxx).xtxx[3] = x1 * ca - y2 * sa + xof;
    (*ptxx).ytxx[3] = y2 * ca + x1 * sa + yof;
}

/*
 * skqtxln - inquire text length
 */
static double
skqtxln(char *pstr)
{
    int     c, prpspc;
    float   xc, xw;

    open_font();
    xc = 0.0;
    xw = skchwd + skchsp;
    prpspc = skfont->fpsp;	/* Enables proportional spacing */
    while ((c = *pstr++))
	if (32 <= c && c < 127)
	    xc += (prpspc) ? skfont->xadv[c] * skchxx + skchsp : xw;
    return ((xc < 0.0) ? 0.0 : xc);
}

void
skpnwd(double pw)
{
    open_font();
    DEV_PNWD((pw < 0) ? pw : pw * sklnwd);
}

static void
open_font(void)
{
    int     chrhgt, chrwid, chrlnw;

    skfont = (copa_.curtxft == ALTFONT) ? fntalt(): fntstd();

    chrhgt = skfont->fchh;	/* character height */
    chrwid = skfont->fchw;	/* character width */
    chrlnw = skfont->flnw;	/* normal line width */
    chrvalign[0] = skfont->fval[0];	/* normal */
    chrvalign[1] = skfont->fval[1];	/* top */
    chrvalign[2] = skfont->fval[2];	/* cap */
    chrvalign[3] = skfont->fval[3];	/* half */
    chrvalign[4] = skfont->fval[4];	/* base */
    chrvalign[5] = skfont->fval[5];	/* bottom */

    skchyx = dev_.chhnc / chrhgt;
    skchxx = skchyx * dev_.chxp;
    skchsp = dev_.chhnc * dev_.chsp;
    skchwd = chrwid * skchxx;
    sklnwd = (float) ((chrlnw > 0) ? (double) chrlnw / chrhgt : 1.0);
}
