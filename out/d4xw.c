/*
 * d4xw - subroutines for producing X window graphics
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include "copa.h"
#include "output.h"
#include "device.h"
#include "clip.h"

#define DEFAULT_DEVICE	1
#define X_DEVICE	4

#define XTRAN(x)    	((int)((x) * adupi_x))
#define YTRAN(y)    	((int)((YPAGE - (y)) * adupi_y)) 
#define BGC(c)		((c<0)?dev_.wci:c)
#define	beep()		put_ch('\a')
#define icon_width 	40
#define icon_height 	40

static void begin_page();
static void end_page();
static void d4box();
static void fountain();
static void set_color();
static void set_clip();
static int get_event();

static unsigned char icon_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00, 0x00, 0x07, 0x06,
   0x00, 0x00, 0x80, 0x01, 0x0c, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00,
   0x40, 0x00, 0x10, 0x00, 0x00, 0xc0, 0x00, 0x7c, 0x00, 0x00, 0xe0, 0x00,
   0x82, 0x01, 0x00, 0xa0, 0x00, 0x03, 0x03, 0x00, 0x80, 0x00, 0x01, 0x06,
   0x00, 0x80, 0x80, 0x00, 0x08, 0x00, 0x80, 0xc0, 0x00, 0x08, 0x00, 0x40,
   0xc0, 0x00, 0x18, 0x00, 0x40, 0x80, 0x00, 0x10, 0x60, 0x20, 0x80, 0x00,
   0x10, 0x20, 0x60, 0x80, 0x00, 0x10, 0x10, 0xc0, 0xc0, 0x00, 0x10, 0x10,
   0x80, 0x40, 0x00, 0x08, 0x08, 0x84, 0x80, 0x00, 0x08, 0x08, 0x86, 0x80,
   0x00, 0x08, 0x08, 0x82, 0x00, 0x01, 0x04, 0x08, 0x82, 0x07, 0x03, 0x02,
   0x08, 0x02, 0x04, 0x06, 0x06, 0x08, 0x02, 0x04, 0x04, 0x04, 0x08, 0x06,
   0x04, 0x02, 0x04, 0x08, 0x04, 0x06, 0x01, 0x0c, 0x10, 0x00, 0x86, 0x01,
   0x08, 0x10, 0x00, 0x98, 0x00, 0x08, 0x20, 0x40, 0x60, 0x00, 0x08, 0x40,
   0x80, 0x01, 0x00, 0x08, 0x00, 0x80, 0x06, 0x08, 0x08, 0x00, 0x80, 0x0c,
   0x06, 0x08, 0x00, 0x80, 0xb0, 0x01, 0x08, 0x00, 0x40, 0x40, 0x00, 0x08,
   0x00, 0x40, 0x00, 0x00, 0x08, 0x00, 0x40, 0x00, 0x00, 0x08, 0x00, 0x40,
   0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00 };

static float adupi_x = 0;
static float adupi_y = 0;
static int xpix = 720, ypix = 348, ncolors;
static int cleared = 0;
static int cleared_yet = 0;
static int exit_flag = 0;

static Atom kill;

//set the width and heigt to static global so the expose event can redraw window
static unsigned int width = 850;	/* width of the window */
static unsigned int height = 700;	/* height of the window */

/* device 7 - system dependent graphic device */
int     d4open();
void    d4close(), d4trns(), d4page(), d4pnwd(), d4fill();

