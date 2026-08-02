/*
 * d4mac - Apple OSX & iOS
 */

#ifdef __OBJC__
#include <UIKit/UIKit.h>    // iPhone
#else
#include <CoreGraphics/CoreGraphics.h>  // Mac Carbon
#endif
#include <stdio.h>
#include "../out/output.h"
#include "../out/device.h"
#include "../out/copa.h"
#include "../out/clip.h"

#define DEFAULT_DEVICE  4
#define XTRAN(x,y)          (txx * x + txy * y + tx0)
#define YTRAN(x,y)          (tyx * x + tyy * y + ty0)
#define MAXPATH         2048
#define MAXINF          80
#define STRIPS          250.0

static double xpix = 0;
static double ypix = 0;
static double tx0 = 0;
static double ty0 = 0;
static double txx = 0;
static double tyx = 0;
static double txy = 0;
static double tyy = 0;
static float dpi = 100.0;
static float cx1, cy1, cx2, cy2;
static float xpos = 0;
static float ypos = 0;
static float pen = 0;
static float lastpen  = 0;
static int npnt = 0;
static int newpage = 0;
static int pagecount = 0;
static int lastcolor = 0;
static void (*mac_beg_page)(int) = NULL;
static void (*mac_end_page)(int, int) = NULL;
static CGContextRef	ctx = NULL;
static CGColorSpaceRef  csp = NULL;

/* device 4 - system dependent graphic device */
static int  d4open(void);
static void d4close(void), d4page(void);
static void d4trns(double, double, int);
static void d4pnwd(double);
static void d4fill(int, float *, float *, int, int);
static void d4curv(float *, float *);
static void beg_page(void), end_page(int);
static void stroke_path(void), background(void);
static void setrgb(int), setpen(float), setclip(void);

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
    d_curv = d4curv;
}

/*
 * d4open - initialize plotting device
 */
int
d4open()
{
    dev_.adupi = dpi;           /* addressable units per inch */
    dev_.maxpen = 1;		/* maximum pen width (inch) */
    dev_.txpr = TXPRSTK;        /* text precision = stroke */
    dev_.prompt = 0;            /* prompt user at end of page */
    dev_.border = 0;            /* draw page border */
    dev_.fillwhite = 1;         /* possible to write white */

    npnt = 0;
    pagecount = 0;
    newpage = 1;
    lastpen = -1;
    lastcolor = -1;

    return (1);                 /* 0 = unsuccessful open, 1 = successful open */
}

/*
 * d4close - close plotting device
 */
void
d4close(void)
{
    end_page(1);
}

/*
 * d4trns - convert NC to device units and move or draw
 *
 *      xx,yy   coordinates in inches (NC) already clipped to page boundary:
 *              0 <= xx <= XPAGE  and  0 <= yy <= YPAGE
 *      do_gs   move/draw flag: 0 = move, 1 = draw
 */
void
d4trns(xx, yy, do_gs)
double  xx, yy;
int     do_gs;
{
    float ix, iy;

    if (newpage) {
	beg_page();
    }
    if (npnt >= MAXPATH || do_gs == 0) {
	stroke_path();
    }
    ix = XTRAN(xx, yy);
    iy = YTRAN(xx, yy);
    if (do_gs) {
	setrgb(dev_.fgci);
	setpen(pen);
	if (npnt == 0) {
	    CGContextBeginPath(ctx);
	    CGContextMoveToPoint(ctx, xpos, ypos);
	}
        CGContextAddLineToPoint(ctx, ix, iy);
	npnt++;
    }
    xpos = ix;
    ypos = iy;
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
    float  cx1, cy1, cx2, cy2, ix, iy;

    if (newpage) {
	beg_page();
    }
    if (npnt >= MAXPATH - 4) {
	stroke_path();
    }
    setrgb(dev_.fgci);
    setpen(pen);
    if (npnt == 0) {
	CGContextBeginPath(ctx);
	CGContextMoveToPoint(ctx, xpos, ypos);
    }
    cx1 = XTRAN(xx[0], yy[0]);
    cy1 = YTRAN(xx[0], yy[0]);
    cx2 = XTRAN(xx[1], yy[1]);
    cy2 = YTRAN(xx[1], yy[1]);
    ix = XTRAN(xx[2], yy[2]);
    iy = YTRAN(xx[2], yy[2]);
    CGContextAddCurveToPoint(ctx, cx1, cy1, cx2, cy2, ix, iy);
    npnt += 3;

    xpos = ix;
    ypos = iy;
}

/*
 * d4page - signal new page (media change)
 */
void
d4page()
{
    end_page(0);
}

/*
 * d4pnwd -
 */
