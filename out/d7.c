/*
 * d7 - Enhanced Metafile device
 */

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "clip.h"
#include "copa.h"
#include "output.h"
#include "device.h"
#include "emf.h"

#define	NPTS            1024
#define VDC(x)		((int)((x)*adupi))
#define XTRAN(x)	((int)((x)*adupi))
#define YTRAN(y)	((int)((YPAGE-(y))*adupi))
#define nint(x)         ((int)floor((x)+0.5))

int     d7open(void);
void    d7close(void), d7page(void);
void    d7trns(double, double, int);
void    d7pnwd(double);
void    d7fill(int, float *, float *, int, int);
void    d7curv(float *, float *);

static void stroke(int);
static int  set_color(int);
static void begin_page(void);
static void end_page(void);
static void emr_write(int, int);
static void emr_bezier(int *, int *, int);
static void emr_fillrgn(int *, int *, int);
static void emr_polyline(int *, int *, int);
static void set_brush(int);
static void set_clip(int);
static void set_pen(int, int);

static double tx0 = 0;
static double ty0 = 0;
static double txx = 0;
static double tyx = 0;
static double txy = 0;
static double tyy = 0;
static float adupi = 1200;
static int *xpt = NULL;
static int *ypt = NULL;
static int npt = 0;
static int cleared = 0;
static int pnco = 0;
static int pnwd = 0;
static int rcnt = 0;
static int rsiz = 0;
static int xpix = 0;
static int ypix = 0;
static int cx1 = 0;
static int cy1 = 0;
static int cx2 = 0;
static int cy2 = 0;
static int havpen = 0;
static int havbrs = 0;
static int havbnd = 0;
static int lastpc = 0;
static int lastpw = 0;
static int lastbc = 0;
static short desc[18] = {
    'B', 'T', 'N', 'R', 'H', ' ', 'P', 'L', 'T',
};
static BB	    b = {0};
static EMFRECORD    r;
static EMFHEADER    h;
static RECT 	    Scr;

/******************************************************************/

/*
 * d7bind - overide default device function
 */
void
d7bind()
{
    d_open = d7open;
    d_close = d7close;
    d_trns = d7trns;
    d_page = d7page;
    d_pnwd = d7pnwd;
    d_fill = d7fill;
    d_curv = d7curv;
}

/*
 * d7open - initialize plotting device
 */
int
d7open()
{
    h.RecordType = EMR_HEADER;
    h.Size = sizeof(h) + sizeof(desc);
    h.BoundsLeft = 0;			    // update this on close
    h.BoundsTop = 0;			    // update this on close
    h.BoundsRight = 0;			    // update this on close
    h.BoundsBottom = 0;			    // update this on close
    h.FrameLeft = 0;			    // 0.01 mm units
    h.FrameTop = 0;			    // 0.01 mm units
    h.FrameRight = (int) (XPAGE * 2540);    // 0.01 mm units
    h.FrameBottom = (int) (YPAGE * 2540);   // 0.01 mm units
    h.Signature = 0x464D4520;
    h.Version[0] = 0;
    h.Version[1] = 1;
    h.FileSize = h.Size;		    // update this on close
    h.NumOfRecords = 1;			    // update this on close
    h.NumOfHandles = 8;
    h.SizeOfDescrip = sizeof(desc) / 2;
    h.OffsOfDescrip = sizeof(h);
    h.NumPalEntries = 0;
    h.WidthDevPixels = (int) (XPAGE * adupi);
    h.HeightDevPixels = (int) (YPAGE * adupi);
    h.WidthDevMM = (int) (XPAGE * 25.4);
    h.HeightDevMM = (int) (YPAGE * 25.4);
    fwrite(&h, sizeof(h), 1, fpout);
    fwrite(desc, sizeof(desc), 1, fpout);
    rsiz = sizeof(h)+sizeof(desc);
    rcnt = 2;

    tx0 = 0;
    ty0 = adupi * YPAGE;
    txx = adupi;
    tyx = 0;
    txy = 0;
    tyy = -adupi;
    dev_.adupi = adupi;		/* addressable units per inch */
    dev_.txpr = TXPRSTK;	/* text precision = stroke */
    dev_.prompt = 0;		/* prompt user at end of page */
    dev_.border = 0;		/* draw page border */
    dev_.maxpen = 0;		/* maximum pen value */
    dev_.fillwhite = 1;		/* possible to write white */
    xpix = (int) (XPAGE * adupi);
    ypix = (int) (YPAGE * adupi);
    Scr.left = 0;
    Scr.top = 0;
    Scr.right = xpix;
    Scr.bottom = ypix;

    xpt = (int *) calloc(NPTS * 2, sizeof(int));
    if (xpt == NULL)
	return (0);
    ypt = xpt + NPTS;
    npt = 0;
    cleared = 0;
    cx1 = cy1 = cx2 = cy2 = 0;
    havpen = havbrs = havbnd = 0;
    lastpc = lastbc = lastpw = -1;

    return (1);	/* 0 = unsuccessful open, 1 = successful open */
}

