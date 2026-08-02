/*
 * d8 - scalable vector graphics (SVG)
 */

#include <stdio.h>
#include "copa.h"
#include "clip.h"
#include "output.h"
#include "device.h"

#define	NPTS            1024
#define VDC(x)		((x)*adupi*psu)
#define XTRAN(x)	((float)((x)*adupi*psu))
#define YTRAN(y)	((float)((YPAGE-(y))*adupi*psu))
#define RGB(r,g,b)	(((r)<<16)|((g)<<8)|b)

/* device 8 - NULL device */
int     d8open(void);
void    d8close(void), d8page(void);
void    d8trns(double, double, int);
void    d8pnwd(double);
void    d8fill(int, float *, float *, int, int);
void    d8curv(float *, float *);

static int  set_color(int);
static void begin_page(void);
static void end_page(void);
static void putpnt(char *, double, double, char *);
static void putstr(char *);
static void set_clip(int);
static void stroke(int);

static char *tpt = NULL;
static double adupi = 7200;
static double psu = 0.01;   // = 72 / adupi
static double pnwd = 0;
static double cx1 = 0;
static double cy1 = 0;
static double cx2 = 0;
static double cy2 = 0;
static float *xpt = NULL;
static float *ypt = NULL;
static int cleared = 0;
static int npt = 0;
static int pnco = 0;
static int cpn = 0;
static int pgn = 0;

/*
 * d8bind - overide default device function
 */
void
d8bind(void)
{
    d_open = d8open;
    d_close = d8close;
    d_trns = d8trns;
    d_page = d8page;
    d_pnwd = d8pnwd;
    d_fill = d8fill;
    d_curv = d8curv;
}

/*
 * d8open - initialize plotting device
 */
