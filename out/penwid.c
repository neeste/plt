/*
 * penwid - set scaled penwidth
 */
#include <stdio.h>
#include "output.h"
#include "device.h"
#include "copa.h"
#include "cview.h"

void
penwid(double wid)
{
    DEV_PNWD((wid > 0) ? wid * cview_.vgn : wid);
    dev_.posknwn = 0;
}

void
pencolor(int ci, int si)
{
    if (si == 1) {
	dev_.fgci = limit(ct_min, ci, ct_max);
    	dev_.gray = 1;
    } else if (si == 2) {
	dev_.fgci = dev_.bgci;
    	dev_.gray = 0;
    } else {
	dev_.fgci = (ci >= 0 && ci < dev_.nci) ? ci : copa_.curfgci;
    	dev_.gray = 0;
    }
}
