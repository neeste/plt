/*
 * d4grx - GRX graphics device
 */

#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <grx20.h>
#include <grdriver.h>
#include "copa.h"
#include "output.h"
#include "device.h"
#include "clip.h"

#define DEFAULT_DEVICE 4

#define XTRAN(x)    ((int)((x) * adupi_x))
#define YTRAN(y)    ((int)((YPAGE - (y)) * adupi_y))
#define nint(x)	    ((int)floor((x) + 0.5))
#define BGC(c)	    ((c<0)?dev_.wci:c)

int    getch();

static int setgray();
static void end_page();
static void begin_page();
static void fountain();

static int cleared = 0;
static int pnwd = 0;
static int wdln = 0;
static int xpix = 0;
static int ypix = 0;
static float adupi_x = 0;
static float adupi_y = 0;
static GrLineOption go;

/* device 4 - system dependent graphic device */
int     d4open();
void    d4close(), d4page(), d4setpal(), d4clip();
void    d4trns(double, double, int);
void    d4pnwd(double);
void    d4fill(int, float *, float *, int, int);

/*
 * d4bind - override default device function
 */
void
d4bind()
{
    d_open = d4open;
    d_close = d4close;
    d_trns = d4trns;
    d_page = d4page;
    d_pnwd = d4pnwd;
    d_fill = d4fill;
    d_setpal = d4setpal;
    d_clip = d4clip;
}

/*
 * d4open - initialize plotting device
 */
int
d4open()
{
    GrSetMode(GR_width_height_color_graphics, 640, 480, 256);
    xpix = GrSizeX();
    ypix = GrSizeY();
    adupi_x = xpix / XPAGE;
    adupi_y = ypix / YPAGE;
    dev_.adupi = adupi_x;       /* addressable units per inch */
    dev_.txpr = TXPRSTK;	/* text precision = stroke */
    dev_.prompt = 1;	        /* prompt user at end of page */
    dev_.border = 0;		/* draw page border */
    dev_.maxpen = 120;		/* maximum pen value */
    dev_.fillwhite = 1;         /* not possible to write white */
    dev_.widestroke = 1;	/* can do thick lines */
    go.lno_pattlen = 0;		/* continuous wide lines */

    return (1);			/* 0 = unsuccessful open, 1 = successful open */
}

/*
 * d4close - close plotting device
 */
void
d4close()
{
    end_page();
    GrSetMode(GR_default_text);
}

/*
 * d4trns - convert NC to device units and move or draw
 *
 *	xx,yy	coordinates in inches (NC) already clipped to page boundary:
 *		0 <= xx <= XPAGE  and  0 <= yy <= YPAGE
 *	do_gs	move/draw flag: 0 = move, 1 = draw
 */
void
d4trns(xx, yy, do_gs)
double  xx, yy;
int     do_gs;
{
    int     x, y, c;
    static int xpos = 0, ypos = 0;

    begin_page();
    x = XTRAN(xx);
    y = YTRAN(yy);
    c = dev_.fgci;
    if (dev_.gray) {
	c = setgray(c);
    }
    if (do_gs) {
	if (!wdln) {
            GrLine(xpos, ypos, x, y, c);
	} else {
            go.lno_width = pnwd;
	    go.lno_color = c;
            GrCustomLine(xpos, ypos, x, y, &go);
	}
    }
    if (pnwd > 1) {
	GrFilledCircle(x, y, pnwd / 2, c);
    }
    xpos = x;
    ypos = y;
}

/*
 * d4page - signal new page (media change)
 */
void
d4page()
{
    end_page();
}

/*
 * d4pnwd - set pen width
 */
void
d4pnwd(pw)
double  pw;			/* pen width in inches */
{
    if (pw < 0)
	pw *= (-0.005);		/* fix-up negative values */
    pnwd = pw * adupi_x;	/* pen width in x-pixels */
    wdln = (pnwd > 1.5 && dev_.widestroke);	/* test for wide lines */
}

/*
 *      Fill a polygon area.
 */
