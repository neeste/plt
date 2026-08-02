/* fx.c */
/* This file should contain all of the Epson FX dependent code for IMP2FX. */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#define xbit1(x)        (((x)+xrev) & 7)
#define xbit3(x)        ((((x)+xrev) / 3) & 7)
#define xbyte1(x)       (((x)+xrev) / 8)
#define ybyte1(x,y)     (y)
#define xbyte3(x)       (((x)+xrev) / 24)
#define ybyte3(x,y)     ((((x)+xrev) % 3) * nd + (y))
#define dotset(x, y)	(lbuf[xbyte1(x)][ybyte1(x,y)] |= xbit[xbit1(x)])
#define dotclr(x, y)	(lbuf[xbyte1(x)][ybyte1(x,y)] &= ~xbit[xbit1(x)])
#define dotsetq(x, y)   (lbuf[xbyte3(x)][ybyte3(x,y)] |= xbit[xbit3(x)])
#define dotclrq(x, y)   (lbuf[xbyte3(x)][ybyte3(x,y)] &= ~xbit[xbit3(x)])

#define ESC     27
#define NSPOTMX	256
#define NCIRCMX	128

extern int prdpi;
extern int verbose;
extern int band;
extern int nband;
extern int xband;
extern int xpage;
extern int xmin;
extern int ymin;
extern int xmax;
extern int ymax;
extern int xoff;
extern int yoff;

int     nint();
int     inside();
int     onspot();
void    lpr();
void    lprs();
void    lprfl();
void    prnlin();
void    prnrestore();

typedef char *buffer;
typedef unsigned buflen;

