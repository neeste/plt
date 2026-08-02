/* setprm.c 
 *
 *
 * 	subroutine setprm (instr)
 *
 *
 *	DATE OF  FIRST  VERSION:	 5-Aug-79
 *	DATE OF CURRENT VERSION:	21-Jun-83
 *
 *	PURPOSE:	To assign values to the parameters for PLTFIL
 *
 *	USAGE:	call SETPRM (instr)
 *
 *		Where	instr  -- input string (contains parameter
 *				  name, "=", and the parameter value)
 *
 *	NOTES FOR PROGRAMMERS:	Certain parameters can accept text values.
 *				Any parameter can accept input from
 *				keyboard if an asterisk is supplied.
 *
 *	PROGRAMMER:	Mike Collins / Eric Javel / Matt Prucka
 *
 ********************************************************
 */
#include "plt.h"
#include "pltblk.h"
#include "fort.h"

int     sttxpm(char *);
int     stnmpm(char *);

void
setprm(char *instr)
{
/*
 *     Check if it's a text parameter.
 */
    if (sttxpm(instr) != 0)
	return;
/*
 *    Entry was not a text parameter, see if it's a numeric parameter
 */
    if (stnmpm(instr) != 0)
	return;
/*
 *     It's not numeric and it's not text, so it must be an error.
 */
    sprintf(ctext_.msg, " Invalid Parameter: %s\n", instr);
    errout(ctext_.msg);
}
