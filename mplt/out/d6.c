/*
 * d6 - subroutines for producing CGM binary encoding
 */
#include <stdio.h>
#include <string.h>
#include "copa.h"
#include "output.h"
#include "device.h"
#include "clip.h"

#define DEFAULT_DEVICE  1
#define	INDEXED	        0
#define	DIRECT	        1
#define	NPTS            1024
#define STRIPS          256.0
#define VDC(x)          round((x)*adupi)
#define	round(x)        ((int)((x)+0.5))

#define LSB  ((int)((char *) &byt_ord)[0])
#define MSB  ((int)((char *) &byt_ord)[1])

static void begin_page(void);
static void colorout(int);
static void end_page(void);
static void intout(int);
static void encode(int, int, int);
static void cgm_header(void);
static void fountain(void);
static void background(void);
static void stroke(int);

static float adupi = 1000;
static float lastwidth = 0;
static int pen_color = 0, pen_width = 0;
static int lastcolor = -1;
static int lastfillcolor = -1;
static int plot_yet = 0;
static int colormode = DIRECT;
static int *xdp = NULL, *ydp = NULL, ndp = 0;
static short byt_ord = 256;

/* device 6 - CGM device */
int     d6open(void);
void    d6close(void), d6page(void);
void    d6trns(double, double, int);
void    d6pnwd(double);
void    d6fill(int, float *, float *, int, int);

/*
 * d6bind - override default device function
 */
void
d6bind(void)
{
    d_open = d6open;
    d_close = d6close;
    d_trns = d6trns;
    d_page = d6page;
    d_pnwd = d6pnwd;
    d_fill = d6fill;
}

/*
 * d6open - initialize plotting device
 */
int
d6open(void)
{
    xdp = (int *) calloc(NPTS * 2, sizeof(int));
    if (xdp == NULL)
	return (0);
    ydp = xdp + NPTS;
    ndp = 0;
    pen_color = 0; 
    pen_width = 0;
    lastwidth = 0;
    lastcolor = -1;
    lastfillcolor = -1;
    plot_yet = 0;
    dev_.adupi = adupi;		/* addressable units per inch */
    dev_.txpr = TXPRSTK;	/* text precision = stroke */
    dev_.prompt = 0;		/* prompt user at end of page */
    dev_.border = 0;		/* draw page border */
    dev_.maxpen = (short) adupi;/* maximum pen value */
    dev_.fillwhite = 1;		/* fill white areas */
    cgm_header();
    return (1);			/* 0 = unsuccessful open, 1 = successful open */
}

/*
 * d6close - close plotting device
 */
void
d6close(void)
{
    end_page();
    encode(0, 2, 0);		/* end metafile */

    free(xdp);
    xdp = NULL;
}

/*
 * d6trns - convert NC to device units and move or draw
 *
 *	xx,yy	coordinates in inches (NC) already clipped to page boundary:
 *		0 <= xx <= XPAGE  and  0 <= yy <= YPAGE
 *	do_gs	move/draw flag: 0 = move, 1 = draw
 */
void
d6trns(double xx, double yy, int do_gs)
{
    if (!plot_yet) {
	begin_page();
    } else if (do_gs == 0) {
	stroke(0);
	ndp = 0;
    }
    xdp[ndp] = VDC(xx);
    ydp[ndp] = VDC(yy);
    ndp++;
    if (ndp >= NPTS)
	stroke(1);
}

/*
 * d6page - signal new page (media change)
 */
void
d6page(void)
{
    end_page();
}

/*
 * d6pnwd -
 */
void
d6pnwd(double pw)
{
    stroke(1);
    if (pw < 0)
	pw *= (-0.005);	/* fix-up negative values */
    pen_width = VDC(limit(0.001, pw, 1.0));
    pen_color = dev_.fgci;
}

void
d6fill(int n, float *px, float *py, int fg, int nc)
{
    int     i, fc;
    float   xt, yt;

    if (!plot_yet)
	begin_page();
    else if (ndp > 1)
	stroke(1);
    encode(5, 22, 2);		        /* interior style */
    intout(1);			        /* solid fill */
    fc = fg ? dev_.fgci : dev_.bgci;    /* fill color */
    if (fc < 0)
	return;
    if (fc != lastfillcolor) {
	encode(5, 23, colormode == INDEXED ? 2 : 6);	/* set fill color */
	colorout(fc);
	lastfillcolor = fc;
    }
    encode(4, 7, n * 4);	/* polygon */

    for (i = 0; i < n; i++) {
	if (nc) {
	    xt = px[i];
	    yt = py[i];
	} else {
	    otrans(px[i], py[i], &xt, &yt);
	    xt = limit(cclip_.tcx1, xt, cclip_.tcx2);	/* crude clipping */
	    yt = limit(cclip_.tcy1, yt, cclip_.tcy2);
	}
	intout(VDC(xt));
	intout(VDC(yt));
    }
}

