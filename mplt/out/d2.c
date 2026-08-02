/*
 * d2 - subroutines for producing impress commands for an Imagen printer
 */

#include <stdio.h>
#include <stdint.h>
#include "copa.h"
#include "output.h"
#include "device.h"
#include "clip.h"

#define ADUPI (300.)
#define	BLACK 15		/* imagen graphic operation */
#define	CIRCARC 150
#define	CREATEPATH 230
#define	DRAWPATH 234
#define	ENDPAGE 219
#define E_O_F 255
#define	FILLPATH 233
#define	LANDSCAPE 29		/* set hv: origin:0, axes:3, orientation:5 */
#define	NOOP 254
#define	SETABSH 135
#define	SETABSV 137
#define	SETHVSYSTEM 205
#define	SETPEN 232
#define	SETPUM 225
#define	SET_TEXTURE 231
#define	SHADE 3
#define	WHITE 0
#define	NPTS 1024

void    otrans(double, double, float *, float *);

static void putword(uint16_t);
static void putstring(char *);

static float xadupi = (float) 300.9;
static float yadupi = 297.5;
static float xoff = 75.;
static float yoff = 40.;	/* 1/4 inch offset from lower-left corner */

struct common {
    unsigned short penn, dcnt, *xd, *yd;
};

static struct common common_ = {0};

/* device 2 - Imagen laser printer */
int     d2open(void);
void    d2close(void), d2page(void);
void    d2trns(double, double, int);
void    d2pnwd(double);
void    d2fill(int, float *, float *, int, int);

/*
 * d2bind - override default device function
 */
void
d2bind(void)
{
    d_open = d2open;
    d_close = d2close;
    d_trns = d2trns;
    d_page = d2page;
    d_pnwd = d2pnwd;
    d_fill = d2fill;
}

/*
 * d2open - initialize plotting device
 */
int
d2open(void)
{
    common_.xd = (unsigned short *) calloc(NPTS * 2, sizeof(unsigned short));
    if (common_.xd == NULL)
	return (0);
    common_.yd = common_.xd + NPTS;
    common_.penn = 1;
    common_.dcnt = 0;
    common_.xd[0] = 0;
    common_.yd[0] = 0;
    dev_.adupi = ADUPI;
    dev_.txpr = TXPRSTK;
    dev_.maxpen = 20;
    dev_.prompt = 0;
    dev_.border = 0;
    dev_.fillwhite = 1;
    if (dev_.prompt)
	putstring("@document(language impress, jobheader on)");
    else
	putstring("@document(language impress, jobheader off)");
    put_ch(SETHVSYSTEM);
    put_ch(LANDSCAPE);
    put_ch(SETPUM);
    put_ch(0);
    put_ch(SETPEN);
    put_ch(2);
    return (1);
}

/*
 * d2close - close plotting device
 */
void
d2close(void)
{
    d2trns(0., 0., 0);
    put_ch(ENDPAGE);
    fflush(fpout);
}

/*
 * d2trns - convert coordinates to impress byte string
 *
 *	xx,yy	coordinates in inches	(NC)
 *	do_gs	0 = move
 *		1 = draw
 */
void
d2trns(double xx, double yy, int do_gs)
{
    int     i;

    if (do_gs == 0 || common_.dcnt >= NPTS) {
	if (common_.dcnt > 1) {
	    put_ch(CREATEPATH);
	    putword(common_.dcnt);
	    for (i = 0; i < (int) common_.dcnt; i++) {
		putword(common_.xd[i]);
		putword(common_.yd[i]);
	    }
	    put_ch(DRAWPATH);
	    put_ch(BLACK);
	}
	if (do_gs == 0) {	/* Start new path at next point */
	    common_.dcnt = 0;
	} else {		/* Start new path at last point */
	    common_.xd[0] = common_.xd[common_.dcnt - 1];
	    common_.yd[0] = common_.yd[common_.dcnt - 1];
	    common_.dcnt = 1;
	}
    }
    common_.xd[common_.dcnt] = (unsigned short) (xx * xadupi + xoff);
    common_.yd[common_.dcnt] = (unsigned short) (yy * yadupi + yoff);
    common_.dcnt++;
}

/*
 * d2page - signal new page (media change)
 */
void
d2page(void)
{
    d2trns(0., 0., 0);
    put_ch(ENDPAGE);
}

/*
 * d2pnwd - places impress instruction for pen
 */
void
d2pnwd(double pw)
{
    int     d;

    if (pw < 0)
	pw *= (-0.005);	/* fix-up negative values */
    d = (int) (pw * ADUPI);
    d = limit(1, d, dev_.maxpen);
    if (d != common_.penn) {
	d2trns(0., 0., 0);
	put_ch(SETPEN);
	put_ch(d);
	common_.penn = d;
    }
}

/*
 * d2fill - fill area with solid white or black
 *	  (do this in NC to avoid duplicating arrays)
 */
void
d2fill(int n, float *px, float *py, int fg, int nc)
{
    int     i, op;
    unsigned short ix, iy;
    float   xt, yt;

    d2trns(0., 0., 0);
    dev_.posknwn = 0;
    put_ch(CREATEPATH);
    putword((unsigned short) n);
    for (i = 0; i < n; i++) {
	if (nc) {
	    xt = px[i];
	    yt = py[i];
	} else {
	    otrans(px[i], py[i], &xt, &yt);
	    xt = limit(cclip_.tcx1, xt, cclip_.tcx2);	/* crude clipping */
	    yt = limit(cclip_.tcy1, yt, cclip_.tcy2);
	}
	ix = (int) (xt * xadupi + xoff);
	iy = (int) (yt * yadupi + yoff);
	putword(ix);
	putword(iy);
    }
    op = (fg) ? BLACK : WHITE;	/* fg=black, bg=white always */
    put_ch(FILLPATH);
    put_ch(op);
}

/*
 * putword - put a word to output file in "big endian" mode
 */
static void
putword(uint16_t w)
{
    put_ch(((w >> 8) & 0xff));
    put_ch((w & 0xff));
}

static void
putstring(char *s)
{
    while (*s)
	put_ch(*s++);
}

