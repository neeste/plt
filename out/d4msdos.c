/*
 * d4msdos - subroutines for producing PC graphics
 */

#ifdef MSDOS
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <grlib.h>
#include "copa.h"
#include "output.h"
#include "device.h"
#include "clip.h"

#define XTRAN(x)    ((int)((x) * adupi_x))
#define YTRAN(y)    ((int)((YPAGE - (y)) * adupi_y))
#define BGC(c)	    ((c<0)?dev_.wci:c)

#define	FN      256
#define	MMV     512 | 0
#define	MDL     512 | 1
#define	MDR     512 | 2

#define	DT_SET  0
#define	DT_XOR  3
#define	beep()	{gr_play(beep_str); gr_setsrq(1);}
#define	BELL	7
#define	CHRWID	8
#define	CHRHGT	8
#define DEFAULT_DEVICE	1
#define PC_DEVICE	4

#define UNKNOWN	         0	/* Unknown board type */
#define MDA              1	/* IBM Monochrome card */
#define CGA              2	/* IBM Color graphics adapter (CGA) */
#define HGC              3	/* Hercules monochrome graphics card */
#define EGA_MONO         4	/* IBM Enhanced graphics adapter (EGA)
				   w/monochrome display */
#define EGA_COLOR        5	/* EGA w/color display */
#define EGA_HIGH         6	/* EGA w/high resolution color display */
#define VGA_MONO         7	/* IBM VGA hi res 2 color, 640x480 */
#define VGA_COLOR        8	/* IBM VGA hi res 16 color, 640x480 */
#define PEGA             9	/* Paradise Autoswitch EGA, 640 x 480 */
#define PVGA            10	/* Paradise VGA, 256 color, 640 x 480 */
#define SETMODE       1000	/* set mode explicitly */

extern int _gr_xpix;
extern int _gr_ypix;
extern int _gr_bppx;

static int getevent();
static int setgray();
static void begin_page();
static void end_page();
static void d4box();
static void cursor();
static void cursor_move();
static void cursor_del();
static void fountain();
static void ega_fountain();
static void vga_fountain();
static void reset_ega_colors();

static char beep_str[130] = "t900o7eb";
static float adupi_x = 0;
static float adupi_y = 0;
static float pnwd = 1.0;
static int c_on = 0, c_h = 4, c_w = 4, c_x = 0, c_y = 0, c_c = 15, c_bgc = 15;
static int mfgc = 0, mbgc = 1;	/* mono fg & bg colors */
static int tfgc = 0, tbgc = 7;	/* text fg & bg colors */
static int xpix = 720, ypix = 348, ncolors;
static int cleared = 0;
static int stroke = 0;
static int grafmode = 0;
static int orig_graf_mode = 0;
static short pc_video_adapt = 0;
static int mouse_present = 0;
static int mlb, mrb, mcx, mcy, mlx, mly;

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
    if (orig_graf_mode == 0)
	orig_graf_mode = gr_gtmod();
    switch (pc_video_adapt) {
    case CGA:
	grafmode = 6;
	break;
    case HGC:
	grafmode = 8;
	break;
    case EGA_COLOR:
	grafmode = 14;
	break;
    case EGA_MONO:
	grafmode = 15;
	break;
    case EGA_HIGH:
	grafmode = 16;
	break;
    case VGA_MONO:
	grafmode = 17;
	break;
    case VGA_COLOR:
	grafmode = 18;
	break;
    case PVGA:
	grafmode = 19;
	break;
    case PEGA:
	grafmode = 80;
	break;
    case SETMODE:
	break;
    default:
	return (0);
    }
    gr_init(grafmode, 0, 0);
    xpix = _gr_xpix;
    ypix = _gr_ypix;
    ncolors = 1 << _gr_bppx;

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
    gr_init(orig_graf_mode, 0, 0);
    reset_ega_colors();
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
    int     x, y, c;
    static int xpos = 0, ypos = 0;

    if (!cleared)
	begin_page();
    x = XTRAN(xx);
    y = YTRAN(yy);
    if (do_gs) {
	c = (ncolors <= 2 && dev_.fgci != dev_.bgci) ? mfgc : dev_.fgci;
	if (stroke)
	    gr_stroke(xpos, ypos, x, y, c, pnwd);
	else
	    gr_line(xpos, ypos, x, y, c);
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
    stroke = (pnwd > 1.5 && dev_.widestroke);	/* test for thick lines */
}

