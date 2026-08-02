/*
 * getarc - get a circular arc of angle "arc" from (x1,y1) to (x2,y2)
 */
#include <math.h>

//
// ibe - interpolate between Bezier endpoints
//

int
ibe(float *x, float *y, int na)
{
    double a, b, c, d, s, t;
    double x1, y1, xa, ya, xb, yb, x2, y2;
    int n;

    x1 = x[0];
    y1 = y[0];
    xa = x[1];
    ya = y[1];
    xb = x[2];
    yb = y[2];
    x2 = x[3];
    y2 = y[3];
    for (n = 1; n < na; n++) {
	t = (double) n / na;
	s = 1 - t;
	a = s * s * s;
	b = t * s * s * 3;
	c = t * t * s * 3;
	d = t * t * t;
	x[n] = (float) (a * x1 + b * xa + c * xb + d * x2);
	y[n] = (float) (a * y1 + b * ya + c * yb + d * y2);
    }
    x[n] = (float) x2;
    y[n] = (float) y2;
    n++;
    return (n);
}

int
getarc(double x2, double y2, double arc, int ns, 
	 float *x, float *y, char *z)
{
    int     n, na;
    double  x1, y1, xm, ym, a, b, m;
    double c, s, d, e, xa, ya, xb, yb;
    static double dr = 3.1415927 / 180;
    static double hc = 180;

    if (arc == 0) {			    /* do a line */
        x[1] = (float) x2;
	y[1] = (float) y2;
        z[1] = 0;
	return (2);
    }
    x1 = x[0];
    y1 = y[0];
    if (arc <= -360 || 360 <= arc) {	    /* do a circle */
	n = getarc(x2, y2, hc, ns, x, y, z) - 1;
	n += getarc(x1, y1, hc, ns, x + n, y + n, z + n);
	return (n);
    }
    //
    // find mid-point of arc
    //
    a = arc / 2;
    b = cos(dr * a) / sin(dr * a);
    m = (a < 0 ? -1 : 1) * sqrt(1 + b * b) - b;
    xm = (x1 + x2 + (y2 - y1) * m) / 2;
    ym = (y1 + y2 - (x2 - x1) * m) / 2;
    if (arc <= -120 || 120 <= arc) {	/* bisect arc */
	n = getarc(xm, ym, a, ns, x, y, z) - 1;
	n += getarc(x2, y2, a, ns, x + n, y + n, z + n);
	return (n);
    }
    //
    // find Bezier control points
    //
    c = cos(dr * arc);
    s = sin(dr * arc);
    d = m * 2 / 3;
    e = d * s / (1 - c);
    xa = x1 + (y2 - y1) * d + (x2 - x1) * e;
    ya = y1 + (y2 - y1) * e - (x2 - x1) * d;
    xb = x2 + (x1 - xa) * c - (y1 - ya) * s;
    yb = y2 + (x1 - xa) * s + (y1 - ya) * c;
    //
    na = (int) ceil(fabs(ns * arc) / 360);
    x[1] = (float) xa;
    y[1] = (float) ya;
    x[2] = (float) xb;
    y[2] = (float) yb;
    x[3] = (float) x2;
    y[3] = (float) y2;
    if (ns > 0) {
        z[1] = 1;
	z[2] = (char) na;
	z[3] = 0;
	n = 4;
    } else {	// flatten curve if ns < 0
	n = ibe(x, y, na);
    }
    return (n);
}