void
d4pnwd(pw)
double  pw;			/* pen width in inches */
{
    stroke_path();
    if (pw < 0) {
	pw *= (-0.005);	/* fix-up negative values */
    }
    pen = limit(0.005 , pw, dev_.maxpen) * dpi;	/* force pen value into range */
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
    int     i, c;
    float   xt, yt, ix, iy;

    if (newpage) {
	beg_page();
    } else {
	stroke_path();
    }
    c = (fg) ? dev_.fgci : dev_.bgci;
    if (c < 0) {
	return;
    }
//    CGContextSaveGState(ctx);
    setrgb(c);
    setclip();
    for (i = 0; i < n; i++) {
	if (nc) {
	    xt = px[i];
	    yt = py[i];
	} else {
	    otrans(px[i], py[i], &xt, &yt);
	}
	ix = XTRAN(xt, yt);
	iy = YTRAN(xt, yt);
	if (i == 0) {
	    CGContextBeginPath(ctx);
	    CGContextMoveToPoint(ctx, ix, iy);
	} else {
	    CGContextAddLineToPoint(ctx, ix, iy);
	}
    }
    CGContextClosePath(ctx);
    CGContextFillPath(ctx);
//    CGContextRestoreGState(ctx);
}


/**********************************************************************************/

static void
stroke_path()
{
    if (npnt > 0) {
        CGContextStrokePath(ctx);
	npnt = 0;
    }		
}

static void
setrgb(int c)
{
    CGFloat rgb[4];

    if (!csp) {
	return;
    }
    if (c != lastcolor) {	/* set rgb color */
	stroke_path();		/* flush any remaining path */
	if (dev_.gray) {
	    rgb[0] = ct_scale(c);
	    rgb[1] = ct_scale(c);
	    rgb[2] = ct_scale(c);
	    rgb[3] = 1; // alpha
	    lastcolor = -c;
	} else {
	    rgb[0] = ct_scale(ct[c].r);
	    rgb[1] = ct_scale(ct[c].g);
	    rgb[2] = ct_scale(ct[c].b);
	    rgb[3] = 1; // alpha
	    lastcolor = c;
	}
	CGContextSetFillColor(ctx, rgb);
	CGContextSetStrokeColor(ctx, rgb);
    }
}

static void
setpen(float pen)
{
    if (pen != lastpen) {
	stroke_path();		/* flush any remaining path */
	CGContextSetLineWidth(ctx, pen);
        lastpen = pen;
    }
}

static void
background(void)
{
    float   x1, y1, x2, y2;
    int     c;
    CGFloat rgb[4];
    CGRect  r;

    x1 = XTRAN(0,0);
    y1 = YTRAN(0,0);
    x2 = XTRAN(XPAGE,YPAGE);
    y2 = YTRAN(XPAGE,YPAGE);

    if (dev_.fnci <0 || dev_.bgci < 0)
        return;
    r.origin.x = x1;
    r.origin.y = y1;
    r.size.width = x2 - x1 + 1;
    r.size.height = y2 - y1 + 1;
    if (dev_.fnci == dev_.bgci) {
	c = dev_.bgci;
	rgb[0] = ct_scale(ct[c].r);
	rgb[1] = ct_scale(ct[c].g);
	rgb[2] = ct_scale(ct[c].b);
	rgb[3] = 1; // alpha
	CGContextBeginPath(ctx);
	CGContextSetFillColor(ctx, rgb);
	CGContextAddRect(ctx, r);
	CGContextFillPath(ctx);
    } else if (csp) {
        CGPoint			p1, p2;
        CGGradientRef		grd;
        CGGradientDrawingOptions    opt;
        CGFloat			rgb[8], loc[2];
        p1.x = XTRAN(0,0);
        p1.y = YTRAN(0,0);
        p2.x = XTRAN(0,YPAGE);
        p2.y = YTRAN(0,YPAGE);
        c = dev_.bgci;
        rgb[0] = ct_scale(ct[c].r);
        rgb[1] = ct_scale(ct[c].g);
        rgb[2] = ct_scale(ct[c].b);
        rgb[3] = 1; // alpha
        c = dev_.fnci;
        rgb[4] = ct_scale(ct[c].r);
        rgb[5] = ct_scale(ct[c].g);
        rgb[6] = ct_scale(ct[c].b);
        rgb[7] = 1; // alpha
        loc[0] = 0;
        loc[1] = 1;
        grd = CGGradientCreateWithColorComponents(csp, rgb, loc, 2);
        opt = kCGGradientDrawsBeforeStartLocation | kCGGradientDrawsAfterEndLocation;
        CGContextBeginPath(ctx);
        CGContextAddRect(ctx, r);
        CGContextDrawLinearGradient(ctx, grd, p1, p2, opt);
        CGGradientRelease(grd);
    } else {	    // use strips if no gradient
        float   r1, g1, b1, r2, g2, b2, p1, p2;
        c = dev_.bgci;
        r1 = ct_scale(ct[c].r);
        g1 = ct_scale(ct[c].g);
        b1 = ct_scale(ct[c].b);
        c = dev_.fnci;
        r2 = ct_scale(ct[c].r);
        g2 = ct_scale(ct[c].g);
        b2 = ct_scale(ct[c].b);
        if (txx) {
            r.size.height = y2 / STRIPS + 1;
        } else {
            r.size.width = x2 / STRIPS + 1;
        }
        p2 = 1 / STRIPS;
        while (p2 <= 1) {
            p1 = 1 - p2;
            rgb[0] = r1 * p1 + r2 * p2;
            rgb[1] = g1 * p1 + g2 * p2;
            rgb[2] = b1 * p1 + b2 * p2;
            rgb[3] = 1; // alpha
            CGContextBeginPath(ctx);
            CGContextSetFillColor(ctx, rgb);
            CGContextAddRect(ctx, r);
            CGContextFillPath(ctx);
            p2 += 1 / STRIPS;
            if (txx) {
                r.origin.y += r.size.height - 1;
            } else {
                r.origin.x += r.size.width - 1;
            }
        }
    }
    lastcolor = dev_.bgci;
}

