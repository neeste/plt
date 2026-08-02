/* iscan.c */
/* 
 * iscan - PLT function to scan LIST array (length = NVALS),
 *	looking for an occurrence of STRNG.  Only the first 4 characters
 *	are considered.
 *
 *	USAGE:		I = ISCAN (LIST,NVALS,STRNG)
 *
 *			Where  LIST -- Array of 8-byte strings to consider
 *			      NVALS -- Number of elements in LIST
 *			      STRNG -- String to look for
 *				  I -- Function return.  If a match was
 *				       found between STRNG and an
 *				       element of LIST, it is set to
 *				       number of the element in LIST
 *				       where the match occurred.
 *				       Otherwise, function return is 0.
 */
#include <string.h>
#include "plt.h"
#include "pltblk.h"

#define	PNLN	4		    /* parameter name length */
#define	PNSZ	(PNLN+1)	    /* parameter array size  */

int chrupr(int);
int isalphnum(char);

int
iscan(char *plist[], int nvals, char *str)
{
    char    s[PNSZ];
    int     i, n;
/*
 * Get the first PNLN characters of the string and convert to upper case
 */
    for (i = 0; i < PNLN && isalphnum(str[i]); i++) {
	s[i] = chrupr(str[i]);
    }
    s[i] = '\0';
/*
 * Compare it with those in LIST
 */
    for (i = 0; i < nvals; i++) {
	n = (int)strlen(plist[i]);
	if (n < PNLN && strcmp(s, plist[i]) == 0)
	    return (i + 1);
	else if (strncmp(s, plist[i], PNLN) == 0)
	    return (i + 1);
    }
    return (0);
}

int
chrupr(int c)
{
    return ((c >= 'a' && c <= 'z') ? c + ('A' - 'a') : c);
}

int
inset(int c, char *s)
{
    char *p = s;

    while (*p != '\0')
	if (*p++ == c)
	    return (int)(p - s);
    return (0);
}

int
iswhite(char c)
{
    return ((c == ' ') || (c == '\t'));
}

int
isalph(char c)
{
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '_'));
}

int
isalphnum(char c)
{
    return (isalph(c) || (c >= '0' && c <= '9'));
}

char   *
skipwhite(char *s)
{
    while (iswhite(*s))
	s++;
    return (s);
}

char   *
skipalphnum(char *s)
{
    while (isalphnum(*s))
	s++;
    return (s);
}
