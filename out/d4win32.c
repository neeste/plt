/*
 * d4win32 - win32 graphics
 */

#ifdef WIN32
#include <windows.h>
#include <windowsx.h>
#include <process.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "copa.h"
#include "output.h"
#include "device.h"
#include "clip.h"
#include "winout.h"

#define DEFAULT_DEVICE      4
#define XTRAN(x,y)          ((int)(txx * x + txy * y + tx0 + 0.5))
#define YTRAN(x,y)          ((int)(tyx * x + tyy * y + ty0 + 0.5))
#define NPTS                1024
#define BGC(c)		    ((c<0)?dev_.wci:c)
#define MAXINF		    80

static void begin_page();
static void end_page();
static void set_clip();
static void stroke();
static COLORREF set_color();

static double tx0 = 0;
static double ty0 = 0;
static double txx = 0;
static double tyx = 0;
static double txy = 0;
static double tyy = 0;
static float zoom = 1;
static float zx = 0;
static float zy = 0;
static float adupi_x = 0;
static float adupi_y = 0;
static int cleared = 0;
static int cleared_yet = 0;
static int xpos = 0;
static int ypos = 0;
static int xpix = 0;
static int ypix = 0;
static int cx1 = 0;
static int cy1 = 0;
static int cx2 = 0;
static int cy2 = 0;
static int npt = 0;
static int rotate = 0;
static HDC hDCOut = 0;
static HPEN hpen = 0;
static HPEN open = 0;
static HRGN hclp = 0;
static LOGPEN lpen;
static POINT pnpt = {0, 0};
static POINT pt[NPTS];
static RECT lpSize = {0, 0, 0, 0};

/* device 4 - system dependent graphic device */
int     d4open(void);
void    d4close(void), d4page(void);
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
}

/*
 * d4open - initialize plotting device
 */
int
d4open()
{
    if (hDCOut == 0)
        return (0);		/* 0 = unsuccessful open */

    if (hpen) {
        DeleteObject(hpen);
    }
    lpen.lopnStyle = PS_SOLID;
    lpen.lopnWidth = pnpt;
    lpen.lopnColor = RGB(0, 0, 0);
    hpen = CreatePenIndirect(&lpen);
    open = SelectPen(hDCOut, hpen);	// save old pen

    xpix = lpSize.right - lpSize.left;
    ypix = lpSize.bottom - lpSize.top;
    rotate = (xpix < ypix);
    if (rotate) {
        adupi_x = xpix * zoom / YPAGE;
	adupi_y = ypix * zoom / XPAGE;
    } else {
        adupi_x = xpix * zoom / XPAGE;
	adupi_y = ypix * zoom / YPAGE;
    }
    if (adupi_x > adupi_y) {
        adupi_x = adupi_y;
    } else if (adupi_y > adupi_x) {
        adupi_y = adupi_x;
    }
    if (rotate) {
     	tx0 = lpSize.left - zx;
    	ty0 = lpSize.top - zy;
    	txx = 0;
    	txy = adupi_x;
    	tyx = adupi_y;
    	tyy = 0;
    } else {
     	tx0 = lpSize.left - zx;
    	ty0 = lpSize.bottom + zy;
    	txx = adupi_x;
    	tyx = 0;
    	txy = 0;
    	tyy = -adupi_y;
   }
    dev_.adupi = adupi_x * 2;	/* addressable units per inch */
    dev_.txpr = TXPRSTK;	/* text precision = stroke */
    dev_.prompt = 1;		/* prompt user at end of page */
    dev_.border = 0;		/* draw page border */
    dev_.maxpen = 20;		/* maximum pen value */
    dev_.fillwhite = 1;		/* possible to write white */

    npt = 0;
    cx1 = cy1 = cx2 = cy2 = 0;
    cleared_yet = 0;

    return (1);			/* 1 = successful open */
}

/*
 * d4close - close plotting device
 */
void
d4close()
{
    if (!cleared_yet)
        begin_page();
    end_page();
    if (hpen) {
        (void) SelectPen(hDCOut, open);	// restore old pen
        DeleteObject(hpen);
	hpen = 0;
    }
    if (hclp) {
        SelectClipRgn(hDCOut, NULL);    // remove previous clipping region
	DeleteObject(hclp);
	hclp = 0;
    }
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
    int x, y;

    begin_page();
    x = XTRAN(xx, yy);
    y = YTRAN(xx, yy);
    if (do_gs == 0)
        stroke(0);
    else if (npt >= NPTS)
        stroke(1);
    pt[npt].x = x;
    pt[npt].y = y;
    npt++;
    xpos = x;
    ypos = y;
    set_clip(1);
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
    if (npt > 0)
        stroke(1);
    if (pw < 0)
	pw *= (-1.0 / 200.0);	/* fix-up negative values */
    pnpt.x = (int) (pw * adupi_x + 0.5);
    pnpt.y = (int) (pw * adupi_y + 0.5);
    lpen.lopnWidth = pnpt;
    lpen.lopnColor = set_color(dev_.fgci);
    hpen = CreatePenIndirect(&lpen);
    DeleteObject(SelectPen(hDCOut, hpen));
}