static void
setclip(void)
{
    float   ix1, iy1, ix2, iy2;

    ix1 = XTRAN(cclip_.tcx1 , cclip_.tcy1);
    iy1 = YTRAN(cclip_.tcx1 , cclip_.tcy1);
    ix2 = XTRAN(cclip_.tcx2 , cclip_.tcy2);
    iy2 = YTRAN(cclip_.tcx2 , cclip_.tcy2);
    if (cx1 != ix1 || cy1 != iy1 
	|| cx2 != ix2 || cy2 != iy2) {
	CGContextBeginPath(ctx);
	CGContextMoveToPoint(ctx, ix1, iy1);
	CGContextAddLineToPoint(ctx, ix2, iy1);
	CGContextAddLineToPoint(ctx, ix2, iy2);
	CGContextAddLineToPoint(ctx, ix1, iy2);
	CGContextAddLineToPoint(ctx, ix1, iy1);
	CGContextClosePath(ctx);
	CGContextClip(ctx);
	cx1 = ix1;
	cy1 = iy1;
	cx2 = ix2;
	cy2 = iy2;
    }
}

/**********************************************************************************/

static void
beg_page()
{
    pagecount++;
    if (mac_beg_page) {
	mac_beg_page(pagecount);
	dev_.adupi = dpi;         /* addressable units per inch */
    }
    setclip();
    background();
    lastpen = -1;
    lastcolor = -1;
    xpos = -1;
    ypos = -1;
    npnt = 0;
    newpage = 0;
}

static void
end_page(int close)
{
    stroke_path();		/* flush any remaining path */
    if (mac_end_page) {
	mac_end_page(newpage, pagecount);
    }
    newpage = 1;
}

/**********************************************************************************/

int
chkdev()
{
    return (DEFAULT_DEVICE);
}

/****************************************************************************/

void
mac_context(CGContextRef *c, CGRect *b, CGColorSpaceRef s)
{
    // graphics context
    ctx = *c;
    CGContextSetLineCap(ctx, kCGLineCapRound);
    CGContextSetLineJoin(ctx, kCGLineJoinRound);
    // size
    xpix = b->size.width;
    ypix = b->size.height;
    // view bounds
    if (xpix > ypix) {
        if ((xpix / XPAGE) < (ypix / YPAGE)) {
            dpi = xpix / XPAGE;
        } else {
            dpi = ypix / YPAGE;
        }
	txx = tyy = dpi;
	txy = tyx = 0;
	tx0 = b->origin.x;
	ty0 = b->origin.y;
    } else {
        if ((xpix / YPAGE) < (ypix / XPAGE)) {
            dpi = xpix / YPAGE;
        } else {
            dpi = ypix / XPAGE;
        }
        txy = dpi;
        tyx = -dpi;
	txx = tyy = 0;
	tx0 = b->origin.x;
	ty0 = b->origin.y + ypix;
    }
    // color space
    csp = s;
    if (csp) {
	CGContextSetFillColorSpace(ctx, csp);
	CGContextSetStrokeColorSpace(ctx, csp);
    }
}

void
mac_page(void (*bp)(int), void (*ep)(int, int))
{
    mac_beg_page = bp;
    mac_end_page = ep;
}

/****************************************************************************/
    
void
info_line(CGContextRef c, float mx, float my, float vx, float vy, int m)
{
    char s[MAXINF];
    double x, y, w;
    float sx, sy, ix, iy;
    int n;
    static char *r[3] = {"page", "frame", "data"};
    static int lnc = 0;

    sx = vx / 140;
    sy = vx / 84;
    ix = sx;
    iy = vy - sy;
    x = mx / dpi;
    y = my / dpi;
    m = inq_frame(&x, &y, m);
    sprintf(s, "%s(x,y) = %.3g,%.3g    ", r[m], x, y);
    n = (int)strlen(s);
    w = fmax(n * sx, lnc * sx);
    setrgb(dev_.bgci);
    CGContextSetLineWidth(c, 0);
    CGContextAddRect(c, CGRectMake(0, vy, ix + w, -1.5 * sy));
    CGContextFillPath(c);
    setrgb(0);
    CGContextSelectFont(c, "Courier", sy, kCGEncodingMacRoman);
    CGContextSetTextDrawingMode(c, kCGTextFill);
    CGContextShowTextAtPoint(c, ix, iy, s, n);
    lnc = n;
}