/*
 *      Fill a polygon area.
 */
void
d4fill(n, px, py, fg, nc)
int     n, fg;
float  *px, *py;
{
    int    *ix, *iy;
    float   xt, yt;
    int     i, c;

    if (n < 3)
	return;
    if (!cleared)
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
	ix[i] = XTRAN(xt);
	iy[i] = YTRAN(yt);
    }
    if (ncolors <= 2) {
	c = (fg) ? mfgc : mbgc;
    } else {
	c = (fg) ? dev_.fgci : BGC(dev_.bgci);
	if (dev_.gray)
	    c = setgray(c);
    }
    gr_areaf(n, ix, iy, c);
    free(ix);
}

void
d4setpal()
{
    int     i, n;
    double  s;

    if (ncolors <= 2)
	return;
    n = dev_.nci;
    s = (ct_max > 0) ? 1.0 / ct_max : 1.0;
    for (i = 0; i < n; i++)
	gr_setcolor(i, ct[i].r * s, ct[i].g * s, ct[i].b * s);
}

void
d4clip()
{
    int    ix1, iy1, ix2, iy2;

    ix1 = XTRAN(cclip_.tcx1);
    iy1 = YTRAN(cclip_.tcy1);
    ix2 = XTRAN(cclip_.tcx2);
    iy2 = YTRAN(cclip_.tcy2);
    gr_cliprect(ix1, iy1, ix2, iy2);
}

int
chkdev()
{
    char   *d, dev[9];
    int     c, i;

    if ((d = getenv("PLT_BELL")) != NULL) {
        strcpy(beep_str, d);
    }
    pc_video_adapt = gr_adapt();
    if ((d = getenv("PLT_DEVICE")) != NULL) {
	while (*d == ' ')
	    d++;
	for (i = 0; i < 8 && (c = *d++) > ' '; i++)
	    dev[i] = (c >= 'a' && c <= 'z') ? c + 'A' - 'a' : c;
	dev[i] = 0;
	if (dev[0] >= '0' && dev[0] <= '9') {
	    pc_video_adapt = SETMODE;
	    grafmode = atoi(dev);
	} else if (strcmp(dev, "HGC") == 0) {
	    pc_video_adapt = HGC;
	} else if (strcmp(dev, "CGA") == 0) {
	    pc_video_adapt = CGA;
	} else if (strcmp(dev, "EGA") == 0) {
	    pc_video_adapt = EGA_HIGH;
	} else if (strcmp(dev, "VGA") == 0) {
	    pc_video_adapt = VGA_COLOR;
	} else if (strcmp(dev, "PEGA") == 0) {
	    pc_video_adapt = PEGA;
	} else if (strcmp(dev, "PVGA") == 0) {
	    pc_video_adapt = PVGA;
	} else if (strcmp(dev, "TEK") == 0) {
	    pc_video_adapt = UNKNOWN;
	} else if (strcmp(dev, "VESA") == 0) {
	    pc_video_adapt = SETMODE;
	    grafmode = 257;	/* 640x480x256 colors */
	}
    }
    if (pc_video_adapt == UNKNOWN || pc_video_adapt == MDA)
	return (DEFAULT_DEVICE);
    return (PC_DEVICE);
}

static void
begin_page()
{
    cleared = 1;
    if (ncolors <= 2)
	gr_clear(mbgc);
    else if (BGC(dev_.fnci) == BGC(dev_.bgci))
	gr_clear(BGC(dev_.bgci));
    else
	fountain();
    if (dev_.border)
	d4box();
}