static buffer *lbuf;
static buflen npts = 0;
static int nd = 0;
static int quadd = 0;
static int gmode = 1;
static int nspot = 0;
static int ncirc = 0;
static int lastpen = 0;
static int nlpb = 99;
static int nlpp = 99;
static int nlmx = 99;
static int ovlp = 1;
static int lfwd = 1;
static int lrev = 1;
static int xrev = 24;
static int xdpi = 240;
static int ydpi = 216;
static int idpi = 300;
static int dotrad = 2;
static int firstprint = 1;
static int xspot[NSPOTMX];
static int yspot[NSPOTMX];
static int xcirc[NCIRCMX];
static int ycirc[NCIRCMX];
static long thistime = 0;
static long lasttime = 0;
static unsigned char xbit[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

/* prnsetup - initialize parameters for the EPSON-FX the printer */

void
prnsetup()
{
    int     i;

    if (prdpi == 240) {
	ovlp = 3;
	xdpi = 216;
	ydpi = 240;
	gmode = 3;
	quadd = 1;
	nlmx = 33 + lrev + lfwd;
    } else {
	ovlp = 1;
	xdpi = 72;
	ydpi = 120;
	gmode = 1;
	quadd = 0;
	nlmx = 99 + lrev + lfwd;
    }
    nd = ydpi * 8 + 1;
    npts = nd * ovlp;
    xrev = lrev * 8 * ovlp;

    lbuf = (buffer *) calloc((unsigned) nlmx, sizeof(buffer));
    if (lbuf == NULL) {
	(void) fprintf(stderr, "Error in imp2fx: problem with memory allocation\n");
	exit(1);
    }
    for (i = 0; i < nlmx; i++)
	if ((lbuf[i] = (buffer) calloc(npts, 1)) == NULL) {	/* no more memory */
	    i--;
	    free(lbuf[i]);	/* free a little memory */
	    break;
	}
    nlpb = i - lrev - lfwd;
    if (nlpb <= 0) {
	(void) fprintf(stderr, "Error in imp2fx: insufficient memory\n");
	exit(1);
    }
    nband = (nlpp + (nlpb - 1)) / nlpb;
    xband = nlpb * 24;
    xpage = nint(10.5 * 216);

    xmin = 0;
    ymin = 0;
    xmax = xband;
    ymax = 8 * 240;

    if (verbose) {
	(void) fprintf(stderr, "imp2fx: %dx%d dpi graphics", xdpi, ydpi);
	if (nband > 1)
	    (void) fprintf(stderr, ", %d bands/page", nband);
	(void) fprintf(stderr, "\n");
    }
}

/* prndump - output current band to the printer */

void
prndump()
{
    int     i, j, m, o;

    if (firstprint) {
	lprs("\033@\r");	/* reset printer */
	firstprint = 0;
    }
    if (band < nband)
	m = nlpb;
    else
	m = nlpp - nlpb * (nband - 1);
    o = ovlp - 1;
    for (i = lrev; i < m + lrev; i++) {
	for (j = 0; j < o; j++)
	    prnlin(lbuf[i] + nd * j, (unsigned) nd, 1);
	prnlin(lbuf[i] + nd * o, (unsigned) nd, 24 - o);
    }
    lprfl();
#ifdef MSDOS
    thistime = time(&lasttime);
#endif
}

/* prnrestore - restore printer to its default (power on) state */

void
prnrestore()
{
    lprs("\033@\r");		/* reset printer */
    lprfl();
}

/* prnlin - print a line of graphics commands with overprint if necessary */

void
prnlin(pb, n, lf)
buffer  pb;
buflen  n;
int     lf;
{
    buflen  j;
    char   *ovrbuf, s[20];
    int     c, ovrflg = 0, lc = 0;
    int     ac, mask;

    for (; n > 0 && pb[n - 1] == 0; n--)	/* trim */
	continue;
    if (n > 0) {
	s[0] = '\r';
	s[1] = ESC;
	s[2] = '*';
	s[3] = gmode;
	s[4] = n % 256;
	s[5] = n / 256;
	lpr(s, 6);
	ovrbuf = calloc((unsigned) (n + 1), 1);
	for (j = 0; j < n; j++) {
	    c = pb[j] & 255;
	    ac = (lc & c);
	    if (ovlp > 1 && ac != 0) {
		ovrbuf[j] = ac;
		mask = ~((unsigned) ac);
		c &= mask;
		ovrflg++;
	    }
	    lc = c;
	    pb[j] = c;
	}
	lpr(pb, n);
	for (j = 0; j < n; j++) {
	    pb[j] = 0;
	}
	if (ovrflg > 0) {
	    prnlin(ovrbuf, n, 0);
	}
	free(ovrbuf);
    }
    if (lf) {
	s[0] = ESC;
	s[1] = '3';
	s[2] = lf;
	s[3] = '\n';
	lpr(s, 4);
    }
#ifdef MSDOS
    thistime = time(&lasttime);
#endif
}

/* clear_page - zero entire pixel buffer for start of new page */

void
clear_page()
{
    buffer  pb;
    buflen  n;
    int     i;

    for (i = lrev; i < (lrev + lfwd); i++) {
	pb = lbuf[nlpb + i];
	n = npts;
	while (n-- > 0)
	    *pb++ = 0;
    }
}

/* clear_band - zero pixel buffer for new band saving previous overlap */

void
clear_band()
{
    buffer  pa, pb;
    buflen  n;
    int     i;

    for (i = lrev; i < (lrev + lfwd); i++) {
	pa = lbuf[i];
	pb = lbuf[nlpb + i];
	n = npts;
	while (n-- > 0) {
	    *pa++ = *pb;
	    *pb++ = 0;
	}
    }
}

/* vline - set pixels from (x,y1) to (x,y2)  */

void
vline(x, y1, y2, c)
int     x, y1, y2;
{
    int     y;

//fprintf(stderr, "vline: x=%d, y=(%d,%d)\n", x, y1, y2);
    if (y2 < y1) {
        y = y1;
        y1 = y2;
        y2 = y;
    }
    if (xmin <= x && x <= xmax && ymin <= y2 && y1 <= ymax) {
        if (y1 < ymin)
            y1 = ymin;
        if (y2 > ymax)
            y2 = ymax;
        if (c && quadd)
            for (y = y1; y <= y2; y++)
                dotsetq(x, y);
        else if (c && !quadd)
            for (y = y1; y <= y2; y++)
                dotset(x, y);
        if (!c && quadd)
            for (y = y1; y <= y2; y++)
                dotclrq(x, y);
        else
            for (y = y1; y <= y2; y++)
                dotclr(x, y);
    }
}

/* spotset - set pixels at (x,y) according to predefined spot pattern */

void
spotset(x, y)
int     x, y;
{
    int     i;

    if (quadd) {
	if (nspot == 1)
	    dotsetq(x, y);
	else
	    for (i = 0; i < nspot; i++)
		dotsetq(x + xspot[i], y + yspot[i]);
    } else {
	if (nspot == 1)
	    dotset(x, y);
	else
	    for (i = 0; i < nspot; i++)
		dotset(x + xspot[i], y + yspot[i]);
    }
}

/* spotclr - clear pixels at (x,y) according to predefined spot pattern */

void
spotclr(x, y)
int     x, y;
{
    int     i;

    if (quadd) {
	if (nspot == 1)
	    dotclrq(x, y);
	else
	    for (i = 0; i < nspot; i++)
		dotclrq(x + xspot[i], y + yspot[i]);
    } else {
	if (nspot == 1)
	    dotclr(x, y);
	else
	    for (i = 0; i < nspot; i++)
		dotclr(x + xspot[i], y + yspot[i]);
    }
}

/* circset - set pixels at (x,y) according to predefined circ pattern */

void
circset(x, y)
int     x, y;
{
    int     i;

    if (quadd) {
	if (ncirc == 1)
	    dotsetq(x, y);
	else
	    for (i = 0; i < ncirc; i++)
		dotsetq(x + xcirc[i], y + ycirc[i]);
    } else {
	if (ncirc == 1)
	    dotset(x, y);
	else
	    for (i = 0; i < ncirc; i++)
		dotset(x + xcirc[i], y + ycirc[i]);
    }
}

/* circclr - clear pixels at (x,y) according to predefined circ pattern */

void
circclr(x, y)
int     x, y;
{
    int     i;

    if (quadd) {
	if (ncirc == 1)
	    dotclrq(x, y);
	else
	    for (i = 0; i < ncirc; i++)
		dotclrq(x + xcirc[i], y + ycirc[i]);
    } else {
	if (ncirc == 1)
	    dotclr(x, y);
	else
	    for (i = 0; i < ncirc; i++)
		dotclr(x + xcirc[i], y + ycirc[i]);
    }
}

/* make_spot - define a pattern of pixels to approximate a circular spot
 *             The diameter of the spot should be about pen/300 inches.
 */

void
make_spot(pen)
{
    double  pw;
    int     i, j, i1, j1, i2, j2, x, y;
    int     xsize, ysize, xdsq, ydsq;
    long    xsc, ysc, sizesq, dsq;

    if (pen == lastpen)
	return;
    lastpen = pen;
    pw = pen / (double) idpi;
    xsize = 1 + (((pen - dotrad) * (long) xdpi) / idpi);
    ysize = 1 + (((pen - dotrad) * (long) ydpi) / idpi);
    if (xsize <= 1 && ysize <= 1) {
	nspot = 1;
	xspot[0] = 0;
	yspot[0] = 0;
    } else {			/* size is bigger than 1 dot */
	nspot = 0;
	i1 = -xsize / 2 - 1;
	j1 = -ysize / 2 - 1;
	i2 = xsize / 2 + 1;
	j2 = ysize / 2 + 1;
	xsc = ((long) ydpi * ydpi);
	ysc = ((long) xdpi * xdpi);
	sizesq = (long) ((pw * xsc) * (pw * ysc));
	for (i = i1; i <= i2; i++) {
	    for (j = j1; j <= j2; j++) {
		xdsq = (xsize % 2) ? (i * i) * 4 : (i * i - i) * 4 + 1;
		ydsq = (ysize % 2) ? (j * j) * 4 : (j * j - j) * 4 + 1;
		dsq = xdsq * xsc + ydsq * ysc;
		if (dsq < sizesq && nspot < NSPOTMX) {
		    xspot[nspot] = i;
		    yspot[nspot] = j;
		    nspot++;
		}
	    }
	}
    }
    ncirc = 0;
    for (i = 0; i < nspot && ncirc < NCIRCMX; i++) {
	x = xspot[i];
	y = yspot[i];
	if (!inside(x, y)) {
	    xcirc[ncirc] = x;
	    ycirc[ncirc] = y;
	    ncirc++;
	}
    }
}

int
inside(x, y)
{
    return (onspot(x - 1, y) && onspot(x + 1, y) &&
        onspot(x, y - 1) && onspot(x, y + 1) &&
        onspot(x - 1, y - 1) && onspot(x - 1, y + 1) &&
        onspot(x + 1, y - 1) && onspot(x + 1, y + 1));
}

int
onspot(x, y)
{
    int     i;

    for (i = 0; i < nspot; i++)
	if (xspot[i] == x && yspot[i] == y)
	    return (1);
    return (0);
}

int
xcvt(x)
{
    x = ((x + xoff) * (long) xdpi) / idpi;
    return (x - xband * (band - 1));
}

int
ycvt(y)
{
    y = ((y + yoff) * (long) ydpi) / idpi;
    return (y);
}

/* idle - send a do nothing command to hold the (shared) PC serial line */

void
idle()
{
#ifdef MSDOS
    if ((time(&thistime) - lasttime) > 2) {	/* every 2 seconds */
	lprs("\r");		/* carriage return */
	lprfl();			/* flush output */
	lasttime = thistime;
    }
#endif
}
