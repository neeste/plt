/* 
 *  xpos.c - scale datum into position on page
 */
#include <math.h>
#include "plt.h"
#include "pltblk.h"

/*
 * xpos - scale x datum into position on page
 */
double
xpos(double x)
{
    if (!axes_.logx) {
	return (axes_.axx * x + axes_.axo);
    } else if (x > 0.0) {
	return (axes_.axx * log10(x) + axes_.axo);
    }
    return (0.0);
}

/*
 * ypos - scale y datum into position on page
 */
double
ypos(double y)
{
    if (!axes_.logy) {
	return (axes_.ayy * y + axes_.ayo);
    } else if (y > 0.0) {
	return (axes_.ayy * log10(y) + axes_.ayo);
    }
    return (0.0);
}

/*
 * rpos - scale right datum into position on page
 */
double
rpos(double r)
{
    if (!axes_.logr) {
	return (axes_.arr * r + axes_.aro);
    } else if (r > 0.0) {
	return (axes_.arr * log10(r) + axes_.aro);
    }
    return (0.0);
}

/*
 * tpos - scale top datum into position on page
 */
double
tpos(double t)
{
    if (!axes_.logt) {
	return (axes_.att * t + axes_.ato);
    } else if (t > 0.0) {
	return (axes_.att * log10(t) + axes_.ato);
    }
    return (0.0);
}
