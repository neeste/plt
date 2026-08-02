/* lj.c */
/* This file should contain all of the laserjet dependent code for IMP2LJ. */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#define dotset(x, y)    (lbuf[x][(y)/8] |= xbit[(y) & 7])
#define dotclr(x, y)    (lbuf[x][(y)/8] &= ~xbit[(y) & 7])

#define ESC      27
#define NSPOTMX	400
#define NCIRCMX 200

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
static int nspot = 0;
static int ncirc = 0;
static int lastpen = 0;
static int nlpb = 99;
static int nlpp = 99;
static int nlmx = 99;
static int xovlp = 0;
static int ljmag = 0;
static int firstprint = 1;
static int xspot[NSPOTMX];
static int yspot[NSPOTMX];
static int xcirc[NCIRCMX];
static int ycirc[NCIRCMX];
static long thistime = 0;
static long lasttime = 0;
static unsigned char xbit[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

/* prnsetup - initialize parameters for the Laserjet the printer */

void
prnsetup()
{
    int     i, xdpi, ydpi;

    if (prdpi != 150 && prdpi != 300)
	prdpi = 75;		/* dots per inch */
    ljmag = 300 / prdpi;	/* magnification */
    xdpi = ydpi = prdpi;	/* dots per inch x,y directions */
    xovlp = (2 * xdpi) / 25;	/* number of lines of overlap between bands */
    nlpb = (xdpi * 11);		/* number of lines per band */
#ifdef MSDOS
    if (prdpi > 200)
	nlpb /= 4;		/* not enough memory in PC for whole page */
#endif

    nlpp = (xdpi * 42) / 4;	/* # dots in x direction */
    nd = (ydpi * 31) / 4;	/* # dots in y direction */
    npts = nd / 8 + 2;
    nlmx = nlpb + xovlp;

    prnrestore();		/* reset printer, initialize output stream */
    lbuf = (buffer *) calloc((unsigned) nlmx, sizeof(buffer));
    if (lbuf == (buffer *) NULL) {
	(void) fprintf(stderr, "Error in imp2lj: problem with memory allocation\n");
	exit(1);
    }
    for (i = 0; i < nlmx; i++)
	if ((lbuf[i] = (buffer) calloc(npts, 1)) == (buffer) NULL) {
	    i--;
	    free(lbuf[i]);	/* free a little memory */
	    break;
	}
    nlpb = i - xovlp;
    if (nlpb <= 0) {
	(void) fprintf(stderr, "Error in imp2lj: insufficient memory\n");
	exit(1);
    }
    nband = (nlpp + (nlpb - 1)) / nlpb;
    xband = nlpb;
    xpage = nint(10.5 * xdpi);

    xmin = 0;
    ymin = 0;
    xmax = xband + xovlp;
    ymax = nd;

    if (verbose) {
	(void) fprintf(stderr, "imp2lj: %d dpi graphics", xdpi);
	if (nband > 1) {
	    (void) fprintf(stderr, ", %d lines/band", nlpb);
	    (void) fprintf(stderr, ", %d bands/page\n", nband);
	} else {
	    (void) fprintf(stderr, ", %d lines/page\n", nlpb);
	}
    }
}

/* prndump - output current band to the printer */

void
prndump()
{
    int     i, m;
    char    s[20];

    if (firstprint) {
	lprs("\033E");		/* RESET */
	lprs("\033&l0E");	/* top margin = 0 */
	(void) sprintf(s, "\033*t%dR", prdpi);
	lprs(s);		/* set resolution */
	lprs("\033*r0A");	/* start graphics at left margin */
	firstprint = 0;
    }
    if (band < nband)
	m = nlpb;
    else
	m = nlpp - nlpb * (nband - 1);
    for (i = 1; i <= m; i++) {
	prnlin(lbuf[i], npts);
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
    lprs("\033E");		/* RESET */
    lprfl();
}

/* prnlin - print a line of graphics commands */

void
prnlin(pb, n)
buffer  pb;
buflen  n;
{
    buflen  i;
    char    s[20];

    while (n > 1 && pb[n - 1] == 0)	/* trim zeros from end */
	n--;
    (void) sprintf(s, "\033*b%dW", n);
    lprs(s);
    lpr(pb, n);
    for (i = 0; i < n; i++)		/* zero buffer */
	pb[i] = 0;
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

    for (i = nlpb; i < (nlpb + xovlp); i++) {
	pb = lbuf[i];
	n = npts;
	while (n-- > 0)
	    *pb++ = 0;
    }
    firstprint++;
}

/* clear_band - zero pixel buffer for new band saving previous overlap */

void
clear_band()
{
    buffer  pa, pb;
    buflen  n;
    int     i;

    for (i = 0; i < xovlp; i++) {
	pa = lbuf[i + nlpb];
	pb = lbuf[i];
	n = npts;
	while (n-- > 0) {
	    *pb++ = *pa;
	    *pa++ = 0;
	}
    }
}

/* vline - set pixels from (x,y1) to (x,y2)  */

void
vline(x, y1, y2, c)
int     x, y1, y2;
{
    int     y;

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
        if (c)
            for (y = y1; y <= y2; y++)
                dotset(x, y);
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
    int     i, xx, yy;

    if (nspot == 1)
	dotset(x, y);
    else
	for (i = 0; i < nspot; i++) {
	    xx = x + xspot[i];
	    yy = y + yspot[i];
	    if (xmin <= xx && xx <= xmax && ymin <= yy && yy <= ymax)
		dotset(xx, yy);
	}
}

/* spotclr - clear pixels at (x,y) according to predefined spot pattern */

void
spotclr(x, y)
int     x, y;
{
    int     i, xx, yy;

    if (nspot == 1)
	dotclr(x, y);
    else
	for (i = 0; i < nspot; i++) {
	    xx = x + xspot[i];
	    yy = y + yspot[i];
	    if (xmin <= xx && xx <= xmax && ymin <= yy && yy <= ymax)
		dotclr(xx, yy);
	}
}

/* circset - set pixels at (x,y) according to predefined circ pattern */

void
circset(x, y)
int     x, y;
{
    int     i, xx, yy;

    if (ncirc == 1)
	dotset(x, y);
    else
	for (i = 0; i < ncirc; i++) {
	    xx = x + xcirc[i];
	    yy = y + ycirc[i];
	    if (xmin <= xx && xx <= xmax && ymin <= yy && yy <= ymax)
		dotset(xx, yy);
	}
}

/* circclr - clear pixels at (x,y) according to predefined circ pattern */

void
circclr(x, y)
int     x, y;
{
    int     i, xx, yy;

    if (ncirc == 1)
	dotclr(x, y);
    else
	for (i = 0; i < ncirc; i++) {
	    xx = x + xcirc[i];
	    yy = y + ycirc[i];
	    if (xmin <= xx && xx <= xmax && ymin <= yy && yy <= ymax)
		dotclr(xx, yy);
	}
}

/* make_spot - define a pattern of pixels to approximate a circular spot
 *             The diameter of the spot should be about  pen/300 inches.
 */

void
make_spot(pen)
{
    int     i1, i2, i, j, dsq, size, sizesq, x, y;

    if (pen == lastpen)
	return;
    lastpen = pen;
    size = pen / ljmag;
    if (size <= 1) {
	nspot = 1;
	xspot[0] = 0;
	yspot[0] = 0;
    } else {			/* size is bigger than 1 dot */
	nspot = 0;
	i1 = -size / 2 - 1;
	i2 = size / 2 + 1;
	sizesq = size * size;
	for (i = i1; i <= i2; i++) {
	    for (j = i1; j <= i2; j++) {
		if ((size % 2) != 0)	/* size is odd */
		    dsq = (i * i + j * j) * 4;
		else		/* size is even */
		    dsq = (i * i + j * j - i - j) * 4 + 2;
		if (dsq < sizesq) {
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
    if (nband > 1)
	return ((x + xoff) / ljmag - xband * (band - 1));
    else
	return ((x + xoff) / ljmag);
}

int
ycvt(y)
{
    return ((y + yoff) / ljmag);
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
