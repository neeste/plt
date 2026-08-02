/* setaxpar */

#include <math.h>
#include "plt.h"
#include "pltblk.h"

double  pow_10(double);

static double
lindif(double mn, double mx)
{
    return ((mn == mx) ? 1.e-30 : mx - mn);
}

static double
logdif(double mn, double mx)
{
    return ((mn == mx || mn <= 0.0 || mx <= 0.0) ?
	1.e-30 : log10(mx) - log10(mn));
}

/* fixlog - adjust log axis parameters for consistency */

static void
fixlog(double vint, float *pmin, float *pmax, float *pcyc)
{
    double  logbase;
    static double log10_2 = 0.301029996;

    logbase = (vint == -2.0) ? log10_2 : 1.0;
    if (*pmin != 0 && *pmax == 0)
	*pmax = (float) (*pmin * pow_10(logbase * floor(*pcyc)));
    if (*pmax != 0 && *pmin == 0)
	*pmin = (float) (*pmax / pow_10(logbase * floor(*pcyc)));
    if (*pmin > 0 && *pmax > 0)
	*pcyc = (float) (log10(*pmax / *pmin) / logbase);
}

void
setaxpar(void)
{
    double  a, b, c, d;
    int raxis, taxis;

    axes_.rxmin = param_.xmin;
    axes_.rxmax = param_.xmax;
    axes_.rymin = param_.ymin;
    axes_.rymax = param_.ymax;

    axes_.rxlen = param_.xlen;
    axes_.rylen = param_.ylen;
    axes_.rxllc = param_.xllc;
    axes_.ryllc = param_.yllc;

    axes_.logx = (param_.xcycle != 0.0
	&& (param_.xmin != 0.0 || param_.xmax != 0.0));
    axes_.logy = (param_.ycycle != 0.0
	&& (param_.ymin != 0.0 || param_.ymax != 0.0));
    axes_.logr = (param_.rcycle != 0.0
	&& (param_.rmin != 0.0 || param_.rmax != 0.0));
    axes_.logt = (param_.tcycle != 0.0
	&& (param_.tmin != 0.0 || param_.tmax != 0.0));

/*
     Process log axis:

    	1. Correct minimum and maximum values if not provided
    	2. Determine imaginary minimum and maximum if the ones provided
    	     are not a power of ten
    	3. Compute correct number of cycles, based on minimum and maximum
 */
    if (axes_.logx)
	fixlog(param_.xint, &axes_.rxmin, &axes_.rxmax, &param_.xcycle);
    if (axes_.logy)
	fixlog(param_.yint, &axes_.rymin, &axes_.rymax, &param_.ycycle);
    if (axes_.logt)
	fixlog(param_.tint, &param_.tmin, &param_.tmax, &param_.tcycle);
    if (axes_.logr)
	fixlog(param_.rint, &param_.rmin, &param_.rmax, &param_.rcycle);

/*
 * Define the transformation from a datum to position
 */

    a = param_.xper * 0.01;
    if (!axes_.logx) {
	b = a / lindif(axes_.rxmin, axes_.rxmax);
	c = axes_.rxmin;
    } else {
	b = a / logdif(axes_.rxmin, axes_.rxmax);
	c = log10(axes_.rxmin);
    }
    d = param_.xshift * 0.01;
    axes_.axx = (float) (param_.xlen * b);
    axes_.axo = (float) (param_.xlen * ((1 - a) / 2 - b * c + d) + param_.xllc);

    a = param_.yper * 0.01;
    if (!axes_.logy) {
	b = a / lindif(axes_.rymin, axes_.rymax);
	c = axes_.rymin;
    } else {
	b = a / logdif(axes_.rymin, axes_.rymax);
	c = log10(axes_.rymin);
    }
    d = param_.yshift * 0.01;
    axes_.ayy = (float) (param_.ylen * b);
    axes_.ayo = (float) (param_.ylen * ((1 - a) / 2 - b * c + d) + param_.yllc);

    a = param_.rper * 0.01;
    if (!axes_.logr) {
	b = a / lindif(param_.rmin, param_.rmax);
	c = param_.rmin;
    } else {
	b = a / logdif(param_.rmin, param_.rmax);
	c = log10(param_.rmin);
    }
    d = param_.rshift * 0.01;
    axes_.arr = (float) (param_.ylen * b);
    axes_.aro = (float) (param_.ylen * ((1 - a) / 2 - b * c + d) + param_.yllc);

    a = param_.tper * 0.01;
    if (!axes_.logt) {
	b = a / lindif(param_.tmin, param_.tmax);
	c = param_.tmin;
    } else {
	b = a / logdif(param_.tmin, param_.tmax);
	c = log10(param_.tmin);
    }
    d = param_.tshift * 0.01;
    axes_.att = (float) (param_.xlen * b);
    axes_.ato = (float) (param_.xlen * ((1 - a) / 2 - b * c + d) + param_.xllc);

    taxis = !param_.openax ||  (param_.tmin != 0) ||  (param_.tmax != 0);
    raxis = !param_.openax ||  (param_.rmin != 0) ||  (param_.rmax != 0);
    axes_.xaxis = (short) param_.xaxis;
    axes_.yaxis = (short) param_.yaxis;
    axes_.taxis = (short) (taxis && param_.taxis);
    axes_.raxis = (short) (raxis && param_.raxis);
}
