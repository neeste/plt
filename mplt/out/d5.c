/*
 * d5 - Postscript graphics device
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "copa.h"
#include "clip.h"
#include "output.h"
#include "device.h"

static void beg_page(void);
static void background(void);
static void end_page(void);
static void position(double, double, double);
static void prologue(void);
static void putps(char *);
static void setclip(void);
static void setrgb(int);
static void stroke_path(void);
static void trailer(void);
static void sclrot(double, double, double *, double *);
static void putpnt(double, double);
static void setpen(short);

static double xpos = 0;
static double ypos = 0;
static double adupi = 720.0;
static double psu = 0.1;	/* = 72 / adupi */
static double bbx1 = 32000;
static double bby1 = 32000;
static double bbx2 = 0;
static double bby2 = 0;
static double mg = 18;
static double xl = 756;
static short pen = 0;
static short have_bb = 0;
static short lastpen = 0;
static short lastcolor = 0;
static unsigned short npd = 0;
static unsigned short maxnpd = 1024;
static unsigned short pagecount = 0;
static unsigned short newpage = 0;
static double cx1= 0;
static double cy1 = 0;
static double cx2= 0;
static double cy2 = 0;

/* device 5 - postscript device */
int     d5open(void);
void    d5close(void), d5page(void);
void    d5trns(double, double, int);
void    d5pnwd(double);
void    d5fill(int, float *, float *, int, int);
void    d5curv(float *, float *);

/*
 * d5bind - override default device function
 */
void
d5bind()
{
    d_open = d5open;
    d_close = d5close;
    d_trns = d5trns;
    d_page = d5page;
    d_pnwd = d5pnwd;
    d_fill = d5fill;
    d_curv = d5curv;
}

/*
 * d5open - initialize plotting device
 */
int
d5open()
{
    dev_.adupi = (float) adupi;	/* addressable units per inch */
    dev_.txpr = TXPRSTK;	/* text precision = stroke */
    dev_.maxpen = 1000;		/* maximum pen value */
    dev_.prompt = 0;		/* prompt user at end of page */
    dev_.border = 0;		/* draw page border */
    dev_.fillwhite = 1;		/* fill white areas */

    xl = XPAGE * adupi * psu;	/* XPAGE converted to "points" */
    pagecount = 0;
    newpage = 1;
    have_bb = 0;
    prologue();			/* write postscript header */
    return (1);			/* 0 = unsuccessful open, 1 = successful open */
}

/*
 * d5close - close plotting device
 */
void
d5close()
{
    end_page();
    trailer();
}

/*
 * d5trns - convert NC to device units and move or draw
 *
 *	xx,yy	coordinates in inches (NC) already clipped to page boundary:
 *		0 <= xx <= XPAGE  and  0 <= yy <= YPAGE
 *	do_gs	move/draw flag: 0 = move, 1 = draw
 */
void
d5trns(xx, yy, do_gs)
double  xx, yy;
int     do_gs;
{
    double  x, y;

    if (newpage)
	beg_page();
    x = xx * adupi;
    y = yy * adupi;
    if (do_gs == 0) {
	if (x == xpos && y == ypos) {
	    return;
	} else {
	    stroke_path();
	    npd = 0;
	}
    } else {
	setrgb(dev_.fgci);
	setpen(pen);
	if (npd >= maxnpd) {
	    stroke_path();
	    npd = 0;
	}
	if (npd == 0) {
	    putpnt(xpos, ypos);
	    putps(" M");
	}
	putpnt(x, y);
	putps(" D");
	npd++;
    }
    xpos = x;
    ypos = y;
}

/*
 * d5curv - convert NC to device units and add curve to path
 *
 *	xx,yy	coordinates of Bezier curve points
 *              in inches (NC) already clipped to page boundary:
 */
void
d5curv(float *xx, float *yy)
{
    double  x = 0, y = 0;
    int     i;

    if (newpage)
	beg_page();
    setrgb(dev_.fgci);
    setpen(pen);
    if (npd >= maxnpd - 4) {
	stroke_path();
	npd = 0;
    }
    if (npd == 0) {
        putpnt(xpos, ypos);
	putps(" M");
    }
    for (i = 0; i < 3; i++) {
	x = xx[i] * adupi;
	y = yy[i] * adupi;
	putpnt(x, y);
	npd++;
    }
    putps(" B");
    xpos = x;
    ypos = y;
}

/*
 * d5page - signal new page (media change)
 */
void
d5page()
{
    end_page();
}

/*
 * d5pnwd -
 */
void
d5pnwd(pw)
double  pw;			/* pen width in inches */
{
    stroke_path();
    if (pw < 0)
	pw *= (-0.005);	/* fix-up negative values */
    pen = (unsigned short) (pw * adupi);
    pen = limit(1, pen, dev_.maxpen);	/* force pen value into range */
}

