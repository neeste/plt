/*
 * dshlin - plots solid lines or dashed lines
 *
 *	date of   first   version:  21-Jun-83
 *	programmer:  S. Neely
 *
 */

double  pythag(double, double);
void    tpl(double, double, double, double);
void    tpcc(float *, float *);

static int indash = 0;
static int dotcnt = 0;
static double extra = 0.0;

void
dot(double x, double y, double d)
{
    float xx[4], yy[4];
    static double a = 0.75;

    d /= 2;
    xx[0] = (float) (x - d);
    yy[0] = (float) y;
    xx[1] = (float) (x - d * a);
    yy[1] = (float) (y - d);
    xx[2] = (float) (x + d * a);
    yy[2] = yy[1];
    xx[3] = (float) (x + d);
    yy[3] = yy[0];
    tpcc(xx, yy);
    yy[1] = (float) (y + d);
    yy[2] = yy[1];
    tpcc(xx, yy);
}

void
dshlin(double xp, double yp, double xn, double yn,
       double dash, double space, double pwid, int ndot, int contin)
{
    double  seg, dr, dx, dy, ax, ay;
    double  x1, y1;

    if (contin == 0) {		/* initialize */
        indash = 0;
	extra = 0;
        dotcnt = ndot;
    }
    if (space == 0) {		/* draw solid line */
	tpl(xp, yp, xn, yn);	/* draw */
    } else {			/* draw dashed line */
	dr = pwid / 2;          /* dot radius */
        dx = xn - xp;
	dy = yn - yp;
	seg = pythag(dx, dy);	/* distance between data points */
	if (seg > 0.0) {	/* draw dash patterns */
	    ax = dx / seg;
	    ay = dy / seg;
	    x1 = xp;
	    y1 = yp;
	    while (seg >= extra) {	        /* do whole dashes and spaces */
		seg -= extra;
		x1 += extra * ax;
		y1 += extra * ay;
		if (dash < dr) {                /* draw dot */
		    dot(x1, y1, dr);
		    extra = space;
		} else if (dotcnt < ndot) {     /* draw dot after dash */
		    dot(x1, y1, pwid / 2);
		    extra = space;
		    indash = 0;
		    dotcnt++;
		} else if (indash) {	        /* draw dash */
		    tpl(xp, yp, x1, y1);
		    indash = 0;
		    extra = space;
                    dotcnt = 0;
		} else {
		    xp = x1;
		    yp = y1;
                    if (dotcnt == ndot) {
    		        indash = 1;
		        extra = dash;
                    }
		}
	    }
	    extra -= seg;	/* do a partial dash or space */
	    if (indash == 1)
		tpl(xp, yp, xn, yn);	/* draw */
	}
    }
}