static void
end_page()
{
    int     c;

    if (!cleared)
	begin_page();
    if (dev_.prompt) {
	beep();
	while ((c = _getch())!= '\r')
	    if (c == '+')
		cursor();
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
cursor()
{
    char    s[40];
    static int xrf = 0;
    static int yrf = 0;
    static int xmn = 0;
    static int ymn = 0;
    static int xmx = 0;
    static int ymx = 0;
    static int xcs = 100;
    static int ycs = 100;
    static int xnw = -1;
    static int ynw = -1;
    static int d1 = 2;
    static int d2 = 50;
    static int c1 = 0;
    static int r1 = 0;
    static int c2 = 0;
    static int r2 = 0;
    static int r3 = 0;
    static int txw = 0;
    static int uch = 0;
    static int nomem = 0;
    static unsigned char *pixbuf = NULL;
    static char *un = "in";
    static float sf = 0.01;
    static unsigned int nb = 0;

    if (pixbuf == NULL) {
    	xmx = (int) (XPAGE * 100);
    	ymx = (int) (YPAGE * 100);
	txw = CHRWID * 19;
	c1 = xpix - txw;
	c2 = c1 + txw - 1;
	r1 = 0;
	r2 = r1 + CHRHGT;
	r3 = r2 + CHRHGT;
	nb = gr_imagesize(c1, r1, c2, r3);
	pixbuf = (unsigned char *) malloc(nb);
	nomem = (pixbuf == NULL);
	c_c = 15;
	c_bgc = (ncolors <= 2) ? mbgc : BGC(dev_.bgci);
	gr_settc(tfgc, tbgc);
	if (mouse_present = gr_minit()){
	    gr_marea(0, 0, 1050, 800);
	}
    }
    if (nomem) {
	beep();
    } else {
	gr_getimage(c1, r1, c2, r3, pixbuf);
    }

    for (;;) {
	if (xcs != xnw || ycs != ynw || uch) {
	    cursor_del();
	    xcs = limit(xmn, xcs, xmx);
	    ycs = limit(ymn, ycs, ymx);
	    sprintf(s, " %6.2f, %6.2f %s ",
		sf * (xcs - xrf), sf * (ycs - yrf), un);
	    gr_text(c1, r2, s);
	    if (xrf > 0 || yrf > 0) {
		sprintf(s, " %6.2f, %6.2f %s ", sf * xrf, sf * yrf, un);
		gr_text(c1, r3, s);
	    }
	    xnw = xcs;
	    ynw = ycs;
	    cursor_move(xcs, ycs);
	}
	switch (getevent()){
	case MMV:		/* mouse moved */
	    xcs = mcx;
	    ycs = 800 - mcy;
	    break;
	case FN | 71:		/* home */
	    xcs -= d1;
	    ycs += d1;
	    break;
	case FN | 72:		/* up arrow */
	    ycs += d1;
	    break;
	case FN | 73:		/* Pg Up */
	    xcs += d1;
	    ycs += d1;
	    break;
	case FN | 75:		/* left arrow */
	    xcs -= d1;
	    break;
	case FN | 77:		/* right arrow */
	    xcs += d1;
	    break;
	case FN | 79:		/* End */
	    xcs -= d1;
	    ycs -= d1;
	    break;
	case FN | 80:		/* down arrow */
	    ycs -= d1;
	    break;
	case FN | 81:		/* Pg Dn */
	    xcs += d1;
	    ycs -= d1;
	    break;
	case MDL:		/* mouse down left */
	case FN | 82:		/* Ins */
	    xrf = xcs;
	    yrf = ycs;
	    xnw = ynw = -1;
	    break;
	case MDR:		/* mouse down right */
	case FN | 83:		/* Del */
	    xrf = yrf = 0;
	    xnw = ynw = -1;
	    if (nomem)
		gr_rectf(c1, r1, c2, r2, c_bgc);
	    else
		gr_putimage(c1, r1, pixbuf, 0);
	    break;
	case '1':
	    xcs -= d2;
	    ycs -= d2;
	    break;
	case '2':
	    ycs -= d2;
	    break;
	case '3':
	    xcs += d2;
	    ycs -= d2;
	    break;
	case '4':
	    xcs -= d2;
	    break;
	case '6':
	    xcs += d2;
	    break;
	case '7':
	    xcs -= d2;
	    ycs += d2;
	    break;
	case '8':
	    ycs += d2;
	    break;
	case '9':
	    xcs += d2;
	    ycs += d2;
	    break;
	case 'c':
	    un = "cm";
	    sf = 0.0254;
	    uch++;
	    break;
	case 'i':
	    un = "in";
	    sf = 0.01;
	    uch++;
	    break;
	case '+':
	case 27:		/* Escape */
	    cursor_del();
	    xnw = ynw = -1;
	    if (nomem)
		gr_rectf(c1, r1, c2, r2, c_bgc);
	    else
		gr_putimage(c1, r1, pixbuf, 1);
	    return;
	default:
	    continue;
	}
    }				/* end for */
}

static void
cursor_move(x, y)
{
    cursor_del();
    c_x = XTRAN(0.01 * x);
    c_y = YTRAN(0.01 * y);
    gr_dotty(DT_XOR);
    gr_line(c_x - c_w, c_y, c_x + c_w, c_y, c_c);
    gr_line(c_x, c_y - c_h, c_x, c_y + c_h, c_c);
    gr_dotty(DT_SET);
    c_on = 1;
    if (mouse_present) {
	mlx = x;
	mly = 800 - y;
	gr_msetpos(mlx, mly);
    }
}

static void
cursor_del()
{
    if (c_on) {			/* remove the old line */
	gr_dotty(DT_XOR);
	gr_line(c_x - c_w, c_y, c_x + c_w, c_y, c_c);
	gr_line(c_x, c_y - c_h, c_x, c_y + c_h, c_c);
	gr_dotty(DT_SET);
	c_on = 0;
    }
}

static int
getevent()
{
    int     c, llb, lrb;

    for (;;) {
	if (_kbhit()){
	    c = _getch();
	    if (c == 0)
		c = FN | _getch();
	    return (c);
	}
	if (mouse_present) {
	    llb = mlb;
	    lrb = mrb;
	    gr_mstatus(&mlb, &mrb, &mcx, &mcy);
	    if (mlb && !llb)
		return (MDL);
	    if (mrb && !lrb)
		return (MDR);
	    if (mcx != mlx || mcy != mly) {
		return (MMV);
	    }
	}
    }
}

static void
fountain()
{
    if (dev_.nci < 125 && 255 <= ncolors)
	vga_fountain();
    else
	ega_fountain();
}

static void
ega_fountain()
{
    int     i, ii, y1, y2;
    unsigned char fill[8];
    static unsigned char m[8] =
    {
	0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
    };
    static int oddnum = 21, nb = 65;

    for (i = 0; i < 8; i++)
	fill[i] = 0;
    y2 = 0;
    for (i = 0; i < nb; i++) {
	y1 = y2;
	y2 = ((i + 1) * _gr_ypix) / nb;
	gr_setfillmask(NULL);
	gr_rectf(0, y1, _gr_xpix - 1, y2 - 1, BGC(dev_.fnci));
	gr_setfillmask(fill);
	gr_rectf(0, y1, _gr_xpix - 1, y2 - 1, BGC(dev_.bgci));
	ii = (oddnum * i) % 64;
	fill[ii % 8] |= m[ii / 8];
    }
    gr_setfillmask(NULL);
    gr_rectf(0, y2, _gr_xpix - 1, _gr_ypix - 1, BGC(dev_.bgci));
}

static void
vga_fountain()
{
    double  fnr, fng, fnb, bgr, bgg, bgb, rr, gg, bb, s;
    int     i, y1, y2, bgc, fnc;
    static int nb = 65;

    bgc = BGC(dev_.bgci);
    fnc = BGC(dev_.fnci);
    fnr = ct_scale(ct[fnc].r);
    fng = ct_scale(ct[fnc].g);
    fnb = ct_scale(ct[fnc].b);
    bgr = ct_scale(ct[bgc].r);
    bgg = ct_scale(ct[bgc].g);
    bgb = ct_scale(ct[bgc].b);
    y2 = 0;
    for (i = 0; i < nb; i++) {
	y1 = y2;
	y2 = ((i + 1) * (long) (_gr_ypix - 1)) / nb;
	s = i / (nb - 1.0);
	rr = fnr * (1 - s) + bgr * s;
	gg = fng * (1 - s) + bgg * s;
	bb = fnb * (1 - s) + bgb * s;
	gr_setcolor(125 + i, rr, gg, bb);
	gr_rectf(0, y1, _gr_xpix - 1, y2 - 1, 125 + i);
    }
    gr_rectf(0, y2, _gr_xpix - 1, _gr_ypix - 1, dev_.bgci);
}

static int
setgray(g)
{
    int     i, n, c;
    int     ir, ig, ib;
    int     d, dmn;
    double  gray;

    n = dev_.nci;
    if (n < 190 && 255 <= ncolors) {
	gray = ct_scale(g);
	c = (int) (190 + 64 * gray);
	gr_setcolor(c, gray, gray, gray);
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

void
reset_ega_colors()
{
    extern RGB_COLOR ct16[16];

    if (ct != ct16) {
	ct = ct16;
	dev_.nci = 16;
	d4setpal();		/* reset default colors */
    }
}

#endif