/*******************************************************************************/

static void
encode(int class, int id, int length)
{
    int     d;

    d = (class << 12) | (id << 5) | (length < 31 ? length : 0x1F);
    intout(d);
    if (length >= 31)
	intout(length);
    return;
}

static void
intout(int i)
{
    char *b;
    short s;

    s = (short) i;
    b = (char *) &s;
    put_ch(b[MSB]);
    put_ch(b[LSB]);
}

static void
colorout(int i)
{
    if (colormode == INDEXED)
	intout(i);
    else if (dev_.gray) {
	intout(i);
	intout(i);
	intout(i);
    } else {
	intout(ct[i].r);
	intout(ct[i].g);
	intout(ct[i].b);
    }
    return;
}

static void
cgm_header(void)
{
    int     i, n;
    static char *comment;

    comment = copa_.creator;
    n = (int) strlen(comment);
    encode(0, 1, n + 1);	/* begin metafile */
    put_ch(n);
    for (i = 0; i < n; i++)
	put_ch(comment[i]);

    encode(1, 1, 2);		/* metafile version */
    intout(1);
    encode(1, 3, 2);		/* VDC type (integer) */
    intout(0);
    encode(1, 4, 2);		/* integer precision */
    intout(16);
    encode(1, 6, 2);		/* index precision */
    intout(16);
    encode(1, 7, 2);		/* color precision */
    intout(16);
    encode(1, 8, 2);		/* color index precision */
    intout(16);
    encode(1, 9, 2);		/* maximum color index */
    intout(255);
    encode(1, 10, 12);		/* color value extent */
    intout(ct_min);
    intout(ct_min);
    intout(ct_min);
    intout(ct_max);
    intout(ct_max);
    intout(ct_max);
    encode(1, 11, 6);		/* metafile elements list */
    intout(1);
    intout(-1);
    intout(1);			/* drawing set & control set */
    return;
}

static void
begin_page(void)
{
    int     i;
    double  r2, g2, b2;

    encode(0, 3, 0);		/* begin picture */
    encode(2, 1, 6);		/* scaling mode (abstract) */
    intout(0);
    intout(0);
    intout(0);
    encode(2, 2, 2);		/* color selection mode */
    intout(colormode);
    encode(2, 3, 2);		/* line width specification mode (absolute) */
    intout(0);
    encode(2, 6, 8);		/* set vdc extents to (0,0), (32767,24968) */
    intout(VDC(0));
    intout(VDC(0));
    intout(VDC(XPAGE));
    intout(VDC(YPAGE));
    if (dev_.bgci >= 0 && dev_.fnci >= 0) {
        if (dev_.bgci != dev_.fnci) {
            r2 = g2 = b2 = ct_scale(ct_min) * 255;
        }
        else {
            r2 = ct_scale(ct[dev_.bgci].r) * 255;
            g2 = ct_scale(ct[dev_.bgci].g) * 255;
            b2 = ct_scale(ct[dev_.bgci].b) * 255;
        }
        encode(2, 7, 6);	/* set background color */
        intout(round(r2));
        intout(round(g2));
        intout(round(b2));
    }
    encode(0, 4, 0);		/* begin picture body */
    encode(3, 1, 2);		/* vdc integer precision */
    intout(16);
    encode(5, 2, 2);		/* line type (solid) */
    intout(1);

/* set color table... */
    if (colormode == INDEXED) {
	if (dev_.gray) {
	    for (i = 0; i < 256; i++) {
		encode(5, 34, 8);
		intout(i);
		intout(255 - i);
		intout(255 - i);
		intout(255 - i);
	    }
	} else {
	    for (i = 0; i < 16; i++) {
		encode(5, 34, 8);
		intout(i);
		intout(ct[i].r);
		intout(ct[i].g);
		intout(ct[i].b);
	    }
	}
    }
    if (dev_.bgci >= 0 && dev_.fnci >= 0) {
	if (dev_.fnci != dev_.bgci) {
	    fountain();
	} else {
	    background();
	}
    }

    lastwidth = (float) 0.3;
    lastcolor = -1;
    plot_yet = 1;
    ndp = 0;
    return;
}

