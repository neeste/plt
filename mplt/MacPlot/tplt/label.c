/*
 *    PURPOSE:	Generates text characters with included symbols
 *
 *    USAGE:
 *
 *	label (x,y,str)
 *
 *    WHERE:
 *
 *	x,y = Starting coordinate of text generation
 *	str = String containing text to be plotted.
 *
 *    Special cases:
 *
 *	1. Text surrounded by angle brackets ([ and ]) is subscripted.
 *
 *	2. Text surrounded by fancy brackets ({ and }) is superscripted.
 *
 *	3. Symbols may be inserted into text strings by surrounding
 *	   the symbol number with vertical bars (|).  A symbol number
 *	   designated by an asterisk indicates that the current
 *	   symbol number should be used.  Symbol numbers inside strings
 *	   are decoded and plotted in place using the current symbol
 *	   size and are bottom-justified.
 *
 *	4. Dollar signs (and tabs) are converted to blanks.
 *
 *	5. Imbedded newlines may be indicated by "\n".
 *
 *************************************************************************
 */
#include <string.h>
#include "plt.h"
#include "pltblk.h"
#include "fort.h"

double  qtxln(char *);
double  inqchh(void);
int     labchr(char *, int *, int *);
void    label_line(double, double, char *);
void    labpos(double, double, char *, double *, double *);
void    shadearea(int, float *, float *, int);

static double fctr = 0.8;	/* relative sub/superscript height */
static double up = 0.7, down = 0.4;

void
label(double x, double y, char *str)
{
    char   *s, *t;
    double  vsp, hgt, chux, chuy, lspx = 0, lspy = 0;
    int     txalh, txalv;

    t = str;
    for (s = str; *s; s++)
	if (s[0] == '\\' && s[1] == '\\') {
	    s++;
	} else if (s[0] == '\\' && s[1] == 'n') {
	    if (t == str) {
		hgt = inqchh();
		qtxal(&txalh, &txalv);
		inqchup(&chux, &chuy);
		vsp = (txalv != 5) ? -param_.mvsp : param_.mvsp;
		lspx = chux * hgt * vsp;
		lspy = chuy * hgt * vsp;
	    }
	    s[0] = '\0';
	    label_line(x, y, t);
	    s[0] = '\\';
	    s += 2;
	    t = s;
	    x += lspx;
	    y += lspy;
	}
    label_line(x, y, t);
}

