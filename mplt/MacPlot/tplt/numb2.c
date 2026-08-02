/*
 *
 *    USAGE:  numlen = NUMB2 (IAX,X,Y,FPN,NDIG)
 *
 *              IAX = Indicates axis being annotated and designates the
 *                    number of characters to use.  Values:
 *                    -2 -- Y-axis, horizontal orientation.  Left-justify
 *                          the value and move the character position
 *                          downward by half its height.
 *                    -1 -- Y-axis, vertical orientation.  Process NDIG
 *                          as described below.
 *                     0 -- X-axis; Process NDIG as described below.
 *                    >0 -- Y-axis, horizontal orientation.  Generate
 *                          this many characters, padding with leading
 *                          blanks, and move the character position
 *                          downward by half its height.
 *            (X,Y) = Starting X,Y coordinates for first character
 *              FPN = Number to be plotted
 *             NDIG = No. of digits and type of string to be plotted.  Values:
 *                    >0 = Real.  NDIG has no. of digits to right of decimal.
 *                    -1 = Integer.
 *
 ************************************************************************
 */
#include <math.h>
#include <string.h>
#include "plt.h"
#include "pltblk.h"
#include "fort.h"

double  pow_10(double);

double
numb2(double x, double y, double fpn, int ndig)
{
    double  z, val, qtxln(char *);
    char    str[26];
    int     i, j, nd;

/*
 * See what kind of number we are plotting, and round it
 */
    if (ndig < 0) {		/* Integer */
	z = 0.5;
	if (fpn >= 0.) {
	    val = floor(fpn + z);
	} else {
	    val = ceil(fpn - z);
	}
    } else {			/* Real */
	z = 5. / pow_10((double) (ndig + 1));
	if (fpn >= 0.) {
	    val = fpn + z;
	} else {
	    val = fpn - z;
	}
    }
/*
* Encode the value
*/
    sprintf(str, "%25.9f", val);
/*
* Look for the first printing character
*/
    for (i = 0; i < 25; i++) {
	if (str[i] != ' ')
	    break;
    }
/*
* Terminate the string in the correct place
*/
    if (ndig < 0) {
	j = 0;
    } else {
	j = ndig + 1;
    }
    str[(16 - 1) + j] = '\0';
    nd = (int)strlen(&str[i]);
    str[i + nd] = '\0';
/*
* Plot the string and bug out
*/
    text(x, y, &str[i]);
    return (qtxln(&str[i]));
}

/* ----------------------------------------------------------------- */


double
pow_10(double arg)
{
    static double ln10 = 2.302585092994045684;

    return (exp(arg * ln10));
}