int
d8open(void)
{
    putstr("<?xml version=\"1.0\" standalone=\"no\"?>");
    putstr("<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\""); 
    putstr("\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">");
    fprintf(fpout, "<svg width=\"%.2fin\" height=\"%.2fin\" ", XPAGE * 0.72, YPAGE * 0.72);
    fprintf(fpout, "viewbox=\"0 0 %.0f %.0f\"\n", VDC(XPAGE), VDC(YPAGE));
    putstr("    xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">");
    putstr("<title>");
    putstr(copa_.title);
    putstr("</title>");
    putstr("<desc>");
    fprintf(fpout, "Creator: %s\n", copa_.creator);
    fprintf(fpout, "Date: %s\n", copa_.date);
    putstr("</desc>");
    putstr("<g style=\"stroke-linecap:round;stroke-linejoin:round\">");

    dev_.adupi = (float) adupi;	/* addressable units per inch */
    dev_.txpr = TXPRSTK;	/* text precision = stroke */
    dev_.prompt = 0;		/* prompt user at end of page */
    dev_.border = 0;		/* draw page border */
    dev_.maxpen = 0;		/* maximum pen value */
    dev_.fillwhite = 1;		/* possible to write white */

    xpt = (float *) calloc(NPTS, 2 * sizeof(float) + 1);
    if (xpt == NULL)
	return (0);
    ypt = xpt + NPTS;
    tpt = (char *) (ypt + NPTS);
    npt = 0;
    cleared = 0;
    cx1 = cy1 = cx2 = cy2 = 0;
    cpn = pgn = 0;

    return (1);			/* 0 = unsuccessful open, 1 = successful open */
}

/*
 * d8close - close plotting device
 */
void
d8close(void)
{
    end_page();
    putstr("</g><!-- EOF -->");
    putstr("</svg>");
}

/*
 * d8trns - convert NC to device units and move or draw
 *
 *	xx,yy	coordinates in inches (NC) already clipped to page boundary:
 *		0 <= xx <= XPAGE  and  0 <= yy <= YPAGE
 *	do_gs	move/draw flag: 0 = move, 1 = draw
 */
void
d8trns(double xx, double yy, int do_gs)
{
    float x, y;

    begin_page();
    x = XTRAN(xx);
    y = YTRAN(yy);
    if (do_gs == 0 && npt > 0) {
	if (x == xpt[npt - 1] && y == ypt[npt - 1]) {
	    return;
	} else {
	    stroke(0);
	}
    } else if (npt >= NPTS - 1) {
        stroke(1);
    }
    xpt[npt] = x;
    ypt[npt] = y;
    tpt[npt] = 0;
    npt++;
    set_clip(1);
}

/*
 * d4curv - convert NC to device units and add curve to path
 *
 *	xx,yy	coordinates of Bezier curve points
 *              in inches (NC) already clipped to page boundary:
 */
void
d8curv(float *xx, float *yy)
{
    int     i;

    begin_page();
    if (npt >= NPTS - 3)
        stroke(1);
    for (i = 0; i < 3; i++) {
        xpt[npt] = XTRAN(xx[i]);
        ypt[npt] = YTRAN(yy[i]);
	tpt[npt] = (i == 0) ? 1 : 0;
	npt++;
    }
    set_clip(1);
}

/*
 * d8page - signal new page (media change)
 */
void
d8page(void)
{
    end_page();
}

/*
 * d8pnwd -
 */
void
d8pnwd(double pw)
{
    if (npt > 0)
        stroke(0);
    if (pw < 0)
	pw *= (-1.0 / 200);	/* fix-up negative values */
    pnwd = VDC(pw);
    pnco = dev_.fgci;
}

void
d8fill(int n, float *px, float *py, int fg, int nc)
{
    float xt, yt, x, y;
    int i, ci, c;

    if (npt > 0)
        stroke(0);
    else
        begin_page();
    set_clip(1);
    ci = fg ? dev_.fgci : dev_.bgci;
    if (ci < 0)
	return;
    c = set_color(ci);
    fprintf(fpout, "<path style=\"stroke:none;");
    fprintf(fpout, "fill:#%06X;\"\n", c);
    for (i = 0; i < n; i++) {
	if (nc) {
	    xt = px[i];
	    yt = py[i];
	} else {
	    otrans(px[i], py[i], &xt, &yt);
	}
	x = XTRAN(xt);
	y = YTRAN(yt);
        if (i == 0) {
	    fprintf(fpout, "clip-path=\"url(#cp%d)\" d=\"\n", cpn);
	    putpnt("M", x, y, "\n");
	} else {
	    putpnt("L", x, y, "\n");
	}
    }
    fprintf(fpout, "\"/>\n");
}

/*****************************************************************************/

void
putstr(char *s)
{
    fprintf(fpout, "%s\n", s);
}

void
putpnt(char *s1, double x, double y, char *s2)
{
    fprintf(fpout, "%s %.1f %.1f%s", s1, x, y, s2);
}

/*****************************************************************************/

static int
set_color(int c)
{
    int r, g, b;

    if (dev_.gray) {
    	r = g = b = (int) (ct_scale(c) * 255 + 0.5);
    } else {
        r = (int) (ct_scale(ct[c].r) * 255 + 0.5);
        g = (int) (ct_scale(ct[c].g) * 255 + 0.5);
        b = (int) (ct_scale(ct[c].b) * 255 + 0.5);
    }
    return (RGB(r,g,b));
}


static void
set_clip(int clip)
{
    double x1, y1, x2, y2, zz;

    if (clip) {
        x1 = XTRAN(cclip_.tcx1);
	y1 = YTRAN(cclip_.tcy1);
	x2 = XTRAN(cclip_.tcx2);
	y2 = YTRAN(cclip_.tcy2);
        if (x1 > x2) {
	    zz = x1;
    	    x1 = x2;
    	    x2 = zz;
	}
	if (y1 > y2) {
    	    zz = y1;
    	    y1 = y2;
    	    y2 = zz;
	}
    } else {
	x1 = 0;
	y1 = 0;
	x2 = VDC(XPAGE);
	y2 = VDC(YPAGE);
    }
    if (x1 != cx1 || y1 != cy1 || x2 != cx2 || y2 != cy2) {
	// set clipping
	fprintf(fpout, "<clipPath id=\"cp%d\"> <path d=\"\n", ++cpn);
	fprintf(fpout, "M %.1f %.1f", x1, y1);
	fprintf(fpout, " %.1f %.1f", x1, y2);
	fprintf(fpout, " %.1f %.1f", x2, y2);
	fprintf(fpout, " %.1f %.1f", x2, y1);
	putstr("\"/> </clipPath>");
	cx1 = x1;
	cy1 = y1;
	cx2 = x2;
	cy2 = y2;
    }
}

static void
rectangle(double x1, double y1, double x2, double y2, int c)
{
    fprintf(fpout, "<path style=\"stroke:none;");
    fprintf(fpout, "fill:#%06X;\" d=\"\n", c);
    putpnt("M", x1, y1, "");
    putpnt("L", x1, y2, "");
    putpnt("L", x2, y2, "");
    putpnt("L", x2, y1, "");
    fprintf(fpout, "\"/>\n");
}

static void
scr_clear(void)
{
    int     c;

    if (dev_.bgci >= 0) {
	c = set_color(dev_.bgci);
	rectangle(0, 0, VDC(XPAGE), VDC(YPAGE), c);
    }
}

static void
fountain(void)
{
    int     c;

    putstr("<g>");
    putstr("  <defs>");
    putstr("    <linearGradient id=\"MyGradient\" x1=\"0%\" y1=\"0%\" x2=\"0%\" y2=\"100%\">");
    c = set_color(dev_.fnci);
    fprintf(fpout, "    <stop offset=\"1%%\" stop-color=\"#%06X\" />\n", c);
    c = set_color(dev_.bgci);
    fprintf(fpout, "    <stop offset=\"99%%\" stop-color=\"#%06X\" />\n", c);
    putstr("    </linearGradient>");
    putstr("  </defs>");
    putstr("  <rect fill=\"url(#MyGradient)\" stroke=\"none\"");  
    fprintf(fpout, "    x=\"0\" y=\"0\" width=\"%.0f\" height=\"%.0f\"/>\n", VDC(XPAGE), VDC(YPAGE));
    putstr("</g>");
}

/****************************************************************************/

static void
begin_page(void)
{
    if (cleared)
        return;
    fprintf(fpout, "<g id=\"page %d\">\n", ++pgn);
    set_clip(0);
    if (dev_.fnci >= 0 && dev_.bgci >= 0) {
        if (dev_.fnci == dev_.bgci)
	    scr_clear();
	else
	    fountain();
    }
    cleared = 1;
}

static void
end_page(void)
{
    if (npt)
        stroke(0);
    fprintf(fpout, "</g><!-- page %d -->\n", pgn);
    cleared = 0;
}

void
stroke(int contin)
{
    int     i, c;

    if (npt > 1) {
	c = set_color(pnco);
        fprintf(fpout, "<path style=\"fill:none;");
        fprintf(fpout, "stroke:#%06X;", c);
        fprintf(fpout, "stroke-width:%.2f;\"\n", pnwd);
	fprintf(fpout, "clip-path=\"url(#cp%d)\" d=\"\n", cpn);
	putpnt("M", xpt[0], ypt[0], "\n");
	for (i = 1; i < npt; i++) {
	    if (tpt[i] == 0) {
		putpnt("L", xpt[i], ypt[i], "\n");
	    } else {
	        putpnt("C", xpt[i], ypt[i], "");
		i++;
	        putpnt("", xpt[i], ypt[i], "");
		i++;
	        putpnt("", xpt[i], ypt[i], "\n");
	    }
	}
	fprintf(fpout, "\"/>\n");
    }
    if (contin && npt > 0) {
	xpt[0] = xpt[npt - 1];
	ypt[0] = ypt[npt - 1];
	npt = 1;
    } else {
        npt = 0;
    }
}
