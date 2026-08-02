/*
 *  labpos.c
 */
#include <string.h>
#include "plt.h"
#include "pltblk.h"
#include "fort.h"

double  inqchh(void);
double  lab_align(char *, double *, double *);
double  lablen(char *);
double  qtxln(char *);
int     labchr(char *, int *, int *);

static double chrhalign[4] = {0.0, 0.0, 0.5, 1.0};
static double chrvalign[6] = {0.0, 1.141, 1.0, 0.5, 0.0, -0.328};
static double fctr = 0.8;	/* relative sub/superscript height */
static double up = 0.7, down = 0.4;

/*
 * labpos - find proper starting position for label
 */
void
labpos(double x, double y, char *pstr, double *x0, double *y0)
{
    double  chux, chuy;
    double  xp, yp;

    (void) lab_align(pstr, &xp, &yp);
    inqchup(&chux, &chuy);
    *x0 = x - (xp * chuy + yp * chux);
    *y0 = y - (yp * chuy - xp * chux);
}

double
lablen(char *ptext)
{
    return (lab_align(ptext, NULL, NULL));
}

double
lab_align(char *ptext, double *xp, double *yp)
{
    char    alfstr[2];
    double  x, y;
    double  hgt;
    double  facc, hk, vk, hy;
    double xbeg, ybeg;
    double bot, cap, top, lbot, lcap ,ltop;
    int     ifont = STDFONT;
    int     i, k, special;
    int     nc;
    int     txalh, txalv;
    register double dtmp = 0;	/* strictly used to save multiplications */

    qtxal(&txalh, &txalv);
    settxal(0, 0);
    xbeg = x = 0;
    ybeg = y = 0;
    lbot = lcap = ltop = 0;
    nc = (int)strlen(ptext);
    settxft(ifont);
    hgt = inqchh();
    facc = hgt;
    hk = (param_.mhkey < 0) ? -param_.mhkey : facc * param_.mhkey;
    vk = (param_.mvkey < 0) ? -param_.mvkey : facc * param_.mvkey;
    hy = facc * HYPHEN_WIDTH;
    alfstr[1] = '\0';
    for (i = 0; i < nc; i++) {
	k = labchr(ptext, &i, &special);
        bot = cap = top = 0;
	if (special == 0 || special == 6) {
	    alfstr[0] = k;
	    dtmp = qtxln(alfstr);
	    if (dtmp > 0.) {
		x += dtmp;
	    }
	    top = y + chrvalign[1] * facc;
	    cap = y + chrvalign[2] * facc;
	    bot = y + chrvalign[5] * facc;
	} else if (special == 1) {	/* superscript or subscript */
	    switch (k) {
	    case '{':		/* start superscript */
		dtmp = up * facc;
		facc *= fctr;
		break;
	    case ']':		/* end subscript */
		facc /= fctr;
		dtmp = down * facc;
		break;
	    case '}':		/* end superscript '}' */
		facc /= fctr;
		dtmp = -up * facc;
		break;
	    case '[':		/* start subscript '[' */
		dtmp = -down * facc;
		facc *= fctr;
		break;
	    }
	    setchh(facc);
	    y += dtmp;
	    hk = (param_.mhkey < 0) ? -param_.mhkey : facc * param_.mhkey;
	    vk = (param_.mvkey < 0) ? -param_.mvkey : facc * param_.mvkey;
	} else if (special == 2) {	/* symbol in text */
	    x += (vk + 0.25 * facc);
	    top = y + (facc * 1.25 + vk) / 2;
	    cap = y + (facc + vk) /2;
	    bot = y + (facc * 0.75 - vk) / 2;
	} else if (special == 3) {	/* shade in text */
	    x += (hk + 0.25 * facc);
	    top = y + (facc * 1.25 + vk) / 2;
	    cap = y + (facc + vk) /2;
	    bot = y + (facc * 0.75 - vk) / 2;
	} else if (special == 4) {	/* line in text */
	    x += (hk + 0.25 * facc);
	} else if (special == 5) {	/* shift fonts */
	    ifont = (ifont == STDFONT) ? ALTFONT : STDFONT;
	    settxft(ifont);
	} else if (special == 7) {	/* hyphen (short dash) */
	    x += (hy + 0.25 * facc);
	} else if (special == 8) {	/* half space */
	    x += (0.42 * facc);
	} else if (special == 9) {	/* back space */
	    x -= (0.84 * facc);
	} else if (special == 10) {	/* half back space */
	    x -= (0.42 * facc);
	} else if (special == 11) {	/* return */
	    x = xbeg;
	    y = ybeg;
	} else if (special == 12) {	/* set beg */
	    xbeg = x;
	    ybeg = y;
	}
	if (ltop < top)
	    ltop = top;
	if (lcap < cap)
	    lcap = cap;
	if (lbot > bot)
	    lbot = bot;
    }
    setchh(hgt);
    settxal(txalh, txalv);
     if (xp && yp) {
	*xp = chrhalign[txalh] * x;
	*yp = chrvalign[txalv] * hgt;
        if (txalv == 0 || txalv == 4) {
	    *yp = 0;
        } else if (txalv == 1) {
	    *yp = ltop;
	} else if (txalv == 2) {
	    *yp = lcap;
	} else if (txalv == 3) {
	    *yp = lcap / 2;
	} else if (txalv == 5) {
	    *yp = lbot;
	}
    }
    return (x);
}
