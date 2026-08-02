/* namfil.c */
/*
 *
 * 	subroutine namfil (NAMSTR,EXT)
 *
 *	PROGRAM NAME:			NAMFIL.RMC
 *
 *	DATE OF  FIRST  VERSION:	 1-Dec-79
 *	DATE OF CURRENT VERSION:	21-Jun-83
 *
 *	PURPOSE:	To ask for and recieve a file name from the
 *			console terminal.
 *
 *	USAGE:		call NAMFIL (NAMSTR,EXT)
 *
 *			Where	NAMSTR -- String to put file name into
 *				   EXT -- Default extension
 *
 *	PROGRAMMER:	Mike Collins / Eric Javel
 *
 *****************************************************************
 */
#include <string.h>
#include "plt.h"
#include "pltblk.h"

int     get_tty(char *, char *, int);
int     indx(char *, int);

void
namfil(char *namstr, char *ext)
{
    namstr[0] = '\0';
    while (namstr[0] == '\0' || namstr[0] == '*') {
	sprintf(ctext_.msg, "Enter FILE NAME [Default Extension: %s] -- ", ext);
	(void) get_tty(ctext_.msg, namstr, 24);
    }

/*
 * If no file extension exists and a default exists, add the default to NAMSTR
 */
    if (indx(namstr, '.') == 0 && *ext != '\0') {
	strcat(namstr, ext);
    }
}
