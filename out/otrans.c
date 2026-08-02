/*
 * otrans - output transformation
 */
#include "output.h"
#include "device.h"
#include "fort.h"
#include "clip.h"
#include "copa.h"
#include "cview.h"
#include "cwind.h"

void
otrans(double x, double y, float *pxt, float *pyt)
{
    if (copa_.rot % 2) {
	*pxt = (float) (copa_.oxy * y + copa_.oxo);
	*pyt = (float) (copa_.oyx * x + copa_.oyo);
    } else {
	*pxt = (float) (copa_.oxx * x + copa_.oxo);
	*pyt = (float) (copa_.oyy * y + copa_.oyo);
    }
}

/*
 * invtrans - inverse output transformation
 */
void
invtrans(double x, double y, float *pxt, float *pyt)
{
    if (copa_.rot % 2) {
	*pxt = (float) (copa_.ixy * y + copa_.ixo);
	*pyt = (float) (copa_.iyx * x + copa_.iyo);
    } else {
	*pxt = (float) (copa_.ixx * x + copa_.ixo);
	*pyt = (float) (copa_.iyy * y + copa_.iyo);
    }
}

/*
 * comp_trns - compute transformation
 */
void
comp_trns(void)
{
    float   x1, y1, x2, y2;

    copa_.rot = ((((int) cview_.vro) % 4) + 4) % 4;
    switch (copa_.rot) {
    case 0:
	copa_.oxx = cview_.vgn;
	copa_.oxo = -cview_.vgn * cwind_.wx1 + cview_.vx1;
	copa_.oyy = cview_.vgn;
	copa_.oyo = -cview_.vgn * cwind_.wy1 + cview_.vy1;
	copa_.ixx = 1 / copa_.oxx;
	copa_.ixo = -copa_.oxo / copa_.oxx;
	copa_.iyy = 1 / copa_.oyy;
	copa_.iyo = -copa_.oyo / copa_.oyy;
	break;
    case 1:
	copa_.oxy = -cview_.vgn;
	copa_.oxo = cview_.vgn * cwind_.wy2 + cview_.vx1;
	copa_.oyx = cview_.vgn;
	copa_.oyo = -cview_.vgn * cwind_.wx1 + cview_.vy1;
	copa_.ixy = 1 / copa_.oxy;
	copa_.ixo = -copa_.oxo / copa_.oxy;
	copa_.iyx = 1 / copa_.oyx;
	copa_.iyo = -copa_.oyo / copa_.oyx;
	break;
    case 2:
	copa_.oxx = -cview_.vgn;
	copa_.oxo = cview_.vgn * cwind_.wx2 + cview_.vx1;
	copa_.oyy = -cview_.vgn;
	copa_.oyo = cview_.vgn * cwind_.wy2 + cview_.vy1;
	copa_.ixx = 1 / copa_.oxx;
	copa_.ixo = -copa_.oxo / copa_.oxx;
	copa_.iyy = 1 / copa_.oyy;
	copa_.iyo = -copa_.oyo / copa_.oyy;
	break;
    case 3:
	copa_.oxy = cview_.vgn;
	copa_.oxo = -cview_.vgn * cwind_.wy1 + cview_.vx1;
	copa_.oyx = -cview_.vgn;
	copa_.oyo = cview_.vgn * cwind_.wx2 + cview_.vy1;
	copa_.ixy = 1 / copa_.oxy;
	copa_.ixo = -copa_.oxo / copa_.oxy;
	copa_.iyx = 1 / copa_.oyx;
	copa_.iyo = -copa_.oyo / copa_.oyx;
	break;
    }

    otrans(cclip_.cx1, cclip_.cy1, &x1, &y1);
    otrans(cclip_.cx2, cclip_.cy2, &x2, &y2);
    x1 = limit(0, x1, XPAGE);
    y1 = limit(0, y1, YPAGE);
    x2 = limit(0, x2, XPAGE);
    y2 = limit(0, y2, YPAGE);
    cclip_.tcx1 = amin1(x1, x2);
    cclip_.tcy1 = amin1(y1, y2);
    cclip_.tcx2 = amax1(x1, x2);
    cclip_.tcy2 = amax1(y1, y2);
    DEV_CLIP();
}