void
label_line(double xx, double yy, char *ptext)
{
    char    alfstr[128];
    double  dtmp = 0;
    double  x, y;
    double  hgt;
    double  facc;
    double  hk, vk;		/* size of key re char height */
    double  hy, hylw;	        /* width and line-weight of hyphen char */
    double  ancc, ancs;
    double  chux, chuy;
    float   xcentr, ycentr;
    float   xbox[4], ybox[4];
    int     ifont = STDFONT;
    int     i, ia, k, d, special;
    int     nc, pc, fc;
    int     txalh, txalv;
    static double xbeg, ybeg;

/*
 * --------------------------------------------------------------------------
 */
    qtxal(&txalh, &txalv);
    if (txalh == 0 && txalv == 0) {
	x = xx;
	y = yy;
    } else {
	labpos(xx, yy, ptext, &x, &y);
	settxal(1, 4);		/* text alignment = (left,base) */
    }
    xbeg = x;
    ybeg = y;
    nc = (int)strlen(ptext);
    settxft(ifont);
    hgt = inqchh();
    inqchup(&chux, &chuy);
    setmkup(chux, chuy);

/*
 *     Determine the factors for positioning nodes, and adjust
 *     the starting position of the character
 */
    facc = hgt;
    ancs = -chux;
    ancc = chuy;
    hk = (param_.mhkey < 0) ? -param_.mhkey : facc * param_.mhkey;
    vk = (param_.mvkey < 0) ? -param_.mvkey : facc * param_.mvkey;
    hy = facc * HYPHEN_WIDTH;
    hylw = size_.msglnw * param_.msgsiz * size_.avgaxl / 1.5;

/*
 *     Draw characters until we are done
 */
    ia = 0;
    for (i = 0; i < nc; i++) {
	k = labchr(ptext, &i, &special);

	if (!special)
	    alfstr[ia++] = k;
	if (special || (i + 1) == nc || (ia + 1) == 128) {	/* dump text */
	    alfstr[ia] = 0;
	    dtmp = qtxln(alfstr);
	    if (dtmp > 0.) {
		text(x, y, alfstr);
		x += ancc * dtmp;
		y += ancs * dtmp;
	    }
	    ia = 0;
	}
	if (special == 1) {	/* subscript or superscript */
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
	    x -= dtmp * ancs;
	    y += dtmp * ancc;
	    hk = (param_.mhkey < 0) ? -param_.mhkey : facc * param_.mhkey;
	    vk = (param_.mvkey < 0) ? -param_.mvkey : facc * param_.mvkey;
	} else if (special == 2) {	/* symbol in text */
	    /* We need to know the center of the symbol */
	    xcentr = (float) (x + (ancc * (vk + 0.25 * facc) - ancs * facc) * 0.5);
	    ycentr = (float) (y + (ancs * (vk + 0.25 * facc) + ancc * facc) * 0.5);
	    if (k <= 5 && param_.solid == 1)
		k += 10;
	    setmkfw(param_.solid != -1);	/* set marker fill white */
	    setmkt(k);
	    setmks(vk);
	    setplw(size_.pltlnw);
	    if (misc_.colors) {
		pc = param_.pltcol < 0 ? inqbci() : (int) param_.pltcol;
		fc = param_.filcol < 0 ? pc : (int) param_.filcol;
		setmkci(pc);
		setmkfi(fc);
	    }
	    marker(1, &xcentr, &ycentr);
	    x += ancc * (vk + 0.25 * facc);
	    y += ancs * (vk + 0.25 * facc);
	} else if (special == 3) {	/* shade in text */
	    xbox[0] = (float) (x + (ancc * (0.25 * facc) - ancs * (facc - vk)) * 0.5);
	    ybox[0] = (float) (y + (ancs * (0.25 * facc) + ancc * (facc - vk)) * 0.5);
	    xbox[1] = (float) (xbox[0] - ancs * vk);
	    ybox[1] = (float) (ybox[0] + ancc * vk);
	    xbox[2] = (float) (xbox[1] + ancc * hk);
	    ybox[2] = (float) (ybox[1] + ancs * hk);
	    xbox[3] = (float) (xbox[2] + ancs * vk);
	    ybox[3] = (float) (ybox[2] - ancc * vk);
	    setplw(size_.pltlnw);
	    if (misc_.colors) {
		setpci((int) param_.pltcol);
	    }
	    shadearea(4, xbox, ybox, k);
	    x += ancc * (hk + 0.25 * facc);
	    y += ancs * (hk + 0.25 * facc);
	} else if (special == 4) {	/* line in text */
	    xbox[0] = (float) (x + (ancc * 0.125 - ancs * 0.5) * facc);
	    ybox[0] = (float) (y + (ancs * 0.125 + ancc * 0.5) * facc);
	    xbox[1] = (float) (xbox[0] + ancc * hk);
	    ybox[1] = (float) (ybox[0] + ancs * hk);
	    setplw(size_.pltlnw);
	    if (misc_.colors) {
		setpci((int) param_.pltcol);
	    }
	    d = (int) param_.lindot;
	    setplt(k, d);		/* select line type */
	    line(2, xbox, ybox);
	    x += ancc * (hk + 0.25 * facc);
	    y += ancs * (hk + 0.25 * facc);
	} else if (special == 5) {	/* shift fonts */
	    ifont = (ifont == STDFONT) ? ALTFONT : STDFONT;
	    settxft(ifont);
	} else if (special == 6) {	/* alternate font character */
	    alfstr[0] = k;
	    alfstr[1] = 0;
	    dtmp = qtxln(alfstr);
	    if (dtmp > 0.) {
		settxft(ALTFONT);
		text(x, y, alfstr);
		x += ancc * dtmp;
		y += ancs * dtmp;
		settxft(ifont);
	    }
	} else if (special == 7) {	/* hyphen (short dash) */
	    xbox[0] = (float) (x + (ancc * 0.125 - ancs * 0.5) * facc);
	    ybox[0] = (float) (y + (ancs * 0.125 + ancc * 0.5) * facc);
	    xbox[1] = (float) (xbox[0] + ancc * hy);
	    ybox[1] = (float) (ybox[0] + ancs * hy);
	    setplw(hylw);               /* set line width */
	    setplt(0, 0);               /* select line type */
	    line(2, xbox, ybox);
	    x += ancc * (hy + 0.25 * facc);
	    y += ancs * (hy + 0.25 * facc);
	} else if (special == 8) {	/* half space */
	    x += ancc * (0.42 * facc);
	    y += ancs * (0.42 * facc);
	} else if (special == 9) {	/* back space */
	    x -= ancc * (0.84 * facc);
	    y -= ancs * (0.84 * facc);
	} else if (special == 10) {	/* half back space */
	    x -= ancc * (0.42 * facc);
	    y -= ancs * (0.42 * facc);
	} else if (special == 11) {	/* return */
	    x = xbeg;
	    y = ybeg;
	} else if (special == 12) {	/* set beg */
	    xbeg = x;
	    ybeg = y;
	}
    }
    setchh(hgt);
    settxal(txalh, txalv);
}