/*
 * d4fill - fill area with solid white or black (WC)
 *	  (do this in NC to avoid duplicating arrays)
 */
void
d4fill(n, px, py, fg, nc)
int     n, fg, nc;
float  *px, *py;
{
    int     i, c, x1, y1, x2, y2;
    float   xt, yt;
    POINT  *pnt;
    HBRUSH  hbr;
    HRGN    hclp, hrgn;

    if (npt > 0)
        stroke(0);
    else
        begin_page();
    set_clip(0);
    x1 = XTRAN(cclip_.tcx1, cclip_.tcy1);
    y1 = YTRAN(cclip_.tcx1, cclip_.tcy1);
    x2 = XTRAN(cclip_.tcx2, cclip_.tcy2);
    y2 = YTRAN(cclip_.tcx2, cclip_.tcy2);
    if (x1 > x2) {
    	c = x1;
    	x1 = x2;
    	x2 = c;
    }
    if (y1 > y2) {
    	c = y1;
    	y1 = y2;
    	y2 = c;
    }
    hclp = CreateRectRgn(x1, y1, x2 + 1, y2 + 1);
    SelectClipRgn(hDCOut, hclp);
    c = (fg) ? dev_.fgci : BGC(dev_.bgci);
    hbr = CreateSolidBrush(set_color(c));
    pnt = (POINT *) calloc(n, sizeof(POINT));
    for (i = 0; i < n; i++) {
	if (nc) {
	    xt = px[i];
	    yt = py[i];
	} else {
	    otrans(px[i], py[i], &xt, &yt);
	}
	pnt[i].x = XTRAN(xt, yt);
	pnt[i].y = YTRAN(xt, yt);
    }
    hrgn = CreatePolygonRgn(pnt, n, ALTERNATE);
    FillRgn(hDCOut, hrgn, hbr);
    SelectClipRgn(hDCOut, NULL);
    DeleteObject(hclp);
    DeleteObject(hbr);
    DeleteObject(hrgn);
    free(pnt);
}

int
chkdev()
{
    return (DEFAULT_DEVICE);
}

static COLORREF
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
    int x1, y1, x2, y2, zz;

    if (clip) {
        x1 = XTRAN(cclip_.tcx1, cclip_.tcy1);
	y1 = YTRAN(cclip_.tcx1, cclip_.tcy1);
	x2 = XTRAN(cclip_.tcx2, cclip_.tcy2);
	y2 = YTRAN(cclip_.tcx2, cclip_.tcy2);
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
	x2 = xpix;
	y2 = ypix;
    }
    if (x1 != cx1 || y1 != cy1 || x2 != cx2 || y2 != cy2) {
	if (hclp) {
	    SelectClipRgn(hDCOut, NULL);    // remove previous clipping region
            DeleteObject(hclp);
	}
        hclp = CreateRectRgn(x1, y1, x2, y2);
	SelectClipRgn(hDCOut, hclp);
	cx1 = x1;
	cy1 = y1;
	cx2 = x2;
	cy2 = y2;
    }
}

static void
rectangle(RECT *R, int c)
{
    HBRUSH hbr;

    hbr = CreateSolidBrush(c);
    FillRect(hDCOut, R, hbr);
    DeleteObject(hbr);
}

static void
scr_clear()
{
    rectangle(&lpSize, set_color(BGC(dev_.bgci)));
}

static void
fountain()
{
    float   w0, w1;
    int     i, j, k, r, g, b, h;
    int	    r0, g0, b0, r1, g1, b1;
    static int nb = 65;
    RECT   strip;

    j = BGC(dev_.bgci);
    k = BGC(dev_.fnci);
    w0 = zy / (zoom * ypix);
    w1 = (zy + ypix) / (zoom * ypix);
    r0 = (int) (ct[j].r * (1 - w0) + ct[k].r * w0 + 0.5);
    g0 = (int) (ct[j].g * (1 - w0) + ct[k].g * w0 + 0.5);
    b0 = (int) (ct[j].b * (1 - w0) + ct[k].b * w0 + 0.5);
    r1 = (int) (ct[j].r * (1 - w1) + ct[k].r * w1 + 0.5);
    g1 = (int) (ct[j].g * (1 - w1) + ct[k].g * w1 + 0.5);
    b1 = (int) (ct[j].b * (1 - w1) + ct[k].b * w1 + 0.5);
    if (!rotate)
        h = lpSize.bottom - lpSize.top;
    else
        h = lpSize.right - lpSize.left;
    strip.right = lpSize.right;
    strip.left = lpSize.left;
    strip.top = lpSize.top;
    strip.bottom = lpSize.bottom;
    for (i = 0; i < nb; i++) {
        if (!rotate)
            strip.bottom = lpSize.top + ((i + 1) * h) / nb;
        else
            strip.left = lpSize.right - ((i + 1) * h) / nb;
        r = (((r0 * i + r1 * (nb - i)) / nb) * 255) / ct_max;
        g = (((g0 * i + g1 * (nb - i)) / nb) * 255) / ct_max;
        b = (((b0 * i + b1 * (nb - i)) / nb) * 255) / ct_max;
        rectangle(&strip, RGB(r,g,b));
        if (!rotate)
            strip.top = strip.bottom;
        else
            strip.right = strip.left;
    }
    strip.bottom = lpSize.bottom;
    r = (ct[j].r * 255) / ct_max;
    g = (ct[j].g * 255) / ct_max;
    b = (ct[j].b * 255) / ct_max;
    rectangle(&strip, RGB(r,g,b));
}