/*
 * d7close - close plotting device
 */
void
d7close()
{
    double sf;
    static int32_t mg = 100; // 1-mm margin

    end_page();

    // write EOF record
    r.P[0] = 0;		// Number of color palette entries
    r.P[1] = 4 * 4;	// Offset of color palette entries
    r.P[2] = 5 * 4;	// Offset to beginning of this record
    emr_write(EMR_EOF, 3);

    // rewrite header record
    rewind(fpout);
    if (havbnd) {
        h.BoundsLeft = b.l;
	h.BoundsTop = b.t;
	h.BoundsRight = b.r;
	h.BoundsBottom = b.b;
	// Frame = Bounds with margin (0.01 mm units)
	sf = 100 * h.WidthDevMM / (double) h.WidthDevPixels;
        h.FrameLeft = nint(b.l * sf) - mg;
	h.FrameTop = nint(b.t * sf) - mg;
	h.FrameRight = nint(b.r * sf) + mg;
	h.FrameBottom = nint(b.b * sf) + mg;
    }
    h.FileSize = rsiz;
    h.NumOfRecords = rcnt;
    fwrite(&h, sizeof(h), 1, fpout);

    if (xpt) {
        free(xpt);
	xpt = NULL;
    }
}

/*
 * d7trns - convert NC to device units and move or draw
 *
 *	xx,yy	coordinates in inches (NC) already clipped to page boundary:
 *		0 <= xx <= XPAGE  and  0 <= yy <= YPAGE
 *	do_gs	move/draw flag: 0 = move, 1 = draw
 */
