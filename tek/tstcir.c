/* tstcir.c */

#include <stdio.h>
#include <math.h>
#include "tcom.h"
#include "arccom.h"

main()
{
    int ci;
    double x0,y0,x1,y1,x2,y2,rc,xc,yc,a0,ac;

    for(;;) {
	printf("Enter three points: x0,y0,x1,y1,x2,y2 :");
	scanf("%f %f %f %f %f %f",&x0,&y0,&x1,&y1,&x2,&y2);
	printf("x0,y0,x1,y1,x2,y2 = %f %f %f %f %f %f\n",x0,y0,x1,y1,x2,y2);
	ci = circen(x0,y0,x1,y1,x2,y2,&rc,&xc,&yc,&a0,&ac);
	printf("ci,rc,xc,yc,a0,ac = %d %f %f %f %f %f\n",ci,rc,xc,yc,a0,ac);
    }
}
