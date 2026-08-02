/* tarc.c */

#include <math.h>
#include "tcom.h"
#include "arccom.h"
#define amax1(a,b) (((a) < (b)) ? (a) : (b))

#ifndef PI
#define PI 3.1415927
#endif
#define EPS 1.e-10

void circen();
void tdraw();
void tmove();

/*
 * tarc - tek arc
 */
void
tarc(double x1, double y1, double x2, double y2)
{
	double x0, y0;
	double rc,xc,yc, a0, ac, a;
	int x,y, n, i;
	double arclen, tmp, da;

	x0 = tcom_.currx; y0 = tcom_.curry;
	circen(x0,y0,x1,y1,x2,y2,&rc,&xc,&yc,&a0,&ac);
	arclen = rc*fabs(ac);
	if(arclen > arccom_.segmin) {
		tmp = (PI/4)*rc;
		n = arclen/amax1(tmp, arccom_.segmin);
	} else
		n=0;
	if(n > 0){
		da = ac/n;
		a = a0;
		for(i = 1;i < n; i++){
			a = a+da;
			tdraw((int)(xc+rc*cos(a)+.5),(int)(yc+rc*sin(a)+.5));
		}
	}
	tdraw( (int) x2, (int) y2);
	tcom_.lastx = tcom_.currx;
	tcom_.lasty = tcom_.curry;
}