void
d7trns(double xx, double yy, int do_gs)
{
    int     x, y;

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
d7curv(float *xx, float *yy)
{
    int	    x[4], y[4];

    if (npt > 0)
        stroke(1);
    else
        begin_page();
    set_clip(1);
    set_pen(pnwd, pnco);
    x[0] = xpt[0];
    y[0] = ypt[0];
    x[1] = XTRAN(xx[0]);
    y[1] = YTRAN(yy[0]);
    x[2] = XTRAN(xx[1]);
    y[2] = YTRAN(yy[1]);
    x[3] = XTRAN(xx[2]);
    y[3] = YTRAN(yy[2]);
    emr_bezier(x, y, 4);
    xpt[0] = x[3];
    ypt[0] = y[3];
    npt = 1;
}

/*
 * d7page - signal new page (media change)
 */
void
d7page()
{
    end_page();
}

/*
 * d7pnwd -
 */
void
d7pnwd(pw)
double  pw;			/* pen width in inches */
{
    if (npt > 0)
        stroke(0);
    if (pw < 0)
	pw *= (-1.0 / 200);	/* fix-up negative values */
    pnwd = VDC(pw);
    pnco = set_color(dev_.fgci);
}

void
d7fill(n, px, py, fg, nc)
int     n, fg, nc;
float  *px, *py;
{
    int     i, ci, c, *xp, *yp;
    float   xt, yt;

    if (npt > 0)
        stroke(0);
    else
        begin_page();
    set_clip(1);
    ci = (fg) ? dev_.fgci : dev_.bgci;
    if (ci < 0)
	return;
    c = set_color(ci);
    set_pen(1, c);
    set_brush(c);
    xp = (int *) calloc(n * 2, sizeof(int));
    yp = xp + n;
    for (i = 0; i < n; i++) {
	if (nc) {
	    xt = px[i];
	    yt = py[i];
	} else {
	    otrans(px[i], py[i], &xt, &yt);
	}
	xp[i] = XTRAN(xt);
	yp[i] = YTRAN(yt);
    }
    emr_fillrgn(xp, yp, n);
    free(xp);
}

/******************************************************************/

static BB
bbox(int *xp, int *yp, int np)
{
    int i, p;
    BB a;

    p = pnwd;		// margin equals pen width
    a.l = a.r = xp[0];;
    a.t = a.b = yp[0];;
    for (i = 0; i < np; i++) {
	if (a.l > xp[i] - p)
	    a.l = xp[i] - p;
	if (a.t > yp[i] - p)
	    a.t = yp[i] - p;
	if (a.r < xp[i] + p)
	    a.r = xp[i] + p;
	if (a.b < yp[i] + p)
	    a.b = yp[i] + p;
    }
    if (!havbnd) {
	b.l = a.l;
	b.t = a.t;
	b.r = a.r;
	b.b = a.b;
	havbnd = 1;
    } else {
	if (b.l > a.l)
	    b.l = a.l;
	if (b.t > a.t)
	    b.t = a.t;
	if (b.r < a.r)
	    b.r = a.r;
	if (b.b < a.b)
	    b.b = a.b;
    }

    return (a);
}

/******************************************************************/

static void
emr_write(int f, int n)
{
    r.Function = f;
    r.Size = 4 * (2 + n);
    fwrite(&r, 4, 2 + n, fpout);
    rsiz += r.Size;
    rcnt += 1;
}

static void
emr_polyline(int *xp, int *yp, int np)
{
    int i, f;
    BB a;

    emr_write(EMR_BEGINPATH, 0);
    for (i = 0; i < np; i++) {
	f = i ? EMR_LINETO : EMR_MOVETOEX;
	r.P[0] = xp[i];
	r.P[1] = yp[i];
        emr_write(f, 2);
    }
    emr_write(EMR_ENDPATH, 0);
    a = bbox(xp, yp, np);
    r.P[0] = a.l;
    r.P[1] = a.t;
    r.P[2] = a.r;
    r.P[3] = a.b;
    emr_write(EMR_STROKEPATH, 4);
}

static void
emr_bezier(int *xp, int *yp, int np)
{
    int i;
    BB a;

    a = bbox(xp, yp, np);
    r.P[0] = a.l;
    r.P[1] = a.t;
    r.P[2] = a.r;
    r.P[3] = a.b;
    r.P[4] = np;
    for (i = 0; i < np; i++) {
	r.P[i + 5] = ((yp[i] & 0xFFFF) << 16) | xp[i];
    }
    emr_write(EMR_POLYBEZIER16, 5 + np);
}

static void
emr_fillrgn(int *xp, int *yp, int np)
{
    int i, f;
    BB a;

    r.P[0] = 1;
    emr_write(EMR_SETPOLYFILLMODE, 1);
    emr_write(EMR_BEGINPATH, 0);
    for (i = 0; i < np; i++) {
	f = i ? EMR_LINETO : EMR_MOVETOEX;
	r.P[0] = xp[i];
	r.P[1] = yp[i];
        emr_write(f, 2);
    }
    emr_write(EMR_CLOSEFIGURE, 0);
    emr_write(EMR_ENDPATH, 0);
    a = bbox(xp, yp, np);
    r.P[0] = a.l;
    r.P[1] = a.t;
    r.P[2] = a.r;
    r.P[3] = a.b;
    emr_write(EMR_FILLPATH, 4);
}

/******************************************************************/

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
set_pen(int w, int c)
{
    if (c != lastpc  || w != lastpw) {
	if (havpen) {
	    r.P[0] = 1;		// handle
	    emr_write(EMR_DELETEOBJECT, 1);
	    havpen = 0;
        }
        if (c >= 0) {
	    r.P[0] = 1;		// handle
	    r.P[1] = 56;	// ???
	    r.P[2] = 0;		// ???
	    r.P[3] = 56;	// ???
	    r.P[4] = 0;		// ???
	    r.P[5] = 1 << 16;   // ???
	    r.P[6] = w;		// width
	    r.P[7] = 0;		// ???
	    r.P[8] = c;		// color
	    r.P[9] = 0;		// ???
	    r.P[10] = 0;	// ???
	    r.P[11] = 0;	// ???
	    emr_write(EMR_EXTCREATEPEN, 12);
	    r.P[0] = 1;		// handle
	    emr_write(EMR_SELECTOBJECT, 1);
	    havpen = 1;
	    lastpc = c;
	    lastpw = w;
	}
    }
}

static void
set_brush(int c)
{
    if (c != lastbc) {
	if (havbrs) {
	    r.P[0] = 2;		// handle
	    emr_write(EMR_DELETEOBJECT, 1);
	    havbrs = 0;
	}
	if (c >= 0) {
	    r.P[0] = 2;		// handle
	    r.P[1] = 0;		// solid
	    r.P[2] = c;		// color
	    r.P[3] = 0;		// ???
	    emr_write(EMR_CREATEBRUSH, 4);
	    r.P[0] = 2;		// handle
	    emr_write(EMR_SELECTOBJECT, 1);
	    havbrs = 1;
	    lastbc = c;
	}
    }
}

static void
set_clip(int clip)
{
    int x1, y1, x2, y2, zz;

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
	x2 = xpix;
	y2 = ypix;
    }
    if (x1 != cx1 || y1 != cy1 || x2 != cx2 || y2 != cy2) {
        r.P[0] = 48;	// ??
	r.P[1] = 5;	// ??
	r.P[2] = 32;	// ??
	r.P[3] = 1;	// ??
	r.P[4] = 1;	// ??
	r.P[5] = 16;
        r.P[6] = x1;
	r.P[7] = y1;
	r.P[8] = x2;
	r.P[9] = y2;
	r.P[10] = x1;
	r.P[11] = y1;
	r.P[12] = x2;
	r.P[13] = y2;
	emr_write(EMR_EXTSELECTCLIPRGN, 14);
	cx1 = x1;
	cy1 = y1;
	cx2 = x2;
	cy2 = y2;
    }
}