static void
end_page(void)
{
    if (ndp > 1)
	stroke(0);
    encode(0, 5, 0);		/* end picture */
    fflush(fpout);
    lastwidth = 0;
    lastcolor = -1;
    plot_yet = 0;
    return;
}

static void
background(void)
{
    double   r2, g2, b2;

    r2 = ct_scale(ct[dev_.bgci].r) * 255;
    g2 = ct_scale(ct[dev_.bgci].g) * 255;
    b2 = ct_scale(ct[dev_.bgci].b) * 255;
    encode(5, 22, 2);		/* interior style */
    intout(1);			/* solid fill */
    encode(5, 23, 6);		/* set fill color */
    intout(round(r2));
    intout(round(g2));
    intout(round(b2));
    encode(4, 7, 16);		/* polygon */
    intout(VDC(0));
    intout(VDC(YPAGE));
    intout(VDC(XPAGE));
    intout(VDC(YPAGE));
    intout(VDC(XPAGE));
    intout(VDC(0));
    intout(VDC(0));
    intout(VDC(0));
    return;
}

static void
fountain(void)
{
    double  rstep, gstep, bstep, dy;
    double  r1, r2, g1, g2, b1, b2, y1, y2;

    r1 = ct_scale(ct[dev_.fnci].r) * 255;
    g1 = ct_scale(ct[dev_.fnci].g) * 255;
    b1 = ct_scale(ct[dev_.fnci].b) * 255;
    r2 = ct_scale(ct[dev_.bgci].r) * 255;
    g2 = ct_scale(ct[dev_.bgci].g) * 255;
    b2 = ct_scale(ct[dev_.bgci].b) * 255;
    rstep = (r2 - r1) / STRIPS;
    gstep = (g2 - g1) / STRIPS;
    bstep = (b2 - b1) / STRIPS;
    dy = YPAGE / STRIPS;
    y1 = YPAGE;
    y2 = YPAGE - dy;
    encode(5, 22, 2);		/* interior style */
    intout(1);			/* solid fill */
    while (y1 > 1e-6) {
	encode(5, 23, 6);	/* set fill color */
	intout(round(r1));
	intout(round(g1));
	intout(round(b1));
	encode(4, 7, 16);	/* polygon */
	intout(VDC(0));
	intout(VDC(y1));
	intout(VDC(XPAGE));
	intout(VDC(y1));
	intout(VDC(XPAGE));
	intout(VDC(y2));
	intout(VDC(0));
	intout(VDC(y2));
        r1 = ((((rstep > 0) && (r1 + rstep) > r2)) || ((rstep < 0) && (r1 + rstep) < r2)) ? r2 : (r1 + rstep);
        g1 = ((((gstep > 0) && (g1 + gstep) > r2)) || ((gstep < 0) && (g1 + gstep) < g2)) ? g2 : (g1 + gstep);
        b1 = ((((bstep > 0) && (b1 + bstep) > r2)) || ((bstep < 0) && (b1 + bstep) < b2)) ? b2 : (b1 + bstep);
	y1 = y2;
        y2 = (y1 > dy) ? (y1 - dy) : 0;
    }
    return;
}

static void
stroke(int contin)
{
    int     i;

    if (ndp > 1) {
	if (pen_width != lastwidth) {
	    encode(5, 3, 2);	/* line width */
	    intout((int) pen_width);
	    lastwidth = (float) pen_width;
	}
	if (pen_color != lastcolor) {
	    encode(5, 4, colormode == INDEXED ? 2 : 6);	/* set line color */
	    colorout(pen_color);
	    lastcolor = pen_color;
	}
	if (ndp == 2) {         /* fix disappearing dots */
	    if (xdp[0] == xdp[1] && ydp[0] == ydp[1]) {
	        xdp[1] = xdp[0] + 1;
	        ydp[1] = ydp[0];
	        xdp[2] = xdp[0];
	        ydp[2] = ydp[0] + 1;
	        xdp[3] = xdp[0] - 1;
	        ydp[3] = ydp[0];
	        xdp[4] = xdp[0];
	        ydp[4] = ydp[0] - 1;
	        xdp[5] = xdp[0] + 1;
	        ydp[5] = ydp[0];
		ndp = 6;
	    }
	}
	encode(4, 1, ndp * 4);	/* polyline */
	for (i = 0; i < ndp; i++) {
	    intout(xdp[i]);
	    intout(ydp[i]);
	}
	if (!contin) {
	    ndp = 0;
	} else {
	    xdp[0] = xdp[ndp - 1];
	    ydp[0] = ydp[ndp - 1];
	    ndp = 1;
	}
    }
}
