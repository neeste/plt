/* alfmod.c */

#include "alfcom.h"
#include "arccom.h"
#include <math.h>
#include <stdlib.h>

void alfa();
void nuxy();
void tarc();
void tdraw();

/*
 * alfmod - draw alpha character in proper font
 */
void
alfmod (int b, int *pxx, int *pyy)
{
    if( b < 0)
	return;
    alfa( b, pxx, pyy);
    return;
}

/*
 * alfarc  - draw an arc of a circle from (nx,ny) to (mx,my)
 */
#ifndef PI
#define	PI 3.1415927
#endif

void
alfarc(int lx, int ly, int *pnx, int *pny, int ia, float *ptr)
{
    int x,y, ixdif, iydif, ixret, iyret, np, ip, ixx, iyy;
    double dxy, angnxt, radius, cenang, angarc, sinang, rad, arclen;
    double sgmn, cn, sn, a11, a12, a21, a22, b1, b2, units, ang, ca, sa;

    ixdif = (*pnx-lx);
    iydif = (*pny-ly);
    dxy = sqrt((double)ixdif * (double)ixdif + (double)iydif * (double)iydif);
    if(dxy < 1.){
	nuxy(*pnx,*pny,ptr,&x,&y);
	tdraw(x,y);
	return;
	}
    ixret = lx;
    iyret = ly;
    angnxt = atan2( (double) iydif, (double) ixdif);
		
    if( abs(ia) >= 360 ){		/* # complete circle */
	radius = dxy/2.;
	cenang = angnxt + PI;
	angarc = 2*PI;
	*pnx = lx;
	*pny = ly;
	}
    else{					/* # part of circle */
	angarc = ia*PI/180.;
	sinang = sin(fabs(angarc/2.));
	if (sinang < .01){
	    tdraw(x,y);
	    return;
	    }
	radius = (dxy/2.)/sinang;
	if (ia > 0 && ia <= 180)
	    cenang = (3*PI - angarc)/2. + angnxt;
	else if(ia > 180 && ia < 360)
	    cenang = (-PI - angarc)/2. + angnxt;
	else if (ia < 0 && ia >= -180)
	    cenang = (-3*PI - angarc)/2. + angnxt;
	else if(ia < -180 && ia > -360)
	    cenang = (PI - angarc)/2. + angnxt;
	}
    rad = radius*sqrt(TR(1,1)*TR(2,2)-TR(1,2)*TR(2,1));
    arclen = fabs(angarc)*rad;
    if(arclen < arccom_.segmin)
	np = 0;
    else {
	sgmn = 2*PI*rad*arccom_.arcsmo/8.;
	if ( sgmn < arccom_.segmin)
	    sgmn = arccom_.segmin;
	np = arclen/sgmn;
	}
    cn = cos(cenang);
    sn = sin(cenang);
    a11 = ( TR(1,1)*cn + TR(1,2)*sn)*radius;
    a12 = (-TR(1,1)*sn + TR(1,2)*cn)*radius;
    a21 = ( TR(2,1)*cn + TR(2,2)*sn)*radius;
    a22 = (-TR(2,1)*sn + TR(2,2)*cn)*radius;
    b1 = TR(1,1)*ixret + TR(1,2)*iyret + TR(1,3) - a11;
    b2 = TR(2,1)*ixret + TR(2,2)*iyret + TR(2,3) - a21;
    if(alfcom_.slnang == -1) {
	ang = angarc * 0.5;
	ca = cos(ang);
	sa = sin(ang);
	ixx = a11*ca + a12*sa + b1 + .5;
	iyy = a21*ca + a22*sa + b2 + .5;
	nuxy(*pnx,*pny,ptr,&x,&y);
	tarc( (double) ixx, (double) iyy, (double) x, (double) y);
	}
    else {
	units = angarc/(np+1);
	for(ip = 1; ip <= np; ip++){
	    ang = units*ip;
	    ca = cos(ang);
	    sa = sin(ang);
	    ixx = a11*ca + a12*sa + b1 + .5;
	    iyy = a21*ca + a22*sa + b2 + .5;
	    tdraw(ixx,iyy);
	    }
	nuxy(*pnx,*pny,ptr,&x,&y);
	tdraw(x,y);
	}
}

void
nuxy(int ix, int iy, float *ptr, int *px, int *py)
{
    *px = TR(1,1)*ix + TR(1,2)*iy + TR(1,3);
    *py = TR(2,1)*ix + TR(2,2)*iy + TR(2,3);
}

/*
 * nunode - unpack coordinate bytes (nx,ny) from node
 */

#define	MSK	0377
void
nunode(int node, int *pnx, int *pny)
{
    *pnx = ((node>>8) & MSK) - 127;
    *pny = ( node & MSK ) - 127;
}

