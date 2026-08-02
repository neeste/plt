/*
 *  alfa0 - font0
 */

#include <stdio.h>
#include "alfcom.h"
#include "font0.x"
#include "font11.x"

#define	SPECIAL	128
#define	MOVE	-126
#define	ARC	-121
#define	CHARWIDTH 55

void alfarc();
void nunode();
void nuxy();
void tdraw();
void tmove();

static int nkhar = NKHAR_0;
static int *khar = khar_0;
static int *node = node_0;

void
set_stdfont()
{
    nkhar = NKHAR_0;
    node = node_0;
    khar = khar_0;
}

void
set_altfont()
{
    nkhar = NKHAR_11;
    node = node_11;
    khar = khar_11;
}

void
alfa (int b, int *pxx, int *pyy)
{
        int x,y,fnode,lnode;
	float *ptr = alfcom_.tr;

	int j, lx, nx, ly, ny, ia;

	if ( b < 0 || b >= nkhar )	/* Make sure index is in range */
	    return;

	TR(1,3) = *pxx;
	TR(2,3) = *pyy;
	fnode = khar[b];		/* # Index  for  first node */
	lnode = khar[b+1] - 1;		/* # Index for last node */
/*
 *	# Process nodes until we finish the symbol or character
 */
	for (j = fnode; j <= lnode; j++) {
	    lx = nx;
	    ly = ny;
	    nunode(node[j-1],&nx,&ny);
	    if (nx == SPECIAL){
		if (ny == MOVE){		/* # move */
		    nunode(node[j], &nx, &ny);
       	    	    nuxy(nx, ny, ptr, &x, &y);
		    tmove(x, y);
		    j++;
		} else if (ny == ARC){		/* # arc */
		    ia = node[j];
		    nunode(node[j+1],&nx,&ny);
		    alfarc(lx, ly, &nx, &ny, ia, ptr);
		    j += 2;
		} else {
		    printf(" special code: nx,ny = %d %d\n", nx, ny);
		}
	    } else {				/* # draw */
       		nuxy(nx, ny, ptr, &x, &y);
		tdraw(x, y);
	    }
	}
/*
 *	# All done.  Find xx and yy of next character.
 */
	*pxx = TR(1,3) + TR(1,1)*CHARWIDTH + 0.5;
	*pyy = TR(2,3) + TR(2,1)*CHARWIDTH + 0.5;
	tmove(*pxx, *pyy);
}

