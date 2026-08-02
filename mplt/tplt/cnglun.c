/* cnglun.c */
/*
 * 	cnglun
 *
 *	DATE OF  FIRST  VERSION:	11-Nov-82
 *	DATE OF CURRENT VERSION:	 9-Sep-88
 *
 *	PURPOSE:	Closes selected logical unit
 *
 *	USAGE:		level = cnglun()
 *
 *	Functions called:
 *			errout, fclose, sprintf
 *
 *	Called by:
 *			proces
 *
 *
 *	PROGRAMMER:	M. Collins / E. Javel / M. Prucka
 *
 *********************************************************************
 */
#include "plt.h"
#include "pltblk.h"
#include "cincl.h"

int
cnglun(void)
{
/*
 *  Close the desired unit
 */
#ifdef DEBUG
    if (flags_.dbflg) {
	sprintf(ctext_.msg, " cnglun: inclev == %d\n", cincl_.inclev);
	errout(ctext_.msg);
    }
#endif
    if (cincl_.inclev <= 0) {
	return (EOF);
    } else if (fclose(misc_.lunin) < 0) {
	sprintf(ctext_.msg, " cnglun: tried to close invalid file\n");
	errout(ctext_.msg);
    } else {
	misc_.lunin = cincl_.inc_file[--cincl_.inclev];
    }
    return (cincl_.inclev);
}