/* box - draw border
 */
static void
box()
{
    float   x = XPAGE, y = YPAGE, z = 0.0;

    d4trns(z, y, 0);
    d4trns(x, y, 1);
    d4trns(x, z, 1);
    d4trns(z, z, 1);
    d4trns(z, y, 1);
}

/* stroke - draw path connecting points
 */
static void
stroke(int contin)
{
    xpos = pt[npt - 1].x;
    ypos = pt[npt - 1].y;
    if (npt > 1) {
        (void) SelectPen(hDCOut, hpen);
        Polyline(hDCOut, pt, npt);
        (void) SetPixel(hDCOut, xpos, ypos, lpen.lopnColor);
    }
    if (contin) {
        pt[0].x = xpos;
        pt[0].y = ypos;
        npt = 1;
    } else {
        npt = 0;
    }
}

/****************************************************************************/

static void (*win_begin_page)() = NULL;
static void (*win_end_page)() = NULL;

static void
begin_page()
{
    if (cleared)
        return;
    set_clip(0);
    if (BGC(dev_.fnci) == BGC(dev_.bgci)) {
        scr_clear();
    } else {
        fountain();
    }
    if (dev_.border) {
        box();
    }
    if (win_begin_page) {
        win_begin_page();
    }
    cleared_yet = cleared = 1;
}

static void
end_page()
{
    if (npt > 0)
        stroke(0);
    if (win_end_page)
        win_end_page();
    cleared = 0;
}

void
win_page(void (*bp)(), void (*ep)())
{
    win_begin_page = bp;
    win_end_page = ep;
}

/****************************************************************************/

static void (*win_error_proc)(int, char *) = NULL;

void
win_errs(void (*ep)(int, char *))
{
    win_error_proc = ep;
}

void
win_errmsg(int ne, char *fn)
{
    if (win_error_proc)
	win_error_proc(ne, fn);
}

/*****************************************************************************/

void
win_DC(HDC hdc, int l, int b, int r, int t)
{
    hDCOut = hdc;

    lpSize.left = l;
    lpSize.bottom = b;
    lpSize.right = r;
    lpSize.top = t;
}

/*****************************************************************************/

void
info_line(HDC hdc, POINTS mc, int mode)
{
    char    s[MAXINF];
    double  x, y, cx, cy;
    int     m, n;
    static char *r[3] = {"page", "frame", "data"};
    static int lnc = 0;

    if (adupi_x <= 0 || adupi_y <= 0)
	return;
    if (rotate) {
        cx = mc.y + zy;
	cy = mc.x + zx;
    } else {
        cx = mc.x + zx;
	cy = ypix - mc.y + zy;
    }
    x = cx / (double) adupi_x;
    y = cy / (double) adupi_y;
    m = inq_frame(&x, &y, mode);
    if (m == 2) {
        sprintf(s, "%s(x,y) = %.3g, %.3g    ", r[m], x, y);
    } else {
        sprintf(s, "%s(x,y) = %.2f, %.2f    ", r[m], x, y);
    }
    m = strlen(s);
    for (n = m; n < lnc && n < (MAXINF - 1); n++)
	s[n] = ' ';
    s[n] = '\0';
    lnc = m;
    TextOut(hdc, 0, 3, s, n);
}

void
disp_zoom(POINTS mc, int zoom_level)
{
    float a, mx, my;
    static float sqrt2 = (float) 1.4142135623730;
    static float last_zoom = 1;

    if (adupi_x <= 0 || adupi_y <= 0)
	return;
    zoom_level = limit(-20, zoom_level, 20);
    if (zoom_level <= 0) {
	zoom = last_zoom = 1;
	zx = zy = 0;
    } else {
        zoom = (float) pow(sqrt2, (double) zoom_level);
	a = zoom / last_zoom;
	mx = mc.x;
	my = rotate ? (float) mc.y : ypix - mc.y;
	last_zoom = zoom;
        zx += (mx + zx) * (a - 1);  // zoom in on mouse location
        zy += (my + zy) * (a - 1);
        zx += (mx - xpix / 2) / 2;  // move toward window center
        zy += (my - ypix / 2) / 2;
	zx = limit(0, zx, xpix * (zoom - 1));
	zy = limit(0, zy, ypix * (zoom - 1));
    }
}

POINTS
disp_move(POINTS p1, POINTS p0)
{
    zx -= p1.x - p0.x;
    zy -= (float) (rotate ? (p1.y - p0.y) : -(p1.y - p0.y));    
    return (p1);
}

void
disp_ofix()
{
    zx = limit(0, zx, xpix * (zoom - 1));
    zy = limit(0, zy, ypix * (zoom - 1));
}

#endif /* WIN32 */