/*
 * d5fill - fill area with solid white or black (WC)
 *	  (do this in NC to avoid duplicating arrays)
 */
void
d5fill(n, px, py, fg, nc)
int     n, fg, nc;
float  *px, *py;
{
    int     i, c;
    float   xt, yt;
    double  ix, iy;

    if (newpage)
	beg_page();
    else
	stroke_path();
    c = (fg) ? dev_.fgci : dev_.bgci;
    if (c < 0)
	return;
    setrgb(c);
    putps(" F1");
    setclip();
    for (i = 0; i < n; i++) {
	if (nc) {
	    xt = px[i];
	    yt = py[i];
	} else {
	    otrans(px[i], py[i], &xt, &yt);
	}
	ix = xt * adupi;
	iy = yt * adupi;
	putpnt(ix, iy);
	putps((i == 0) ? " M" : " D");
    }
    putps(" F2");
}

static void
beg_page()
{
    char    str[80];

    cx1 = cy1 = cx2 = cy2 = 0;
    lastpen = -1;
    lastcolor = -1;
    pagecount++;
    sprintf(str, "%%%%Page: %u %u\n", pagecount, pagecount);
    putps(str);
    background();
    putps(" N");		/* new path */
    newpage = 0;
}

static void
end_page()
{
    stroke_path();		/* flush any remaining path */
    putps(" P");
    newpage = 1;
}

static void
putcm(c, s)
char   *c, *s;
{
    if (s && *s) {
	putps(c);
	putps(s);
	putps("\n");
    }
}

static void
prologue()
{
    putps("%!PS-Adobe-2.0 EPSF-2.0\n");
    putcm("%%Title: ", copa_.title);
    putcm("%%Creator: ", copa_.creator);
    putcm("%%CreationDate: ", copa_.date);
    putps("%%BoundingBox: (atend)\n");	/* 10.5 x 8 inches */
    putps("%%Pages: (atend)\n");
    putps("%%EndComments\n");
    putps("/D /lineto load def\n");	/* Draw command definition */
    putps("/B /curveto load def\n");	/* Curve command definition */
    putps("/C {clip newpath} bind def\n");
    putps("/F1 {gsave newpath} bind def\n");
    putps("/F2 {closepath eofill flush grestore} bind def\n");
    putps("/G /setgray load def\n");
    putps("/M /moveto load def\n");	/* Move command definition */
    putps("/N /newpath load def\n");
    putps("/P /showpage load def\n");
    putps("/RGB /setrgbcolor load def\n");
    putps("/S {1 setlinejoin 1 setlinecap stroke} def\n");
    putps("/W /setlinewidth load def\n");	/* line width definition */
    putps("/RF {RGB /y2 exch def /x2 exch def /y1 exch def /x1 exch def\n");
    putps("    F1 x1 y1 M x1 y2 D x2 y2 D x2 y1 D F2\n");
    putps("/I /lineto load def\n");	/* Draw command definition */
    putps("} def\n");
    putps("%%EndPrologue\n");
}

static void
trailer()
{
    char    str[80];
    double  xp;
    int     x1, y1, x2, y2;

    xp = mg + xl + mg;
    x1 = (int) limit(0, floor(bbx1) - 1, xp);
    y1 = (int) limit(0, floor(bby1) - 1, xp);
    x2 = (int) limit(0, ceil(bbx2) + 1, xp);
    y2 = (int) limit(0, ceil(bby2) + 1, xp);
    putps("%%Trailer\n");
    sprintf(str, "%%%%BoundingBox: %d %d %d %d\n", x1, y1, x2, y2);
    putps(str);
    sprintf(str, "%%%%Pages: %u\n", pagecount);
    putps(str);
    putps("%%EOF\n");
}

static void
position(double x, double y, double p)
{
    double x1 = x - p;
    double y1 = y - p;
    double x2 = x + p;
    double y2 = y + p;

    if (!have_bb) {
	bbx1 = x1;
	bby1 = y1;
	bbx2 = x2;
	bby2 = y2;
	have_bb = 1;
    } else {
        if (bbx1 > x1)
	    bbx1 = x1;
	if (bby1 > y1)
	    bby1 = y1;
	if (bbx2 < x2)
	    bbx2 = x2;
	if (bby2 < y2)
	    bby2 = y2;
    }
}

static void
stroke_path()
{
    if (npd != 0)
	putps(" S");
    npd = 0;
    dev_.posknwn = 0;
}

