/* sttxpm.c */

#include <string.h>
#include <stdlib.h>
#include "plt.h"
#include "pltblk.h"
#include "fort.h"

char   *stracp(char *, char *);
char   *skipwhite(char *);
int     get_tty(char *, char *, int);
int     indx(char *, int);
int     iscan(char *[], int, char *);

static char *txtnam[]=
{
    "XFMT", "YFMT", "TFMT", "RFMT",
    "XLAB", "YLAB", "TLAB", "RLAB",
    "XANN", "YANN", "TANN", "RANN",
    "XDAT", "YDAT", "ZDAT", "SELE"
};

static int ntnam = sizeof(txtnam) / sizeof(txtnam[0]);

int
sttxpm(char *instr)
{
    char  **param = &ctext_.xfmt, *s;
    int     j, k, jm1;

/*
 *    See if it is a text parameter
 */
    j = iscan(txtnam, ntnam, instr);
    if (j == 0)
	return (0);
    jm1 = j - 1;
/*
 *	Skip white space after the equals sign
 */
    k = indx(instr, '=');
    s = skipwhite(instr + k);
/*
 * 	Check to see if input from keyboard is desired
 */
    if (*s == '*') {
	sprintf(ctext_.msg, "\n Enter value for %s: ", txtnam[jm1]);
	if (get_tty(ctext_.msg, s, 128) != 0)
	    return (-1);
	s = skipwhite(s);
    }
/*
 * 	Copy the string into the appropriate place
 *	Memory for storing strings is allocated as needed.
*/
    param[jm1] = stracp(param[jm1], s);
    return (j);
}

char   *
stracp(char *s1, char *s2)
{
    char   *s;
    int     n;

    if (s1 != NULL)
	free(s1);
    n = (int)strlen(s2);
    s1 = s = (char *) malloc(n + 1);
    while (n--)
	*s1++ = *s2++;
    *s1 = '\0';
    return (s);
}
