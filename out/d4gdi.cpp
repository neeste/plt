/*
 * d4gdi - Windows GDI+
 */

#ifdef WIN32
#include <windows.h>
#include <windowsx.h>
#include <process.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gdi.h"

#define DEFAULT_DEVICE      4
#define NPTS                1024
#define XTRAN(x,y)          ((int)(txx * x + txy * y + tx0 + 0.5))
#define YTRAN(x,y)          ((int)(tyx * x + tyy * y + ty0 + 0.5))
#define BGC(c)		    ((c<0)?dev_.wci:c)

/*****************************************************************************/

extern "C" {

#include "output.h"
#include "device.h"
#include "copa.h"
#include "clip.h"
#include "winout.h"

int     chkdev();
void    d4bind();

/* device 4 - system dependent graphic device */
int     d4open();
void    d4close();
void    d4trns(double, double, int);
void    d4page();
void    d4pnwd(double);
void    d4fill(int, float *, float *, int, int);
void    d4curv(float *, float *);

void    win_page(void (*bp)(), void (*ep)());
void    win_DC(HDC, int, int, int, int);

}

using namespace Gdiplus;

GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR           gdiplusToken;

/*****************************************************************************/

static void begin_page();
static void end_page();
static void get_rgb(int, int *, int *, int *);
static void scr_clear();
static void set_clip(int);
static void stroke(int);

static double adupi = 0;
static double tx0 = 0;
static double ty0 = 0;
static double txx = 0;
static double tyx = 0;
static double txy = 0;
static double tyy = 0;
static float pnwd = 0;
static int cleared = 0;
static int cleared_yet = 0;
static int xpos = 0;
static int ypos = 0;
static int xpix = 0;
static int ypix = 0;
static int gdi = 0;
static int npt = 0;
static int pnco = 0;
static int rotate = 0;
static int cx1 = 0;
static int cy1 = 0;
static int cx2 = 0;
static int cy2 = 0;
static int *xpt = NULL;
static int *ypt = NULL;
static HDC hDCOut = 0;
static HRGN hclp = 0;
static RECT lpSize = {0, 0, 0, 0};

/*****************************************************************************/

/*
 * d4bind - overide default device function
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
    d_curv = d4curv;
}

/*
 * d4open - initialize plotting device
 */
int
d4open()
{
    double adupi_x, adupi_y;

    if (hDCOut == 0)
        return (0);		/* 0 = unsuccessful open */
   
    // Initialize GDI+.
    gdi = GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    xpix = lpSize.right - lpSize.left;
    ypix = lpSize.bottom - lpSize.top;
    rotate = (xpix < ypix);
    if (rotate) {
        adupi_x = xpix / YPAGE;
	adupi_y = ypix / XPAGE;
    } else {
        adupi_x = xpix / XPAGE;
	adupi_y = ypix / YPAGE;
    }
    adupi = (adupi_x < adupi_y) ? adupi_x : adupi_y;
    if (rotate) {
     	tx0 = lpSize.left;
    	ty0 = lpSize.top;
    	txx = 0;
    	txy = adupi;
    	tyx = adupi;
    	tyy = 0;
    } else {
     	tx0 = lpSize.left;
    	ty0 = lpSize.top + adupi * YPAGE;
    	txx = adupi;
    	tyx = 0;
    	txy = 0;
    	tyy = -adupi;
   }
    dev_.adupi = (float) adupi;	/* addressable units per inch */
    dev_.txpr = TXPRSTK;	/* text precision = stroke */
    dev_.prompt = 1;		/* prompt user at end of page */
    dev_.border = 0;		/* draw page border */
    dev_.maxpen = 40;		/* maximum pen value */
    dev_.fillwhite = 1;		/* possible to write white */

    xpt = (int *) calloc(NPTS * 2, sizeof(int));
    if (xpt == NULL)
	return (0);
    ypt = xpt + NPTS;
    npt = 0;
    cleared = 0;
    cx1 = cy1 = cx2 = cy2 = 0;

    return (1);			/* 1 = successful open */
}

