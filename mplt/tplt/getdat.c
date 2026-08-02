/*
 * getdat - get floating-point numbers from string
 *
 *		usage: getdat(s, col, ncol)
 *
 *		where:
 *			s    = points to the string to be parsed
 *			col  = points to an array of double float which
 *			       contains default values on input
 *			       and interpreted values on output
 *			ncol = number of values in "col" array.
 *
 */

char   *skipwhite(char *);
int     getnum(char *, double *, char *);

int
getdat(char *s, double *col, int ncol)
{
    int     i, b;

    for (i = 0; i < ncol; i++) {
	if (*s == '\0')
	    continue;
	s = skipwhite(s);
	if (i > 0 && *s == ',')		/* skip comma */
	    s = skipwhite(++s);
	if (*s == '\0')
	    continue;
	b = getnum(s, col + i, " \t,;!\"");
	if (b < 0)
	    break;
	s += b;
    }
    return (i);
}
