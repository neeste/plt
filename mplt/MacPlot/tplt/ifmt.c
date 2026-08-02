/* ifmt.c */
/*
 *
 * 	function ifmt (FMT)
 *
 *
 *	PROGRAM NAME:			IFMT.C
 *
 *	DATE OF  FIRST  VERSION:	20-Nov-79
 *	DATE OF CURRENT VERSION:	26-Jun-87
 *
 *	PURPOSE:	PLTFIL routine to decode a text annotation
 *			format specification into a form suitable
 *			to be sent as an argument to the Versaplot
 *			routine NUMB.
 *
 *	USAGE:		ndec = IFMT (fmt)
 *
 *			Where  pfmt -- pointer to char array containing
 *					  FORTRAN type format specification
 *
 *				 ndec --  -1 if format is integer
 *					   n if format is FX.n
 *
 *	PROGRAMMER:	Mike Collins / Eric Javel
 *
 ***************************************************************
 */

int indx(char *, int);

int
ifmt(char *pfmt)
{
    int     n;

    for (n = 0; pfmt[n] == ' '; n++)	/* skip leading blanks */
	continue;
    if (pfmt[n] == 'F' || pfmt[n] == 'f') {
	n = indx(pfmt, '.');	/* Default to integer if no "." */
	if (n > 0 && pfmt[n] >= '0' && pfmt[n] <= '9') {
	    return (pfmt[n] - '0');	/* Floating point format */
	}
    }
    return (-1);		/* Integer format */
}
