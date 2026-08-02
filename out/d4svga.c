/*
 * d4svga - subroutines for producing SVGALIB graphics
 */

#include <stdlib.h>
#include <stdio.h>
#include <vga.h>
#include <math.h>
#include "copa.h"
#include "output.h"
#include "device.h"
#include "clip.h"

#define XTRAN(x)    ((int)((x) * adupi_x))
#define YTRAN(y)    ((int)((YPAGE - (y)) * adupi_y))
#define BGC(c)	    ((c<0)?dev_.wci:c)

#define	beep()	put_ch('\a')
#define DEFAULT_DEVICE	1
#define SVGA_DEVICE	    4
#define X_DEVICE	    7

static void begin_page();
static void end_page();
static void d4box();
static void fountain();

static float adupi_x = 0;
static float adupi_y = 0;
static float pnwd = 1.0;
static int xpix = 720, ypix = 348, ncolors;
static int cleared = 0;
static int stroke = 0;
static unsigned char bgsl[1024];

/* device 4 - system dependent graphic device */
int     d4open();
void    d4close(), d4trns(), d4pnwd(), d4page();

/*
 * d4bind - override default device function
 */
void
d4bind()
{
    d_open = d4open;
    d_close = d4close;
    d_trns = d4trns;
    d_pnwd = d4pnwd;
    d_page = d4page;
}

/*
 * d4open - initialize plotting device
 */
int
d4open()
{
    int mode;

    vga_init();
    mode = vga_getdefaultmode();
    if (mode == -1)
	mode = G640x480x16;	/* Default mode. */
    if (!vga_hasmode(mode))
	return(0);
    vga_setmode(mode);
    xpix = vga_getxdim();
    ypix = vga_getydim();
    ncolors = vga_getcolors();

    adupi_x = ((xpix - 1) / XPAGE);
    adupi_y = ((ypix - 1) / YPAGE);
    dev_.adupi = adupi_x * 2;
    dev_.txpr = TXPRSTK;
    dev_.maxpen = 120;
    dev_.prompt = 1;
    dev_.border = 0;
    dev_.fillwhite = 1;
    dev_.widestroke = 1;
    return (1);
}

/*
 * d4close - close plotting device
 */
void
d4close()
{
    end_page();
    vga_setmode(TEXT);
}

/*
 * d4trns - convert NC to PC move or draw
 *
 *	xx,yy	coordinates in inches (NC)
 *	do_gs	0 = move
 *		1 = draw
 */
void
d4trns(xx, yy, do_gs)
double  xx, yy;
int     do_gs;
{
    double  dx, dy, ms, mg, vx, vy, hw;
    int     sx, sy, tx, ty, cp, wx, wy;
    int     x, y, c, x1, y1, x2, y2;
    static int xpos = 0, ypos = 0;

    if (!cleared)
	begin_page();
    x = XTRAN(xx);
    y = YTRAN(yy);
    dx = x - xpos;
    dy = y - ypos;
    ms = dx * dx + dy * dy;
    if (do_gs) {
	c = dev_.fgci;
	vga_setegacolor(c);
	if (stroke == 0) {
	    vga_drawline(xpos, ypos, x, y);
	} else if (ms > 0) {
	    mg = sqrt(ms);
	    hw = pnwd / 2 / adupi_x;
	    vx = -dy / mg * adupi_x;
	    vy = dx / mg * adupi_y;
	    sx = (vx < 0) ? -1 : 1;
	    sy = (vy < 0) ? -1 : 1;
	    wx = -hw * vx;
	    wy = -hw * vy;
	    ms = wx * wx + wy * wy;
	    if (vx * sx > vy * sy) {
		while (wx * wx <= ms) {
		    x1 = xpos + wx;
		    y1 = ypos + wy;
		    x2 = x + wx;
		    y2 = y + wy;
		    vga_drawline(x1, y1, x2, y2);
		    cp = -sqrt(ms - wx * wx) * sx;
		    if (cp != 0) {
		        vga_drawline(x1, y1 - cp, x1, y1);
		        vga_drawline(x2, y2, x2, y2 + cp);
		    }
		    wx += sx;
		    ty = wx * vy / vx;
		    while (ty != wy) {
			vga_drawline(xpos + wx, ypos + wy, x + wx, y + wy);
			wy += sy;
		    }
	 	}
	    } else {
		while (wy * wy <= ms) {
		    x1 = xpos + wx;
		    y1 = ypos + wy;
		    x2 = x + wx;
		    y2 = y + wy;
		    vga_drawline(x1, y1, x2, y2);
		    cp = sqrt(ms - wy * wy) * sy;
		    if (cp != 0) {
			vga_drawline(x1 - cp, y1, x1, y1);
			vga_drawline(x2, y2, x2 + cp, y2);
		    }
		    wy += sy;
		    tx = wy * vx / vy;
		    while (tx != wx) {
			vga_drawline(xpos + wx, ypos + wy, x + wx, y + wy);
			wx += sx;
		    }
	 	}
	    }	
	}
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
	pw *= (-0.005);	/* fix-up negative values */
    pnwd = pw * adupi_x;	/* pen width in x-pixels */
    stroke = (pnwd > 1.5 && dev_.widestroke);	/* test for thick lines */
}

int
chkdev()
{
    char *s;

    if (getenv("DISPLAY"))
    	return (X_DEVICE);
    if (getenv("TERM"))
        if (strcmp(getenv("TERM"), "linux") == 0)
	    return (SVGA_DEVICE);
    return (DEFAULT_DEVICE);
}

static void
scr_clear()
{
    int x, y;

    vga_setegacolor(BGC(dev_.bgci));
    for (x = 0; x < xpix; x++)
	bgsl[x] = BGC(dev_.bgci);
    for (y = 0; y < ypix; y++)
	vga_drawscanline(y, bgsl);
}

static void
begin_page()
{
    cleared = 1;
    if (ncolors <= 2 || BGC(dev_.fnci) == BGC(dev_.bgci))
	scr_clear();
    else
	fountain();
    if (dev_.border)
	d4box();
}

static void
end_page()
{
    if (!cleared)
	begin_page();
    if (dev_.prompt) {
	beep();
	getchar();
    }
    cleared = 0;
}

/* d4box - draw box
*/
static void
d4box()
{
    float x = XPAGE, y = YPAGE, z = 0.0;

    d4trns(z, y, 0);
    d4trns(x, y, 1);
    d4trns(x, z, 1);
    d4trns(z, z, 1);
    d4trns(z, y, 1);
    fflush(stdout);
}

static void
fountain()
{
    int     i, ii, x, y, y1, y2;
    unsigned char fill[8];
    static unsigned char m[8] = {
	0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
    };
    static int oddnum = 21, nb = 65;

    for (i = 0; i < 8; i++)
	fill[i] = 0;
     y2 = y = 0;
    for (i = 0; i < nb; i++) {
	y1 = y2;
	y2 = ((i + 1) * ypix) / nb;
	for (y = y1; y < y2; y++) {
	    for (x = 0; x < xpix; x++)
		bgsl[x] = (fill[y % 8] & m[x % 8]) ? dev_.bgci : BGC(dev_.fnci);
	    vga_drawscanline(y, bgsl);
	}
	ii = (oddnum * i) % 64;
	fill[ii % 8] |= m[ii / 8];
    }
    for (x = 0; x < xpix; x++)
        bgsl[x] = BGC(dev_.bgci);
    while (y < ypix)
	vga_drawscanline(y, bgsl);
}