Colormap cmap;
Display *mdsp;
Window  mwin;
GC      mgc;
//backup window pixmap (to redraw window)
Pixmap backup;

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

    int     dpth;
    int     scrn;
    unsigned long bp;
    unsigned long wmsk;
    unsigned long wp;
    Pixmap icon;
    XGCValues gcv;
    XSetWindowAttributes swa;
    XSizeHints theSizeHints;
    XWindowAttributes wa;

    int     x = 8;		/* x top left corner of window */
    int     y = 8;		/* y top left corner of window */
    int     border_width = 2;	/* border width of the window */

    mdsp = XOpenDisplay("");
    if (mdsp == NULL) {
	return(0);
    }
    scrn = DefaultScreen(mdsp); 
    dpth = DefaultDepth(mdsp, scrn); 
    wp = WhitePixel(mdsp, scrn);
    bp = BlackPixel(mdsp, scrn);
    cmap = DefaultColormap(mdsp, scrn); 
    x = XDisplayWidth(mdsp, scrn);
    y = XDisplayHeight(mdsp, scrn);
    height = y * (double) 3 / 4;
    width = height * (double) 21 / 16;
    x = (x - width) / 2;
    y = 8;
    swa.border_pixel = BlackPixel(mdsp, scrn); 
    swa.background_pixel = WhitePixel(mdsp, scrn); 
    swa.override_redirect = False;
    swa.backing_store = WhenMapped;
    wmsk = CWBackPixel | CWBorderPixel | CWBackingStore | CWOverrideRedirect; 
    mwin = XCreateWindow(mdsp,
	RootWindow(mdsp, scrn),
	x, y, width, height, border_width,
	dpth, InputOutput, CopyFromParent,
	wmsk, &swa);
    XFlush(mdsp);

    theSizeHints.flags = PPosition | PSize;	/* set mask for the hints */
    theSizeHints.x = x;		/* x position */
    theSizeHints.y = y;		/* y position */
    theSizeHints.width = width;	/* width of the window */
    theSizeHints.height = height;	/* height of the window */

    icon = XCreateBitmapFromData(mdsp, mwin, (char *) icon_bits, 
            icon_height, icon_height); 

    XSetStandardProperties(mdsp, mwin, "BTNRH PLT", "", icon, 0, 0, NULL);

    mgc = XCreateGC(mdsp, mwin, (unsigned long) 0, &gcv);

    XSelectInput(mdsp, mwin, ExposureMask | KeyPressMask | ClientMessage);

    /* error... cannot create gc */
    if (mgc == 0) {
	XDestroyWindow(mdsp, scrn);
	exit(0);
    }
    /* set forground and background defaults */
    else {
	XSetForeground(mdsp, mgc, bp);
	XSetBackground(mdsp, mgc, wp);
    }

    XMapRaised(mdsp, mwin);
    XGetWindowAttributes(mdsp, mwin, &wa);

    backup = XCreatePixmap(mdsp, RootWindow(mdsp, scrn), width, height, 
            DefaultDepth(mdsp, scrn));

    kill = XInternAtom(mdsp, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(mdsp, mwin, &kill, 1);

    xpix = wa.width;
    ypix = wa.height;
    ncolors = 256;

    adupi_x = xpix / XPAGE;
    adupi_y = ypix / YPAGE;
    dev_.adupi = adupi_x * 2;
    dev_.txpr = TXPRSTK;
    dev_.maxpen = 120;
    dev_.prompt = 1;
    dev_.border = 0;
    dev_.fillwhite = 1;
    dev_.widestroke = 1;

//    dev_.bgci = wp;
//    dev_.fgci = bp;
    cleared_yet = 0;

    return (1);
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

    XDestroyWindow(mdsp, mwin);
    XCloseDisplay(mdsp);
}

/*
 * d4trns - convert NC to PC move or draw
 *
 *      xx,yy   coordinates in inches (NC)
 *      do_gs   0 = move
 *              1 = draw
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
    if (do_gs) {
	c = dev_.fgci;
	set_color(c);
	XDrawLine(mdsp, mwin, mgc, xpos, ypos, x, y);
	XDrawLine(mdsp, backup, mgc, xpos, ypos, x, y);
    } else {
        XFlush(mdsp);
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
    unsigned int lnwd;

    if (pw < 0)
	pw *= (-0.005);	/* fix-up negative values */
    lnwd = pw * adupi_x + 0.5;	/* pen width in x-pixels */
    XSetLineAttributes(mdsp, mgc, lnwd, LineSolid, CapRound, JoinRound);
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
    float   xt, yt;
    XPoint  *pnt;

    begin_page();
    c = (fg) ? dev_.fgci : BGC(dev_.bgci);
    set_color(c);
    set_clip(1);
    pnt = (XPoint *) calloc(n, sizeof(XPoint));
    for (i = 0; i < n; i++) {
	if (nc) {
	    xt = px[i];
	    yt = py[i];
	} else {
	    otrans(px[i], py[i], &xt, &yt);
	}
	pnt[i].x = XTRAN(xt);
	pnt[i].y = YTRAN(yt);
    }
    XFillPolygon(mdsp, mwin, mgc, pnt, n, Nonconvex, CoordModeOrigin);
    XFillPolygon(mdsp, backup, mgc, pnt, n, Nonconvex, CoordModeOrigin);
    XFlush(mdsp);
    set_clip(0);
    free(pnt);
}

