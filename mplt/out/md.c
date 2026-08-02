/* md.c - move and out_draw (not required by 'tplt') */

#include "output.h"
#include "device.h"
#include "clip.h"
#include "copa.h"
#include "cview.h"
#include "cwind.h"

int     at_curpnt(double, double);
int     clip(float *, float *, float *, float *);
void    pencolor(int, int);
void    penwid(double);
void	set_curpnt(double, double);

static double px1 = 0;
static double py1 = 0;
/*
 * out_out_move - change the current point to (x,y)
 */

void
out_move(double x, double y)
{
    px1 = (float) x;
    py1 = (float) y;
}

/*
 * out_draw - from (px1,py1) to (px2,y2)
 */

void
out_draw(double px2, double y2)
{
    float   xt1, yt1, xt2, yt2;

    pencolor(copa_.curpci, 0);
    penwid((copa_.curplw < 0) ? copa_.curplw : copa_.curplw * 0.005);

    otrans(px1, py1, &xt1, &yt1);
    otrans(px2, y2, &xt2, &yt2);

/* Clip the line and plot it if there's anything left after clipping */

    if (clip(&xt1, &yt1, &xt2, &yt2)) {
	if (!at_curpnt(xt1, yt1)) {
	    DEV_TRNS(xt1, yt1, 0);
	}
	DEV_TRNS(xt2, yt2, 1);
	set_curpnt(xt2, yt2);
    }
    px1 = px2;
    py1 = y2;
}

void
set_curpnt(double x, double y)
{
    dev_.curxnc = (float) x;
    dev_.curync = (float)  y;
    dev_.posknwn = ((x >= 0) && (y >= 0));
}

int
at_curpnt(double x, double y)
{
    return (dev_.posknwn && x == dev_.curxnc && y == dev_.curync);
}