static void
background()
{
    char    str[80];
    double  x1, y1, x2, y2;
    float   r1, g1, b1, r2, g2, b2;
    int     c;

    sclrot(0.0, 0.0, &x1, &y1);
    sclrot(x_page * adupi, y_page * adupi, &x2, &y2);
    if (dev_.fnci <0 || dev_.bgci < 0)
        return;
    if (dev_.fnci == dev_.bgci) {
	c = dev_.bgci;
	r1 = ct_scale(ct[c].r);
	g1 = ct_scale(ct[c].g);
	b1 = ct_scale(ct[c].b);
	sprintf(str, " %.1f %.1f %.1f %.1f %.3f %.3f %.3f RF",
	    x1, y1, x2, y2, r1, g1, b1);
	putps(str);
	lastcolor = dev_.bgci;
    } else {
	c = dev_.fnci;
	r1 = ct_scale(ct[c].r);
	g1 = ct_scale(ct[c].g);
	b1 = ct_scale(ct[c].b);
	c = dev_.bgci;
	r2 = ct_scale(ct[c].r);
	g2 = ct_scale(ct[c].g);
	b2 = ct_scale(ct[c].b);
	sprintf(str, " /x1 %.1f def /y1 %.1f def", x1, y1);
	putps(str);
	sprintf(str, " /x2 %.1f def /y2 %.1f def", x2, y2);
	putps(str);
	sprintf(str, " /r1 %.3f def /g1 %.3f def /b1 %.3f def", r1, g1, b1);
	putps(str);
	sprintf(str, " /r2 %.3f def /g2 %.3f def /b2 %.3f def", r2, g2, b2);
	putps(str);
	putps(" /ns 250 def");
	putps(" /dx x2 x1 sub ns div def");
	putps(" /dr r2 r1 sub ns div def");
	putps(" /dg g2 g1 sub ns div def");
	putps(" /db b2 b1 sub ns div def");
	putps(" 1 1 ns { pop");
	putps(" x2 dx sub y1 x2 y2 r1 g1 b1 RF");
	putps(" /x2 x2 dx sub def");
	putps(" /r1 r1 dr add def");
	putps(" /g1 g1 dg add def");
	putps(" /b1 b1 db add def");
	putps(" } for\n");
	lastcolor = dev_.bgci;
    }
}

static void
setclip()
{
    char    str[80];
    double  tx, ty;
    double  ix1, iy1, ix2, iy2;

    ix1 = cclip_.tcx1 * adupi;
    iy1 = cclip_.tcy1 * adupi;
    ix2 = cclip_.tcx2 * adupi;
    iy2 = cclip_.tcy2 * adupi;
    if (cx1 != ix1 || cy1 != iy1 
	|| cx2 != ix2 || cy2 != iy2) {
	sclrot(ix1, iy1, &tx, &ty);
	sprintf(str, " %.1f %.1f M", tx, ty);
	putps(str);
	sclrot(ix2, iy1, &tx, &ty);
	sprintf(str, " %.1f %.1f D", tx, ty);
	putps(str);
	sclrot(ix2, iy2, &tx, &ty);
	sprintf(str, " %.1f %.1f D", tx, ty);
	putps(str);
	sclrot(ix1, iy2, &tx, &ty);
	sprintf(str, " %.1f %.1f D", tx, ty);
	putps(str);
	putps(" C");
	cx1 = ix1;
	cy1 = iy1;
	cx2 = ix2;
	cy2 = iy2;
    }
}

static void
setrgb(int c)
{
    char    str[80];
    double  red, grn, blu;

    if (c != lastcolor) {	/* set rgb color */
	if (dev_.gray) {
	    sprintf(str, " %.3f G", ct_scale(c));
	    lastcolor = -c;
	} else {
	    red = ct_scale(ct[c].r);
	    grn = ct_scale(ct[c].g);
	    blu = ct_scale(ct[c].b);
	    if (red == grn && grn == blu && blu == red)
		sprintf(str, " %.3f G", red);	/* gray */
	    else
		sprintf(str, " %.3f %.3f %.3f RGB", red, grn, blu);
	    lastcolor = c;
	}
	putps(str);
    }
}

static void
putps(s)
char   *s;
{
    static int nput = 0;
    int     nc;

    nc = (int)strlen(s);
    if ((*s == '%' && nput > 0) || nput + nc > 75 || nc == 0) {
	fputs("\n", fpout);
	nput = 0;
    }
    fputs(s, fpout);
    if (s[nc - 1] == '\n')
	nput = 0;
    else
	nput += nc;
}

static void
sclrot(ix, iy, tx, ty)
double ix, iy;
double *tx, *ty;
{
    if (o_page) {
        *tx = mg + ix * psu;
	*ty = mg + iy * psu;
    } else {
        *tx = mg + iy * psu;
	*ty = mg + xl - ix * psu;
    }
}

static void
putpnt(double x, double y)
{
    char   str[80];
    double tx, ty;

    sclrot(x, y, &tx, &ty);
    sprintf(str, " %.1f %.1f", tx, ty);
    putps(str);
    position(tx, ty, pen * psu);
}

static void
setpen(short pen)
{
    char   str[80];

    if (pen != lastpen) {
	sprintf(str, " %.1f W", pen * psu);
	putps(str);
        lastpen = pen;
    }
}
