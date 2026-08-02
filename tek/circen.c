/* circen.c */

#include <math.h>
#include "tcom.h"
#include "arccom.h"
#define amax1(a,b) (((a) < (b)) ? (a) : (b))

#ifndef PI
#define PI 3.1415927
#endif
#define EPS 1.e-10

#define dsign(a,b) ((0.0 <= (b)) ? fabs((double) (a)) : -fabs((double)(a)))
/*
 * circen - find the center and radius of a circle defined by 3 points
 */
void
circen(double x0, double y0, double x1, double y1, double x2, double y2,
    double *prc, double *pxc, double *pyc, double *pa0, double *pac)
{
        double xdiff, ydiff, a, b, c, s1, s2, d, an1, an2, an3, an4;
	double xc1, xc2, yc1, yc2, d1, d2, a1, a2, aa, ab;
	double rc, xc, yc;

	xdiff = x1 - x0;
	ydiff = y1 - y0;
	a = sqrt( xdiff*xdiff + ydiff*ydiff);
	xdiff = x2 - x1;
	ydiff = y2 - y1;
	b = sqrt( xdiff*xdiff + ydiff*ydiff);
	xdiff = x0 - x2;
	ydiff = y0 - y2;
	c = sqrt( xdiff*xdiff + ydiff*ydiff);
	if ( c < EPS ) {  /* degenerates to where a or b is the diameter */
	    *prc = .25*(a+b);	/* so take the average and compute radius */
	    xc = *pxc = (x0+x1)*.5;
	    yc = *pyc = (y0+y1)*.5;
	} else {
	    if ( a < EPS || b < EPS) {
		*prc = 1/EPS;	/* try rc == inf. (or close to it) */
	    } else {
		s1 = .5*(a+b+c);
		s2 = sqrt(s1*(s1-a)*(s1-b)*(s1-c));
		if(s2 < EPS) {
			*prc = .25*a*b*c/EPS;
		} else {
			*prc = .25*a*b*c/s2;
		}
	    }
	    rc = *prc;
	    d = sqrt(rc * rc - (.25*c*c));
	    an1 = atan2( y2 - y0, x2 - x0);
	    an2 = atan2( d, .5*c);
	    an3 = an1+an2-PI;
	    an4 = an1-an2-PI;
	    xc1 = x0 - rc * cos(an3);
	    yc1 = y0 - rc * sin(an3);
	    xc2 = x0 - rc * cos(an4);
	    yc2 = y0 - rc * sin(an4);
	    xdiff = x1 - xc1;
	    ydiff = y1 - yc1;
	    d1 = fabs( rc * rc - xdiff*xdiff - ydiff*ydiff);
	    xdiff = x1 - xc2;
	    ydiff = y1 - yc2;
	    d2 = fabs( rc * rc - xdiff*xdiff - ydiff*ydiff);
	    if(d1 < d2){
		    *pxc = xc1; xc = *pxc;
		    *pyc = yc1; yc = *pyc;
	    } else {
		    *pxc = xc2; xc = *pxc;
		    *pyc = yc2; yc = *pyc;
	    }
	}
	*pa0 = atan2( y0 - yc, x0 - xc);
	a1 = atan2( y1 - yc, x1 - xc);
	a2 = atan2( y2 - yc, x2 - xc);
	aa = a2 - *pa0;
	ab = a1 - *pa0;
	if( (fabs(aa) > fabs(ab)) && (dsign(1.,aa) == dsign(1.,ab)))
		*pac = aa;
	else
		*pac = aa - dsign(2*PI, aa);
}

