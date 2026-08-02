/* shadeare.c - shade an area */

#include "plt.h"
#include "pltblk.h"

void
shadearea(int n, float *px, float *py, int ssi)
{
    int     solid, hatch, outline, ci = 0, si = 0;

    if (ssi < 0)
	return;

    hatch = (ssi % 20) - 2;
    outline = ((ssi == 0) || ((ssi % 40) >= 20));
    if (ssi == 141) {           /* arrow */
	ci = (int) param_.pltcol;
	si = 0;
	solid = 1;
    } else if ((ssi % 20) == 1) {
	ci = (int) param_.shdcol;
	si = 0;
	solid = 1;
    } else if (ssi == 20) {
	ci = (int) param_.bgcol;
	si = 2;
	solid = 1;
    } else if (ssi / 40 == 1) {
	ci = (int) (param_.shdgray * 1000 + 0.5);
	si = 1;
	solid = 1;
    } else if (ssi / 40 == 2) {
	ci = (int) param_.filcol;
	si = 2;
	solid = 1;
    } else {
	solid = 0;
    }
    if (solid) {
	setfais(1);		/* fill area interior style = solid */
	setfasi(si);		/* fill area style index = color */
	if (misc_.colors)
	    setfaci(ci);	/* fill area color index */
	area(n, px, py);
//	if (ssi == 40 || ssi == 60)
	if (ssi == 40)
	    return;
    }
    if (hatch >= 0) {		/* HATCH FILL */
 	if (misc_.colors)
	    setfaci((int) param_.shdcol);
	setfalw(size_.shdlnw);	/* fill area line width */
	setfais(3);		/* fill area interior style = hatch */
	setfasi(hatch);		/* fill area style index */
	area(n, px, py);
    }
    if (outline) {		/* OUTLINE */
	if (misc_.colors)
	    setfaci((int) param_.pltcol);
	setfais(0);		/* fill area interior style = hollow */
	area(n, px, py);
    }
}
