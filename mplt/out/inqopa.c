/*
 * inqopa - inquire output primitive attributes
 */
#include "output.h"
#include "device.h"
#include "fort.h"
#include "clip.h"
#include "copa.h"
#include "cview.h"
#include "cwind.h"

double  pythag(double, double);
void    setdch(void);

/*
 * inqplt - inquire plot line type
 */
int
inqplt()
{
    return (copa_.curplt);
}

/*
 * qtxlw - inquire text line width
 */
double
qtxlw()
{
    return (copa_.curtxlw);
}

/*
 * inqchh - inquire character height
 */
double
inqchh(void)
{
    return (copa_.curchh);
}

/*
 * qtxal - inquire text alignment
 */
void
qtxal(alignh, alignv)
int    *alignh, *alignv;
{
    *alignh = copa_.curtxalh;
    *alignv = copa_.curtxalv;
}

/*
 * inqchup - inquire character up vector
 */
void
inqchup(chux, chuy)
double *chux, *chuy;
{
    *chux = copa_.curchux;
    *chuy = copa_.curchuy;
}

/*
 * qtxln - inquire text length
 */
double
qtxln(char *pstr)
{
    double  txln;
    TXX     txx;
    static double z = 0;

    setdch();			/* set device character variables */
    if (copa_.curtxpr == TXPRSTK) {
	skqtxx(z, z, pstr, &txx);	/* Software characters */
    } else {
	DEV_QTXX(z, z, pstr, &txx);	/* Hardware characters */
    }
    txln = pythag(txx.xcat - txx.xtx0, txx.ycat - txx.ytx0);
    return (txln / cview_.vgn);
}

/*
 * inqbci - inquire black color index
 */
int
inqbci()
{
    return (dev_.bci);
}

/*
 * inqwci - inquire white color index
 */
int
inqwci()
{
    return (dev_.wci);
}
