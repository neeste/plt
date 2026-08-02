/* impress.c */
/* This file should contain all the IMPRESS dependent code for IMP2FX. */

#include <stdio.h>
#include <ctype.h>


#define ADUPI 300
#define	BLACK 15		/* imagen graphic operation */
#define	CIRCARC 150
#define	CREATEPATH 230
#define	DRAWPATH 234
#define	ENDPAGE 219
#define	E_O_F 255
#define	FILLPATH 233
#define	LANDSCAPE 29		/* set hv: origin: 0, axes: 3, orientation: 5 */
#define	NOOP 254
#define	NPTS 1024
#define	SETABSH 135
#define	SETABSV 137
#define	SETHVSYSTEM 205
#define	SETPEN 232
#define	SETPUM 225
#define	WHITE 0

extern FILE *in;		/* input stream (the only info passed via
				   global */

static short xd[NPTS], yd[NPTS];
static short hvsystem = LANDSCAPE;
static short pum = 0;
static short pen = 2;
static short ndpt = 0;

char   *strchr();
int     atoi();
int     save_place();
int     xcvt();
int     ycvt();
int     getword();
void    create_path();
void    draw_path();
void    fill_path();
void    idle();
void    doxy();
void    tpcl();
void    tppcl();
void    make_spot();

/* imp_scan - parses IMPRESS commands until EOF or ENDPAGE command */

int
imp_scan()
{
    short   c;

    while ((c = getc(in)) != EOF) {
	idle();
	switch (c) {
	case CREATEPATH:
	    create_path();
	    break;
	case DRAWPATH:
	    draw_path();
	    break;
	case ENDPAGE:
	    if (save_place())
		return (1);
	    break;
	case E_O_F:
	    return (EOF);
	case FILLPATH:
	    fill_path();
	    break;
	case NOOP:
	    break;
	case SETHVSYSTEM:
	    hvsystem = getc(in);
	    hvsystem = hvsystem;	/* access for lint */
	    break;
	case SETPEN:
	    pen = getc(in);
	    make_spot(pen);
	    break;
	case SETPUM:
	    pum = getc(in);
	    pum = pum;			/* access for lint */
	    break;
	case '@':
	    if (getc(in) == 'd')        /* assume it's a document header */
                while (getc(in) != ')') /* and ignore it */
                    continue;
	    break;
	default:
	    fprintf(stderr, "unrecognized IMPRESS command: %d\n", c);
	}
    }
    return (EOF);
}

/* create_path */

void
create_path()
{
    short   i, n;

    ndpt = 0;
    n = getword(in);
    if (n > NPTS) {
        fprintf(stderr, "Too many points (%d) in path [max=%d].\n", n, NPTS);
        exit(1);
    }
    for (i = 0; i < n; i++) {
	xd[ndpt] = getword(in);
	yd[ndpt] = getword(in);
	ndpt++;
    }
}

/* draw_path */

void
draw_path()
{
    short   i, style, color;
    short   x1, y1, x2, y2;

    style = getc(in);
    color = (style == BLACK) ? 1 : 0;
    x1 = xcvt(xd[0]);
    y1 = ycvt(yd[0]);
    for (i = 1; i < ndpt; i++) {
	x2 = xcvt(xd[i]);
	y2 = ycvt(yd[i]);
	tpcl(x1, y1, x2, y2, color);
	x1 = x2;
	y1 = y2;
    }
}

/* fill_path */

void
fill_path()
{
    short   i, style, color;
    short   x, xmn, ymn, xmx, ymx;

    style = getc(in);
    if (ndpt > 0) {
        color = (style == BLACK) ? 1 : 0;
        make_spot(1);
        xmn = xmx = xd[0];
        ymn = ymx = yd[0];
        for (i = 1; i < ndpt; i++) {
            if (xmn > xd[i])
                xmn = xd[i];
            if (ymn > yd[i])
                ymn = yd[i];
            if (xmx < xd[i])
                xmx = xd[i];
            if (ymx < yd[i])
                ymx = yd[i];
        }
        for (x = xmn; x <= xmx; x++)
            tppcl(x, ymn, x, ymx, color, ndpt, xd, yd);
        make_spot(pen);
    }
}

int
getword(fd)
FILE   *fd;
{
    short   a, b;

    a = getc(fd);
    b = getc(fd);
    return ((a << 8) | (b & 0xFF));
}