static void
rectangle(RECT *R, int c)
{
    set_pen(1, c);
    set_brush(c);
    r.P[0] = R->left;
    r.P[1] = R->top;
    r.P[2] = R->right;
    r.P[3] = R->bottom;
    emr_write(EMR_RECTANGLE, 4);
}

static void
scr_clear()
{
    if (dev_.bgci >= 0)
	rectangle(&Scr, set_color(dev_.bgci));
}

static void
fountain()
{
    int     i, j, k, r, g, b, h;
    RECT   strip;
    static int nb = 65;

    j = dev_.bgci;
    k = dev_.fnci;
    h = (int) (Scr.bottom - Scr.top);
    strip.right = Scr.right;
    strip.left = Scr.left;
    strip.top = Scr.top;
    strip.bottom = Scr.bottom;
    for (i = 0; i < nb; i++) {
	strip.bottom = Scr.top + ((i + 1) * h) / nb;
        r = (((ct[j].r * i + ct[k].r * (nb - i)) / nb) * 255) / ct_max;
        g = (((ct[j].g * i + ct[k].g * (nb - i)) / nb) * 255) / ct_max;
        b = (((ct[j].b * i + ct[k].b * (nb - i)) / nb) * 255) / ct_max;
        rectangle(&strip, RGB(r,g,b));
	strip.top = strip.bottom;
    }
    strip.bottom = Scr.bottom;
    r = (ct[j].r * 255) / ct_max;
    g = (ct[j].g * 255) / ct_max;
    b = (ct[j].b * 255) / ct_max;
    rectangle(&strip, RGB(r,g,b));
}

/****************************************************************************/

static void
begin_page()
{
    if (cleared)
        return;
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
end_page()
{
    if (npt)
        stroke(0);
    cleared = 0;
}

static void
stroke(int contin)
{
    if (npt > 1) {
	if (npt == 2 && xpt[0] == xpt[1] && ypt[0] == ypt[1]) {
	    // set single points ???
	}
        set_pen(pnwd, pnco);
	emr_polyline(xpt, ypt, npt);
    }
    if (contin) {
        xpt[0] = xpt[npt - 1];
        ypt[0] = ypt[npt - 1];
        npt = 1;
    } else {
        npt = 0;
    }
}
