/* solidarea - Fill area with solid pattern
 */
#include <stdio.h>
#include "output.h"
#include "device.h"
#include "copa.h"
#include "clip.h"
#include "cview.h"

#define	WHITE	0

int     test_box(int, float *, float *);
void    getbox(int, float  *, float  *, float  *, float  *, float  *, float  *);
void    pencolor(int, int);
void    tppcl(double, double, double, double, int, float *, float *, int);
void	set_curpnt(double, double);

/* solidarea - make interior of area solid */

void
solidarea(int n, float *px, float *py)
{
    pencolor(copa_.curfaci, copa_.curfasi);
    DEV_FILL(n, px, py, (copa_.curfasi != 2), 0);
}

/* skfill - stroke version of "fill area" primitive
 */

void
skfill(int n, float *px, float *py, int fg, int nc)
{
    float   bx1, by1, bx2, by2;
    float   tx1, ty1, tx2, ty2;
    float   pw, xx;
    int     oldfgci = 0;

    if (!fg && dev_.fillwhite < 0)
	return;
    getbox(n, px, py, &bx1, &by1, &bx2, &by2);
    if (bx1 > XMX || by1 > YMX || bx2 < XMN || by2 < YMN)
	return;
    if (!fg) {			/* switch color ? */
	oldfgci = dev_.fgci;
	dev_.fgci = dev_.bgci;
    }
    pw = (float) 0.01;			/* 0.01 inch */
    DEV_PNWD(pw);
    if (test_box(n, px, py)) {	/* can do "boxes" more efficiently */
        if (bx1 < XMN) bx1 = XMN;
        if (by1 < YMN) by1 = YMN;
        if (bx2 > XMN) bx2 = XMX;
        if (by2 > YMN) by2 = YMX;
	if (nc) {
	    tx1 = bx1;
	    ty1 = by1;
	    tx2 = bx2;
	    ty2 = by2;
	} else {
	    otrans(bx1, by1, &tx1, &ty1);
	    otrans(bx2, by2, &tx2, &ty2);
	}
	tx1 += (float) (0.5 * pw);
	tx2 -= (float) (0.5 * pw);
	DEV_TRNS(tx1, ty1, 0);
	DEV_TRNS(tx1, ty2, 1);
	for (xx = tx1 + pw; xx <= tx2; xx += pw) {
	    DEV_TRNS(xx, ty2, 1);
	    DEV_TRNS(xx, ty1, 1);
	    xx += pw;
	    if (xx <= tx2) {
		DEV_TRNS(xx, ty2, 1);
		DEV_TRNS(xx, ty1, 1);
	    }
	}
	DEV_TRNS(tx2, ty1, 0);
	DEV_TRNS(tx2, ty2, 1);
	set_curpnt(tx2, ty2);
    } else {			/* use full polygon clipping algorithm */
	bx1 += (float) (0.5 * pw);
	bx2 -= (float) (0.5 * pw);
	tppcl(bx1, by1, bx1, by2, n, px, py, nc);
	for (xx = bx1 + pw; xx <= bx2; xx += pw) {
	    tppcl(xx, by2, xx, by1, n, px, py, nc);
	    xx += pw;
	    if (xx <= bx2) {
		tppcl(xx, by1, xx, by2, n, px, py, nc);
	    }
	}
	tppcl(bx2, by1, bx2, by2, n, px, py, nc);
    }
    if (!fg)
	dev_.fgci = oldfgci;
}

int
test_box(int n, float *px, float *py)
{
    if (n != 4)
	return (0);
    if (px[0] == px[1] && px[2] == px[3] && py[1] == py[2] && py[3] == py[1])
	return (1);
    if (py[0] == py[1] && py[2] == py[3] && px[1] == px[2] && px[3] == px[1])
	return (1);
    return (0);
}