static void
scr_clear()
{
    set_color(BGC(dev_.bgci));
    XFillRectangle(mdsp, mwin, mgc, 0, 0, xpix, ypix);
    XFillRectangle(mdsp, backup, mgc, 0, 0, xpix, ypix);
}

static void
begin_page()
{
    if (cleared)
	return;
    usleep(250 * 1000);
    if (ncolors <= 2 || BGC(dev_.fnci) == BGC(dev_.bgci))
	scr_clear();
    else
	fountain();
    if (dev_.border)
	d4box();
    cleared_yet = cleared = 1;
}

static void
end_page()
{
    if (!cleared)
	return;
    XFlush(mdsp);
    if (dev_.prompt) {
	beep();
        get_event();
    }
    cleared = 0;
}

/* d4box - draw box
 */
static void
d4box()
{
    float   x = XPAGE, y = YPAGE, z = 0.0;

    d4trns(z, y, 0);
    d4trns(x, y, 1);
    d4trns(x, z, 1);
    d4trns(z, z, 1);
    d4trns(z, y, 1);
    fflush(fpout);
}

static void
set_clip(int on)
{
    XRectangle r[1];

    if (on) {
        r[0].x = cclip_.tcx1 * adupi_x;
        r[0].y = cclip_.tcy1 * adupi_y;
        r[0].width = (cclip_.tcx2 - cclip_.tcx1) * adupi_x + 0.5;
        r[0].height = (cclip_.tcy2 - cclip_.tcy1) * adupi_y + 0.5;
    } else {
        r[0].x = 0;
        r[0].y = 0;
        r[0].width = xpix;
        r[0].height = ypix;
    }
    XSetClipRectangles(mdsp, mgc, 0, 0, r, 1, Unsorted);
}

static void
set_rgb_color(int r, int g, int b)
{
    XColor  hw;

    hw.red = r * 65;
    hw.green = g * 65;
    hw.blue = b * 65;
    hw.pixel = 0x20;
    hw.flags = -1;
    XAllocColor(mdsp, cmap, &hw);
    XSetForeground(mdsp, mgc, hw.pixel);
}

static void
set_color(int c)
{
    if (dev_.gray)
	set_rgb_color(c, c, c);
    else
	set_rgb_color(ct[c].r, ct[c].g, ct[c].b);
}

static void
fountain()
{
    int     i, j, k, y1, y2;
    int     r, g, b;
    static int nb = 65;

    y2 = 0;
    j = BGC(dev_.bgci);
    k = BGC(dev_.fnci);
    for (i = 0; i < nb; i++) {
	y1 = y2;
	y2 = ((i + 1) * ypix) / nb;
        r = (ct[j].r * i + ct[k].r * (nb - i)) / nb;
        g = (ct[j].g * i + ct[k].g * (nb - i)) / nb;
        b = (ct[j].b * i + ct[k].b * (nb - i)) / nb;
	set_rgb_color(r, g, b);
	XFillRectangle(mdsp, mwin, mgc, 0, y1, xpix, y2 - y1);
	XFillRectangle(mdsp, backup, mgc, 0, y1, xpix, y2 - y1);
    }
    set_rgb_color(ct[j].r, ct[j].g, ct[j].b);
    XFillRectangle(mdsp, mwin, mgc, 0, y2, xpix, ypix - y2);
    XFillRectangle(mdsp, backup, mgc, 0, y2, xpix, ypix - y2);
    XFlush(mdsp);
}

/*this fucntion enters a simple event handler which redraws the window 
and exits when it recieves a keyboard event  */

static int
get_event()
{
    static XEvent report;

    while (1)   //start an infinite look to look for a keyed event
    {
        XNextEvent(mdsp, &report);
        switch (report.type)
        {
            case Expose: //expose stuff here...
                XCopyArea(mdsp, backup, mwin, mgc, 0,0,width,height,0,0);
                break;
            case NoExpose: //no expose stuff here...
                break;
            case KeyPress:
                return 1;
                break;
            case ClientMessage:
		exit_flag++;
                return 1;
                break;
            default: //more stuff and things...
                break;
        }
    }
}

int
chkdev()
{
    if (getenv("DISPLAY"))
    	return (X_DEVICE);
    return (DEFAULT_DEVICE);
}
