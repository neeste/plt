/*
 * sktext
 */
#include "output.h"
#include "device.h"
#include "clip.h"
#include "copa.h"
#include "cview.h"
#include "font.h"

#define XMN cclip_.tcx1
#define YMN cclip_.tcy1
#define XMX cclip_.tcx2
#define YMX cclip_.tcy2
#define inwin(x,y)	(XMN <= x && x <= XMX && YMN <= y && y <= YMX)

void    dranodes(short *, short *, int, TRANSF *, int);
void    skqtxx(double, double, char *, TXX *);
void    tpcl(double, double, double, double);

static void drachar(double, double, int, int);

FONT   *skfont = NULL;
float   skchxx = 0;
float   skchyx = 0;
float   skchwd = 0;
float   skchsp = 0;

void
sktext(double xpos, double ypos, char *pstr)
{
    int     k, prpspc, clipyet;
    float   xb = 0, yb = 0;
    float   xt, yt;
    float   crot, srot, xsp, ysp, xbb = 0, ybb = 0;
    TXX     txx;
    float   chr_xleft, chr_width = 0, chr_ybase;

    skqtxx(xpos, ypos, pstr, &txx);
    clipyet = inwin(txx.xtxx[0], txx.ytxx[0])
	&& inwin(txx.xtxx[1], txx.ytxx[1])
	&& inwin(txx.xtxx[2], txx.ytxx[2])
	&& inwin(txx.xtxx[3], txx.ytxx[3]);
    xt = txx.xtx0;
    yt = txx.ytx0;
    srot = -dev_.chuxnc * skchxx;
    crot = dev_.chuync * skchyx;
    xsp = dev_.chuync * skchsp;
    ysp = -dev_.chuxnc * skchsp;
    prpspc = skfont->fpsp;	/* Enables proportional spacing */
    if (!prpspc) {
	chr_width = skchwd;
	xb = (float) (0.5 * xsp);
	yb = (float) (0.5 * ysp);
    } else {
	chr_ybase = skfont->fval[4];	/* Vertical offset to base of char */
	xbb = (float) (0.5 * xsp + srot * chr_ybase);
	ybb = (float) (0.5 * ysp - crot * chr_ybase);
    }
    while (*pstr) {
	k = *pstr++;
	if (32 <= k && k < 127) {	/* printable ascii characters */
	    if (!prpspc) {
		drachar(xt + xb, yt + yb, k, clipyet);
	    } else {
		chr_xleft = skfont->xlft[k];
		chr_width = skfont->xadv[k] * dev_.chxp;
		xb = crot * chr_xleft + xbb;
		yb = srot * chr_xleft + ybb;
		drachar(xt + xb, yt + yb, k, clipyet);
	    }
	    xt += crot * chr_width + xsp;
	    yt += srot * chr_width + ysp;
	}
    }
}

void
sktxxt(double xpos, double ypos, char *pstr)
{
    TXX     txx;

    skqtxx(xpos, ypos, pstr, &txx);
    tpcl(txx.xtxx[0], txx.ytxx[0], txx.xtxx[1], txx.ytxx[1]);
    tpcl(txx.xtxx[1], txx.ytxx[1], txx.xtxx[2], txx.ytxx[2]);
    tpcl(txx.xtxx[2], txx.ytxx[2], txx.xtxx[3], txx.ytxx[3]);
    tpcl(txx.xtxx[3], txx.ytxx[3], txx.xtxx[0], txx.ytxx[0]);
}

/*
 * drachar - draw a character using table of symbol node definitions
 */
static void
drachar(double xllt, double yllt, int k, int clipyet)
{
    float   tsl;
    int     nsides, ndx0, ndy0;
    short  *fnode, *lnode;
    TRANSF  ndt;

    /* check range of character code */
    if (k < 0 || k >= skfont->nkhar) {
	return;
    }
    /* find first and last nodes in table */
    fnode = skfont->node + skfont->khar[k] - 1;
    lnode = skfont->node + skfont->khar[k + 1] - 2;
    ndx0 = skfont->fnx0;	/* x origin for node values */
    ndy0 = skfont->fny0;	/* y origin for node values */
    nsides = limit(8, (int) (0.5 * dev_.chhnc * dev_.adupi), 64);
    tsl = (dev_.chsxnc == 0) ? 0 : dev_.chsync / dev_.chsxnc;
    ndt.txx = dev_.chuync * skchxx;
    ndt.txy = dev_.chuxnc * skchyx + tsl * dev_.chuync * skchxx;
    ndt.tyx = -dev_.chuxnc * skchxx;
    ndt.tyy = dev_.chuync * skchyx - tsl * dev_.chuxnc * skchxx;
    ndt.txo = (float) (xllt - (ndt.txx + ndt.txy) * ndx0);
    ndt.tyo = (float) (yllt - (ndt.tyx + ndt.tyy) * ndy0);
    dranodes(fnode, lnode, nsides, &ndt, clipyet);
}
