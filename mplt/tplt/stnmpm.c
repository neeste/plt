/* stnmpm.c */

#include "plt.h"
#include "pltblk.h"
#include "fort.h"

char   *luname(int);
char   *skipwhite(char *);
int     chrupr(int);
int     getnum(char *, double *, char *);
int     get_tty(char *, char *, int);
int     indx(char *, int);
int     insnam(char *);
int     lookup(char *);
void    stprmv(int, double);

int
stnmpm(char *instr)
{
    char    ch1, ch2, *s;
    int     i, j, k;
    double  prm;

/*
 *     Check the parameter list to see if it is an arithmetic parameter.
 */
    if ((j = lookup(instr)) < 0)
	if ((j = insnam(instr)) < 0)
	    return (0);

/*
 *	Skip white space after the equals sign
 */
    k = indx(instr, '=');
    s = skipwhite(instr + k);
/*
 * 	Check to see if we need to get the value from the keyboard
 */
    if (*s == '*') {
	s = instr;
	sprintf(ctext_.msg, "\n Enter value for %s: ", luname(j));
	if (get_tty(ctext_.msg, s, 128) != 0)
	    return (-1);
    }
/*
* 	Change lower-case first two characters to upper-case
*/
    ch1 = chrupr(s[0]);
    ch2 = chrupr(s[1]);
    prm = 0.0;		/* default value */
/*
* 	Accept YES and NO as parameter entries for all parameters
*/
    if ((i = indx("NY", ch1)) > 0) {
	prm = i - 1;
    }
/*
* 	Accept SYMBOL, LINE, BOTH, HISTO, RECTANGLE, POLYGON,
*	WAVE, G=CONTIGUOUS_RECTANGLE, CUMMULATIVE, AND FREQUENCY 
*       as entries for PLTYPE
*/
    else if ((j == misc_.i_pltype) && ((i = indx("SLBHRPWGCF", ch1)) > 0)) {
	    prm = i;
    }
/*
* 	Accept INWARD and OUTWARD as entries for TICMRK
*/
    else if ((j == misc_.i_ticdir) && ((i = indx("OI", ch1)) > 0)) {
	    prm = i - 1;
    }
/*
* 	Accept NORMAL, LEFT, CENTER, and RIGHT for MHALIGN
*/
    else if ((j == misc_.i_mhalign) && ((i = indx("NLCR", ch1)) > 0)) {
	    prm = i - 1;
    }
/*
* 	Accept NORMAL, TOP, CAP, HALF, BASE, and BOTTOM for MVALIGN
*/
    else if ((j == misc_.i_mvalign) && ((i = indx("NTCHB", ch1)) > 0)) {
	if (ch1 == 'B' && ch2 == 'O')
	    prm = 5;
	else
	    prm = i - 1;
    }
/*
* 	If its not a letter, assume its a number
*/
    else if (getnum(s, &prm, ";!:") < 0) {
	prm = 0.0;
    }
    stprmv(j, prm);
    return (j + 1);
}
