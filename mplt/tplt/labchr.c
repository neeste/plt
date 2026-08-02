/* labchr.c */

#include "plt.h"
#include "pltblk.h"

int	getnum(char *, double *, char *);
int     indx(char *, int);
int     lookup(char *);
void    setsiz(void);
void    stprmv(int, double);

/* 
 * embed_param - parse embedded parameters in key 
 */

static int
embed_param(char *c)
{
    char   *s;
    int     i, j, k;
    double  prm;

    s = c + 1;

/* Check the parameter list to see if it is an arithmetic parameter. */
    if ((j = lookup(s)) < 0)
	return (1);

    /* Position pointer after the equals sign */
    if ((k = indx(s, '=')) <= 0)
	return (1);
    s += k;

/* If its not a letter, assume its a number */
    if ((i = getnum(s, &prm, "|,")) < 0)
	return (1);

    stprmv(j, prm);
    setsiz();

    return (k + i + 1);
}


/*
 * labchr - return next character in label
 */

int
labchr(char *ptext, int *pi, int *pspecial)
{
    double  t;
    int     i, k, j, jj, kk, icode, ii, special;

    special = 0;
    i = *pi;
    k = ptext[i];
    switch (k) {		/* See if this is a special character */
    case '\\':			/* insert character after backslash */
	if (ptext[i + 1] != '\0') {
	    k = ptext[++i];
	}
	if (k == '-')		/* hyphen (short dash) */
	    special = 7;
	if (k == ' ')		/* half space */
	    special = 8;
	if (k == 'b')		/* back space */
	    special = 9;
	if (k == 'h')		/* half back space */
	    special = 10;
	if (k == 'r')		/* return */
	    special = 11;
	if (k == '.')		/* set beg */
	    special = 12;
	break;
    case '$':			/* change dollar to space */
    case '\t':			/* change tab to space */
	k = ' ';
	break;
    case '{':			/* start superscript */
    case ']':			/* end subscript */
    case '}':			/* end superscript '}' */
    case '[':			/* start subscript '[' */
	special = 1;
	break;
    case '|':			/* Symbol in text? */
	/* Look for the second delimiter */
	for (j = 1; ptext[i + j] != '|' && ptext[i + j] != '\0'; j++)
	    continue;
	if (ptext[i + j] == '\0')
	    break;
	jj = ptext[i + 1];
	if ((j == 1) || (j == 2 && jj == '*')) {
	    k = (int) param_.symbol;	/* Current symbol */
	    i += j;
	    special = 2;
	} else if (ptext[i + j] == '|') {
	    /* decode symbol/shade/line number */
	    ii = (jj == '#' || jj == '_') ? 2 : 1;
	    kk = ptext[i + ii];
	    icode = 0;
	    if ((kk >= '0' && kk <= '9') || kk == '(' || kk == '-') {
		ii += getnum(ptext + i + ii, &t, "|,");
		icode = (int) t;
	    }
	    while (ptext[i + ii] == ',') {	/* look for embedded parameters */
		ii += embed_param(ptext + i + ii);
	    }
	    if (ii == j) {
		k = icode;
		i += j;
		if (jj == '_') {
		    special = 4;
		} else if (jj == '#') {
		    special = 3;
		} else if (0 <= k && k < 32) {
		    special = 2;
		} else if (k < -32) {
		    special = 6;
		    k = -k;
		}
	    }
	}
	break;
    case '^':			/* shift fonts */
	special = 5;
	break;
    }
    *pspecial = special;
    *pi = i;
    return (k);
}