void
d4fill(n, px, py, fg, nc)
int     n, fg;
float  *px, *py;
{
    int     p[300][2], *ix, *iy;
    float   xt, yt;
    int     i, c;

    if (n < 3)
	return;
    begin_page();
    ix = (int *) malloc(2 * n * sizeof(int));
    if (ix == NULL)
	return;
    iy = ix + n;
    for (i = 0; i < n; i++) {
	if (nc) {
	    xt = px[i];
	    yt = py[i];
	} else {
	    otrans((double) px[i], (double) py[i], &xt, &yt);
	}
	p[i][0] = XTRAN(xt);
	p[i][1] = YTRAN(yt);
    }
    c = (fg) ? dev_.fgci : BGC(dev_.bgci);
    if (c < 0)
	c = dev_.wci;
    if (dev_.gray)
        c = setgray(c);
    GrFilledPolygon(n, p, c);
    free(ix);
}

void
d4setpal()
{
    int     i, n, rr, gg, bb;
    double  s;

    n = dev_.nci;
    s = (ct_max > 0) ? 1.0 / ct_max : 1.0;
    for (i = 0; i < n; i++) {
	rr = nint(ct[i].r * s * 255);
	gg = nint(ct[i].g * s * 255);
	bb = nint(ct[i].b * s * 255);
	GrSetColor(125 + i, rr, gg, bb);
    }
}

void
d4clip()
{
    int    ix1, iy1, ix2, iy2;

    ix1 = XTRAN(cclip_.tcx1);
    iy1 = YTRAN(cclip_.tcy1);
    ix2 = XTRAN(cclip_.tcx2);
    iy2 = YTRAN(cclip_.tcy2);
    GrSetClipBox(ix1, iy1, ix2, iy2);
}

int
chkdev()
{
    return (DEFAULT_DEVICE);
}

static void
begin_page()
{
    if (cleared)
        return;
    if (BGC(dev_.fnci) == BGC(dev_.bgci))
	GrClearScreen(BGC(dev_.bgci));
    else
	fountain();
    cleared = 1;
}

static void
end_page()
{
    int     c;

    if (!cleared)
	return;
    if (dev_.prompt) {
	while ((c = getch()) != '\r')
	    continue;
    } else {
	usleep(100000);
    }
    cleared = 0;
}

static void
fountain()
{
    double  fnr, fng, fnb, bgr, bgg, bgb, s;
    int     i, y1, y2;
    int rr, gg, bb;
    static int nb = 65;

    fnr = ct_scale(ct[dev_.fnci].r) * 255;
    fng = ct_scale(ct[dev_.fnci].g) * 255;
    fnb = ct_scale(ct[dev_.fnci].b) * 255;
    bgr = ct_scale(ct[dev_.bgci].r) * 255;
    bgg = ct_scale(ct[dev_.bgci].g) * 255;
    bgb = ct_scale(ct[dev_.bgci].b) * 255;
    y2 = 0;
    for (i = 0; i < nb; i++) {
	y1 = y2;
	y2 = ((i + 1) * (long) (ypix - 1)) / nb;
	s = i / (nb - 1.0);
	rr = nint(fnr * (1 - s) + bgr * s);
	gg = nint(fng * (1 - s) + bgg * s);
	bb = nint(fnb * (1 - s) + bgb * s);
	GrSetColor(125 + i, rr, gg, bb);
	GrFilledBox(0, y1, xpix - 1, y2 - 1, 125 + i);
    }
    GrFilledBox(0, y2, xpix - 1, ypix - 1, dev_.bgci);
}

static int
setgray(g)
{
    int     i, n, c, gray;
    int     ir, ig, ib;
    int     d, dmn;

    n = dev_.nci;
    if (n < 190) {
	gray = nint(ct_scale(g) * 255);
	c = (int) (190 + gray / 4);
	GrSetColor(c, gray, gray, gray);
    } else {
	c = 0;
	dmn = ct_max;
	for (i = 0; i < n; i++) {
	    ir = ct[i].r;
	    ig = ct[i].g;
	    ib = ct[i].b;
	    if ((ir == ig) && (ig == ib) && (ib == ir)) {
		d = (ir > g) ? ir - g : g - ir;
		if (dmn > d) {
		    dmn = d;
		    c = i;
		    if (dmn == 0)
			break;
		}
	    }
	}
    }
    return (c);
}