/*
 * d4close - close plotting device
 */
void
d4close()
{
    end_page();
    GdiplusShutdown(gdiplusToken);
    if (hclp) {
        DeleteObject(hclp);
	hclp = 0;
    }
    if (xpt) {
	free(xpt);
	xpt = NULL;
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
d4trns(double xx, double yy, int do_gs)
{
    begin_page();
    if (do_gs == 0)
        stroke(0);
    else if (npt >= NPTS - 1)
        stroke(1);
    xpt[npt] = XTRAN(xx, yy);
    ypt[npt] = YTRAN(xx, yy);
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
d4curv(float *xx, float *yy)
{
    int	    x1, y1, xa, ya, xb, yb, x2, y2;
    int	    r, g, b;

    if (npt > 0)
        stroke(1);
    else
        begin_page();
    set_clip(1);
    x1 = xpos;
    y1 = ypos;
    xa = XTRAN(xx[0], yy[0]);
    ya = YTRAN(xx[0], yy[0]);
    xb = XTRAN(xx[1], yy[1]);
    yb = YTRAN(xx[1], yy[1]);
    x2 = XTRAN(xx[2], yy[2]);
    y2 = YTRAN(xx[2], yy[2]);
    get_rgb(dev_.fgci, &r, &g, &b);
    Pen pen(Color(r,g,b), pnwd);
    pen.SetStartCap(LineCapRound);
    pen.SetEndCap(LineCapRound);
    pen.SetLineJoin(LineJoinRound);
    Graphics graphics(hDCOut);
    graphics.SetPageUnit(UnitPixel);
    graphics.SetClip(hclp, CombineModeReplace);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.DrawBezier(&pen, x1, y1, xa, ya, xb, yb, x2, y2);
    xpt[0] = x2;
    ypt[0] = y2;
    npt = 1;
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
d4pnwd(double pw)
{
    if (npt > 0)
        stroke(1);
    if (pw < 0)
	pw *= (-0.005);	/* fix-up negative values */
    pnwd = (float) (pw * adupi);
    pnco = dev_.fgci;
}

/*
 * d4fill - fill polygon
 */
void
d4fill(int n, float *px, float *py, int fg, int nc)
{
    float xt, yt;
    int i, c, r, g, b;
    Point pnt[256];

    if (npt > 0)
        stroke(0);
    else
        begin_page();
    set_clip(1);
    //
    // fill area
    //
    c = fg ? dev_.fgci : BGC(dev_.bgci);
    get_rgb(c, &r, &g, &b);;
    SolidBrush brush(Color(r,g,b));
    for (i = 0; i < n; i++) {
	if (nc) {
	    xt = px[i];
	    yt = py[i];
	} else {
	    otrans(px[i], py[i], &xt, &yt);
	}
	Point p(XTRAN(xt, yt), YTRAN(xt, yt));
	pnt[i] = p;
    }
    Graphics graphics(hDCOut);
    graphics.SetPageUnit(UnitPixel);
    graphics.SetClip(hclp, CombineModeReplace);
    graphics.FillPolygon(&brush, pnt, n);
}

/****************************************************************************/

static void
get_rgb(int c, int *r, int *g, int *b)
{
    if (dev_.gray) {
    	*r = *g = *b = (int) (ct_scale(c) * 255 + 0.5);
    } else {
        *r = (int) (ct_scale(ct[c].r) * 255 + 0.5);
        *g = (int) (ct_scale(ct[c].g) * 255 + 0.5);
        *b = (int) (ct_scale(ct[c].b) * 255 + 0.5);
    }
}

static COLORREF
set_color(int c)
{
    int r, g, b;

    get_rgb(c, &r, &g, &b);
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
	if (hclp)
            DeleteObject(hclp);
        hclp = CreateRectRgn(x1, y1, x2, y2);
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
    int     i, j, k, r, g, b, h;
    static int nb = 65;
    RECT   strip;

    j = BGC(dev_.bgci);
    k = BGC(dev_.fnci);
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
        r = (((ct[j].r * i + ct[k].r * (nb - i)) / nb) * 255) / ct_max;
        g = (((ct[j].g * i + ct[k].g * (nb - i)) / nb) * 255) / ct_max;
        b = (((ct[j].b * i + ct[k].b * (nb - i)) / nb) * 255) / ct_max;
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

/****************************************************************************/

static void (*win_begin_page)() = NULL;
static void (*win_end_page)() = NULL;

static void
begin_page()
{
    if (cleared)
        return;
    set_clip(0);
    if (BGC(dev_.fnci) == BGC(dev_.bgci))
        scr_clear();
    else
        fountain();
    if (dev_.border)
        box();
    if (win_begin_page)
        win_begin_page();
    cleared = cleared_yet = 1;
}

static void
end_page()
{
    if (npt)
        stroke(0);
    else if (!cleared_yet)
        begin_page();
    if (win_end_page)
        win_end_page();
    cleared = 0;
}

/* stroke - draw path connecting points
 */
static void
stroke(int contin)
{
    int r, g, b, i;

    if (npt > 0) {
	xpos = xpt[npt - 1];
	ypos = ypt[npt - 1];
    }
    if (npt > 1) {
	get_rgb(pnco, &r, &g, &b);
        Graphics graphics(hDCOut);
        graphics.SetPageUnit(UnitPixel);
        graphics.SetClip(hclp, CombineModeReplace);
        graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	if (npt == 2 && xpt[0] == xpt[1] && ypt[0] == ypt[1]) {
	    if (pnwd < 1.5) {
		SolidBrush brush(Color(r,g,b));
		graphics.FillRectangle(&brush, xpt[0], ypt[0], 1, 1);
	    } else {
		Pen pen(Color(r,g,b), pnwd);
		pen.SetLineCap(LineCapRound, LineCapRound, DashCapRound);
		pen.SetLineJoin(LineJoinRound);
		graphics.DrawRectangle(&pen, xpt[0], ypt[0], 1, 1);
	    }
	} else {
	    Pen pen(Color(r,g,b), pnwd);
	    pen.SetLineCap(LineCapRound, LineCapRound, DashCapRound);
	    pen.SetLineJoin(LineJoinRound);
	    for (i = 1; i < npt; i++) {
		graphics.DrawLine(&pen, xpt[i - 1], ypt[i - 1], xpt[i], ypt[i]);
	    }
	}
    }
    if (contin) {
        xpt[0] = xpos;
        ypt[0] = ypos;
        npt = 1;
    } else {
        npt = 0;
    }
}

/*****************************************************************************/

void
win_page(void (*bp)(), void (*ep)())
{
    win_begin_page = bp;
    win_end_page = ep;
}

void
win_DC(HDC hdc, int l, int b, int r, int t)
{
    hDCOut = hdc;

    lpSize.left = l;
    lpSize.bottom = b;
    lpSize.right = r;
    lpSize.top = t;
}

void
info_line(HDC hdc, POINTS mc, int mode)
{
    double  x, y;
    int     i, cx, cy;
    char    s[40];
    static  double eps = 1e-9;

    if (adupi <= 0)
	return;
    cx = mc.x;
    cy = lpSize.bottom - mc.y;
    x = cx / adupi;
    y = cy / adupi;
    sprintf(s, " x,y = %3.2f, %3.2f", x, y);
    for (i = strlen(s); i < 20; i++)
	s[i] = ' ';
    s[i] = '\0';
    TextOut(hdc, 0, 3, s, strlen(s));
}

int
chkdev()
{
    return (DEFAULT_DEVICE);
}

#endif /* WIN32 */
